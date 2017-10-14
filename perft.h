#ifndef PERFT_H
#define PERFT_H

#include "move_list.h"
#include "moves.h"
#include "moves_generator.h"
#include "pawns_moves.h"
#include "zobrist.h"
#include "cache.h"


template <bool sideToMove, bool useCache>
U64 Perft(const ChessBoard &board, AllMoves &allMoves, U64 kingsMoves, int depth);


template <bool sideToMove, class M, bool useCache>
inline U64 iterateOverMoves(const ChessBoard &board, MoveList<M> &moveList, int depth) {

	U64 nodes = 0;

	M* moves = moveList.getMoves();
    while(moveList.hasNextMove(moves))  {

    	M move = *moves++;

    	U64 moveKey;

		if(useCache) {
			moveKey = board.createKey<sideToMove>(move);
			U64 value = CACHE::get(board.getKey() ^ moveKey ^ ZOBRIST::getDepthKey(depth));

			if(value) {
				nodes += value;
				continue;
			}
		}
		ChessBoard copy = board.makeMove<sideToMove>(move);
		if(useCache) {
			copy.updateKey(moveKey);
		}


		AllMoves allMoves;
		U64 kingMoves = setCheckAndPinners<sideToMove>(copy);
		if(copy.isCheck()) {
			if(copy.isDoubleCheck()) {
				nodes += Perft<!sideToMove, DOUBLE_CHECK, useCache>(copy, kingMoves, allMoves, depth);
			} else {
				nodes += Perft<!sideToMove, CHECK, useCache>(copy, kingMoves, allMoves, depth);
			}
		} else {
			nodes += Perft<!sideToMove, NO_CHECK, useCache>(copy, kingMoves, allMoves, depth);
		}
    }
    return nodes;
}

template <bool sideToMove, bool useCache>
inline U64 castleShort(const ChessBoard &board, int depth) {

	U64 moveKey = board.createKeyKingSideCastle<sideToMove>();

	if(useCache) {
		U64 value = CACHE::get(board.getKey() ^ moveKey ^ ZOBRIST::getDepthKey(depth));
		if(value) {
			return value;
		}
	}
	ChessBoard copy = board.makeShortCastle<sideToMove>();
	if(useCache) {
		copy.updateKey(moveKey);
	}
	U64 kingMoves = setCheckAndPinners<sideToMove>(copy);

	AllMoves allMoves;
	if(copy.isCheck()) {
		return Perft<!sideToMove, CHECK, useCache>(copy, kingMoves, allMoves, depth);
	} else {
		return Perft<!sideToMove, NO_CHECK, useCache>(copy, kingMoves, allMoves, depth);
	}
}

template <bool sideToMove, bool useCache>
inline U64 castleLong(const ChessBoard &board, int depth) {

	U64 moveKey = board.createKeyQueenSideCastle<sideToMove>();

	if(useCache) {
		U64 value = CACHE::get(board.getKey() ^ moveKey ^ ZOBRIST::getDepthKey(depth));
		if(value) {
			return value;
		}
	}
	ChessBoard copy = board.makeLongCastle<sideToMove>();
	if(useCache) {
		copy.updateKey(moveKey);
	}

	U64 kingMoves = setCheckAndPinners<sideToMove>(copy);

	AllMoves allMoves;
	if(copy.isCheck()) {
		return Perft<!sideToMove, CHECK, useCache>(copy, kingMoves, allMoves, depth);
	} else {
		return Perft<!sideToMove, NO_CHECK, useCache>(copy, kingMoves, allMoves, depth);
	}
}

template <bool sideToMove, CHECK_T check, bool useCache>
U64 Perft(ChessBoard &board, U64 kingsMoves, AllMoves &allMoves, int depth) {

	if(check != DOUBLE_CHECK) {
		generateMoves<sideToMove, check>(board, allMoves);
		generatePawnsMoves<sideToMove, check>(board, allMoves);
	}
	generateMovesAndCapturesForKing<sideToMove>(kingsMoves, board, allMoves);

	if(check == NO_CHECK) {
		if(isQueenSideCastlePossible<sideToMove>(board)) {
			allMoves.addQueenSideCastle();
		}

		if(isKingSideCastlePossible<sideToMove>(board)) {
			allMoves.addKingSideCastle();
		}
	}

    if(depth > 1) {

    	depth--;
    	U64 nodes = 0;

    	nodes += iterateOverMoves<sideToMove, Capture, useCache>(board, allMoves.captures, depth);
    	nodes += iterateOverMoves<sideToMove, Move, useCache>(board, allMoves.moves, depth);
    	nodes += iterateOverMoves<sideToMove, Promotion, useCache>(board, allMoves.promotions, depth);
    	nodes += iterateOverMoves<sideToMove, PromotionCapture, useCache>(board, allMoves.promotionCaptures, depth);


    	if(!check) {
			if(allMoves.longCastle) {
				nodes += castleLong<sideToMove, useCache>(board, depth);
			}
			if(allMoves.shortCastle) {
				nodes += castleShort<sideToMove, useCache>(board, depth);
			}
    	}

    	if(useCache) {
    		CACHE::put(board.getKey() ^ ZOBRIST::getDepthKey(depth+1), nodes);
    	}
    	return nodes;
    } else {
    	U64 nodes = allMoves.allMovesCount();
    	if(useCache) {
    		CACHE::put(board.getKey() ^ ZOBRIST::getDepthKey(1), nodes);
    	}
    	return nodes;
    }

}


#endif
