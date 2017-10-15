#ifndef LEGALITY_H_
#define LEGALITY_H_

#include "bitboard.h"
#include "chessboard_utils.h"
#include "moves_provider.h"


inline U64 getDirection(const int from, const U64 king) {

	const U64 (*moves) = queenMoves[from];

	if((moves[NE] | moves[SW]) & king) {
		return moves[NE] | moves[SW];
	}
	if((moves[SE] | moves[NW]) & king) {
		return moves[SE] | moves[NW];
	}
	if((moves[N] | moves[S]) & king) {
		return moves[N] | moves[S];
	}
	if((moves[E] | moves[W]) & king) {
		return moves[E] | moves[W];
	}

	return 0;
}

template <bool sideToMove>
inline bool isKingMoveLegal(const ChessBoard& board, const U64 allPieces, const U64 king) {

	U64 queenAndRook = board.piecesByType<!sideToMove>(ROOK) | board.piecesByType<!sideToMove>(QUEEN);
	U64 queenAndBishop = board.piecesByType<!sideToMove>(BISHOP) | board.piecesByType<!sideToMove>(QUEEN);
	U64 knights = board.piecesByType<!sideToMove>(KNIGHT);

	SQUARE_T sqr = getFirstPieceSquare(king);

	if (MOVE_PROVIDER::getMoves<KNIGHT>(allPieces, sqr) & knights) {
		return false;
	}

	if(MOVE_PROVIDER::getMoves<BISHOP>(allPieces, sqr) & queenAndBishop) {
		return false;
	}

	if(MOVE_PROVIDER::getMoves<ROOK>(allPieces, sqr) & queenAndRook) {
		return false;
	}

	if (kingMoves[sqr] & board.piecesByType<!sideToMove>(KING)) {
		return false;
	}

	return true;
}


template <bool sideToMove>
inline bool isEnPassantMoveLegal(const ChessBoard& board, const Capture& move) {

	U64 allPieces = board.allPieces();
	allPieces -= move.maskFrom;
	allPieces -= move.capturedPieceSquare;
	allPieces += move.maskTo;

	SQUARE_T sqr = getFirstPieceSquare(board.piecesByType<sideToMove>(KING));

	U64 queenAndBishop = board.piecesByType<!sideToMove>(BISHOP) | board.piecesByType<!sideToMove>(QUEEN);
	if(MOVE_PROVIDER::getMoves<BISHOP>(allPieces, sqr) & queenAndBishop) {
		return false;
	}

	U64 queenAndRook = board.piecesByType<!sideToMove>(ROOK) | board.piecesByType<!sideToMove>(QUEEN);
	if(MOVE_PROVIDER::getMoves<ROOK>(allPieces, sqr) & queenAndRook) {
		return false;
	}

	return true;
}

template <bool sideToMove>
inline bool isKingMoveLegal(const ChessBoard& board, const Move& move) {

	U64 allPieces = board.allPieces();
	allPieces -= move.maskFrom;

	return isKingMoveLegal<sideToMove>(board, allPieces, move.maskTo);
}

inline void getChecksOnPositiveDirection(ChessBoard &board, const U64 movesOnDir, const U64 attackers, U64 &pinnedPieces) {

	if(movesOnDir & attackers) {
		U64 potentialBlockers = movesOnDir & board.allPieces();
		U64 blocker = getFirstPieceMask(potentialBlockers);
		if(blocker & attackers) {
			U64 allowedSquares = blocker - 1;
			U64 legalMoves = movesOnDir & allowedSquares;

			board.checkMap |= legalMoves;
			board.checkMap |= blocker;
		} else {
			potentialBlockers-=blocker;
			U64 nextBlocker = getFirstPieceMask(potentialBlockers);
			if(nextBlocker & attackers) {
				U64 allowedSquares = nextBlocker - 1;
				U64 legalMoves = movesOnDir & allowedSquares;

				pinnedPieces |= legalMoves;
				pinnedPieces |= nextBlocker;
			}
		}
	}
};

inline void getChecksOnNegativeDirection(ChessBoard &board, const U64 movesOnDir, const U64 attackers, U64 &pinnedPieces) {

	if(movesOnDir & attackers)
	{
		U64 potentialBlockers = movesOnDir & board.allPieces();
		U64 blocker = getLastPieceMask(potentialBlockers);
		if(blocker & attackers) {
			U64 allowedSquares = ~(blocker - 1);
			U64 legalMoves = movesOnDir & allowedSquares;

			board.checkMap |= legalMoves;
		}
		else {
			potentialBlockers-=blocker;
			U64 nextBlocker = getLastPieceMask(potentialBlockers);
			if(nextBlocker & attackers) {
				U64 allowedSquares = ~(nextBlocker - 1);
				U64 legalMoves = movesOnDir & allowedSquares;
				pinnedPieces |= legalMoves;
			}
		}
	}
};


template <bool sideToMove>
inline U64 setCheckAndPinners(ChessBoard& board) {

	U64 king = board.piecesByType<!sideToMove>(KING);
	SQUARE_T sqr = getFirstPieceSquare(king);


	const U64 (*moves) = queenMoves[sqr];


	U64 queenAndRook = board.piecesByType<sideToMove>(ROOK) | board.piecesByType<sideToMove>(QUEEN);
	U64 queenAndBishop = board.piecesByType<sideToMove>(BISHOP) | board.piecesByType<sideToMove>(QUEEN);

	getChecksOnPositiveDirection(board, moves[NW], queenAndBishop, board.pinnedToBishop);
	getChecksOnPositiveDirection(board, moves[NE], queenAndBishop, board.pinnedToBishop);
	getChecksOnPositiveDirection(board, moves[E], queenAndRook, board.pinnedToRook);

	getChecksOnNegativeDirection(board, moves[SE], queenAndBishop, board.pinnedToBishop);
	getChecksOnNegativeDirection(board, moves[SW], queenAndBishop, board.pinnedToBishop);
	getChecksOnNegativeDirection(board, moves[W], queenAndRook, board.pinnedToRook);

	board.pinnedPawns = board.getPinnedPieces();

	getChecksOnNegativeDirection(board, moves[S], queenAndRook, board.pinnedToRook);
	getChecksOnPositiveDirection(board, moves[N], queenAndRook, board.pinnedToRook);

	if(MOVE_PROVIDER::getMoves<KNIGHT>(0, sqr) & board.piecesByType<sideToMove>(KNIGHT)) {
		board.checkMap += MOVE_PROVIDER::getMoves<KNIGHT>(0, sqr) & board.piecesByType<sideToMove>(KNIGHT);
	}

	U64 pawnsToMove = board.piecesByType<sideToMove>(PAWN);
	U64 pawnAttacks = PAWNS::getPawnAttacks<sideToMove>(pawnsToMove);

	U64 allKingMoves = exclude(kingMoves[sqr], pawnAttacks);

	if (pawnAttacks & king) {
		board.checkMap += PAWNS::getPawnAttacks<!sideToMove>(pawnAttacks & king) & pawnsToMove;
	}

	return allKingMoves;
}

#endif
