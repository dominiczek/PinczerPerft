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
U64 iterateOverMoves(const ChessBoard &board, MoveList<M> &moveList, int depth) {

	U64 nodes = 0;

	M* moves = moveList.getMoves();
    while(moveList.hasNextMove(moves))  {

    	M move = *moves++;

		ChessBoard copy = board.makeMove<sideToMove>(move);
		if(useCache) {
			U64 value = CACHE::get(copy.getKey() ^ ZOBRIST::getDepthKey(depth));

			if(value) {
				nodes += value;
				continue;
			}
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

//2252209110354 10 -1
//

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
				ChessBoard copy = board.makeLongCastle<sideToMove>();

				U64 value = CACHE::get(copy.getKey() ^ ZOBRIST::getDepthKey(depth+1));

				if(useCache && value) {
					nodes += value;
				} else {
					U64 kingMoves = setCheckAndPinners<sideToMove>(copy);

					AllMoves allMoves;
					if(copy.isCheck()) {
						nodes += Perft<!sideToMove, CHECK, useCache>(copy, kingMoves, allMoves, depth);
					} else {
						nodes += Perft<!sideToMove, NO_CHECK, useCache>(copy, kingMoves, allMoves, depth);
					}
				}
			}

			if(allMoves.shortCastle) {
				ChessBoard copy = board.makeShortCastle<sideToMove>();

				U64 value = CACHE::get(copy.getKey() ^ ZOBRIST::getDepthKey(depth+1));

				if(useCache && value) {
					nodes += value;
				} else {
					U64 kingMoves = setCheckAndPinners<sideToMove>(copy);
					AllMoves allMoves;

					if(copy.isCheck()) {
						nodes += Perft<!sideToMove, CHECK, useCache>(copy, kingMoves, allMoves, depth);
					} else {
						nodes += Perft<!sideToMove, NO_CHECK, useCache>(copy, kingMoves, allMoves, depth);
					}
				}
			}
    	}

    	if(useCache) {
    		CACHE::put(board.getKey() ^ ZOBRIST::getDepthKey(depth+1), nodes);
    	}
    	return nodes;
    } else {
    	U64 nodes = allMoves.allMovesCount();
//    	cout<<nodes<<endl;
    	if(useCache) {
    		CACHE::put(board.getKey() ^ ZOBRIST::getDepthKey(1), nodes);
    	}
    	return nodes;
    }

}


#endif
