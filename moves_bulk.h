#ifndef MOVES_BULK_H_
#define MOVES_BULK_H_

#include "legality.h"

template <bool sideToMove>
inline int countMovesAndCapturesForKing(const U64 attacks, const ChessBoard &board) {

	int result = 0;

	U64 kingToMove = board.piecesByType<sideToMove>(KING);

	U64 legalMoves = exclude(attacks, board.piecesBySide<sideToMove>());

	U64 allPieces = board.allPieces() - kingToMove;

	while (legalMoves) {
		U64 to = popFirstPieceMask(legalMoves);

		if(isKingMoveLegal<sideToMove>(board,  allPieces, to)) {
			result++;
		}
	}

	return result;
}

template <bool sideToMove>
inline int countMoves(const ChessBoard &board, const U64 attacks) {
	U64 legalMoves = exclude(attacks, board.allPieces());
	return popCount(legalMoves);
}

template <bool sideToMove>
inline int countCaptures(const ChessBoard &board, const U64 attacks) {
	U64 captures = attacks & board.piecesBySide<!sideToMove>();
	return popCount(captures);
}

template <bool sideToMove>
inline int countAllMoves(const ChessBoard &board, const U64 attacks) {
	U64 allMoves = exclude(attacks, board.piecesBySide<sideToMove>());
	return popCount(allMoves);
}

template <bool sideToMove, CHECK_T check, PIECE_T piece>
inline int countForPieces(const ChessBoard& board) {
	int result = 0;
	U64 piecesToMove = board.piecesByType<sideToMove>(piece);
	while (piecesToMove) {
		SQUARE_T toMove = getFirstPieceSquare(piecesToMove);
		piecesToMove &= piecesToMove - 1;
		U64 attacks = MOVE_PROVIDER::getMoves<piece>(board.allPieces(), toMove);

		if(check) {
			attacks &= board.checkMap;
		}

		result += countAllMoves<sideToMove>(board, attacks);
	}

	return result;
}

template <bool sideToMove, CHECK_T check, PIECE_T piece>
inline int countForNotPinnedPieces(const ChessBoard& board) {
	int result = 0;
	U64 piecesToMove = board.notPinnedPiecesByType<sideToMove, piece>();
	while (piecesToMove) {
		SQUARE_T toMove = getFirstPieceSquare(piecesToMove);
		piecesToMove &= piecesToMove - 1;
		U64 attacks = MOVE_PROVIDER::getMoves<piece>(board.allPieces(), toMove);

		if(check) {
			attacks &= board.checkMap;
		}

		result += countAllMoves<sideToMove>(board, attacks);
	}

	return result;
}

template <bool sideToMove, CHECK_T check, PIECE_T piece>
inline int countForPinnedPieces(const ChessBoard& board) {
	int result = 0;
	U64 piecesToMove = board.pinnedPiecesByType<sideToMove, piece>();
	while (piecesToMove) {
		SQUARE_T toMove = getFirstPieceSquare(piecesToMove);
		piecesToMove &= piecesToMove -1;
		U64 attacks = MOVE_PROVIDER::getMoves<piece>(board.allPieces(), toMove);
		attacks &= getDirection(toMove, board.piecesByType<sideToMove>(KING));
		if(check) {
			attacks &= board.checkMap;
		}

		result += countCaptures<sideToMove>(board, attacks);
		if(check == NO_CHECK) {
			result += countMoves<sideToMove>(board, attacks);
		}
	}

	return result;
}

template <bool sideToMove, CHECK_T check>
inline int countMoves(const ChessBoard& board) {

	int result = 0;

	if(board.getPinnedPieces()) {
		result += countForNotPinnedPieces<sideToMove, check, QUEEN>(board);
		result += countForNotPinnedPieces<sideToMove, check, BISHOP>(board);
		result += countForNotPinnedPieces<sideToMove, check, ROOK>(board);
		result += countForNotPinnedPieces<sideToMove, check, KNIGHT>(board);

		result += countForPinnedPieces<sideToMove, check, QUEEN>(board);
		result += countForPinnedPieces<sideToMove, check, BISHOP>(board);
		result += countForPinnedPieces<sideToMove, check, ROOK>(board);
	} else {
		result += countForPieces<sideToMove, check, QUEEN>(board);
		result += countForPieces<sideToMove, check, BISHOP>(board);
		result += countForPieces<sideToMove, check, ROOK>(board);
		result += countForPieces<sideToMove, check, KNIGHT>(board);
	}

	return result;
}


#endif
