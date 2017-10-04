#ifndef MOVES_GENERATOR_H_
#define MOVES_GENERATOR_H_

#include "legality.h"

template <bool sideToMove>
bool isKingSideCastlePossible(const ChessBoard &board) {
	if(board.allPieces() & shortCastleBlockers[sideToMove]) {
		return false;
	}

	if(!board.hasCastleRight<sideToMove>(KING_SIDE)) {
		return false;
	}

	if((rookShortCastleSqr[sideToMove] | kingShortCastleSqr[sideToMove]) &
			PAWNS::getPawnAttacks<!sideToMove>(board.piecesByType<!sideToMove>(PAWN))) {
		return false;
	}

	if(!isKingMoveLegal<sideToMove>(board, board.allPieces(), rookShortCastleSqr[sideToMove])) {
		return false;
	}

	if(!isKingMoveLegal<sideToMove>(board, board.allPieces(), kingShortCastleSqr[sideToMove])) {
		return false;
	}

	return true;
}

template <bool sideToMove>
bool isQueenSideCastlePossible(const ChessBoard &board) {
	if(board.allPieces() & longCastleBlockers[sideToMove]) {
		return false;
	}

	if(!board.hasCastleRight<sideToMove>(QUEEN_SIDE)) {
		return false;
	}

	if((rookLongCastleSqr[sideToMove] | kingLongCastleSqr[sideToMove]) &
			PAWNS::getPawnAttacks<!sideToMove>(board.piecesByType<!sideToMove>(PAWN))) {
		return false;
	}

	if(!isKingMoveLegal<sideToMove>(board, board.allPieces(), rookLongCastleSqr[sideToMove])) {
		return false;
	}

	if(!isKingMoveLegal<sideToMove>(board, board.allPieces(), kingLongCastleSqr[sideToMove])) {
		return false;
	}

	return true;
}

template <bool sideToMove>
inline void generateMovesAndCapturesForKing(const U64 attacks, const ChessBoard &board, AllMoves &moveList) {

	U64 kingToMove = board.piecesByType<sideToMove>(KING);

	U64 captures = attacks & board.piecesBySide<!sideToMove>();
	U64 legalMoves = attacks - (attacks & board.allPieces());

	while (captures) {
		U64 to = popFirstPieceMask(captures);

		Capture capture(KING, kingToMove, to);

		if(isKingMoveLegal<sideToMove>(board, capture)) {
			moveList.addCapture(capture);
		}
	}

//	if(check) {
//		legalMoves &= ~board.checkMap;
//	}

	while (legalMoves) {
		U64 to = popFirstPieceMask(legalMoves);
		Move move(KING, kingToMove, to);

		if(isKingMoveLegal<sideToMove>(board,  move)) {
			moveList.addMove(move);
		}
	}
}

template <bool sideToMove>
inline void generateMoves(const ChessBoard &board, const U64 sqrMask,
		const int piece, const U64 attacks, AllMoves &moveList) {

	U64 legalMoves = attacks - (attacks & board.allPieces());

	while(legalMoves) {
		U64 to = popFirstPieceMask(legalMoves);
		Move move(piece, sqrMask, to);
		moveList.addMove(move);
	}
}

template <bool sideToMove>
inline void generateCaptures(const ChessBoard &board, const U64 sqrMask,
		const int piece, const U64 attacks, AllMoves &moveList) {

	U64 captures = attacks & board.piecesBySide<!sideToMove>();

	while (captures) {
		U64 to = popFirstPieceMask(captures);
		Capture capture(piece, sqrMask, to);
		moveList.addCapture(capture);
	}
}

template <bool sideToMove, CHECK_T check, PIECE_T piece>
inline void iterateOverPieces(const ChessBoard& board, AllMoves& movesList) {
	U64 piecesToMove = board.piecesByType<sideToMove>(piece);
	while (piecesToMove) {
		SQUARE_T toMove = getFirstPieceSquare(piecesToMove);
		U64 sqrMask = popFirstPieceMask(piecesToMove);
		U64 attacks = MOVE_PROVIDER::getMoves<piece>(board.allPieces(), toMove);

		if(check) {
			attacks &= board.checkMap;
		}

		generateMoves<sideToMove>(board, sqrMask, piece, attacks, movesList);
		generateCaptures<sideToMove>(board, sqrMask, piece, attacks, movesList);
	}
}

template <bool sideToMove, CHECK_T check, PIECE_T piece>
inline void iterateOverNotPinnedPieces(const ChessBoard& board, AllMoves& movesList) {
	U64 piecesToMove = board.notPinnedPiecesByType<sideToMove, piece>();
	while (piecesToMove) {
		SQUARE_T toMove = getFirstPieceSquare(piecesToMove);
		U64 sqrMask = popFirstPieceMask(piecesToMove);
		U64 attacks = MOVE_PROVIDER::getMoves<piece>(board.allPieces(), toMove);

		if(check) {
			attacks &= board.checkMap;
		}

		generateMoves<sideToMove>(board, sqrMask, piece, attacks, movesList);
		generateCaptures<sideToMove>(board, sqrMask, piece, attacks, movesList);
	}
}

template <bool sideToMove, CHECK_T check, PIECE_T piece>
inline void iterateOverPinnedPieces(const ChessBoard& board, AllMoves& movesList) {
	U64 piecesToMove = board.pinnedPiecesByType<sideToMove, piece>();
	while (piecesToMove) {
		SQUARE_T toMove = getFirstPieceSquare(piecesToMove);
		U64 sqrMask = popFirstPieceMask(piecesToMove);
		U64 attacks = MOVE_PROVIDER::getMoves<piece>(board.allPieces(), toMove);


		if(check) {
			attacks &= board.checkMap;
		}
		attacks &= getDirection(toMove, board.piecesByType<sideToMove>(KING));

		generateCaptures<sideToMove>(board, sqrMask, piece, attacks, movesList);
		if(check == NO_CHECK) {
			generateMoves<sideToMove>(board, sqrMask, piece, attacks, movesList);
		}
	}
}

template <bool sideToMove, CHECK_T check>
inline void generateMoves(const ChessBoard& board, AllMoves& movesList) {

	if(board.getPinnedPieces()) {
		iterateOverNotPinnedPieces<sideToMove, check, QUEEN>(board, movesList);
		iterateOverNotPinnedPieces<sideToMove, check, BISHOP>(board, movesList);
		iterateOverNotPinnedPieces<sideToMove, check, ROOK>(board, movesList);
		iterateOverNotPinnedPieces<sideToMove, check, KNIGHT>(board, movesList);

		iterateOverPinnedPieces<sideToMove, check, QUEEN>(board, movesList);
		iterateOverPinnedPieces<sideToMove, check, BISHOP>(board, movesList);
		iterateOverPinnedPieces<sideToMove, check, ROOK>(board, movesList);
	} else {
		iterateOverPieces<sideToMove, check, QUEEN>(board, movesList);
		iterateOverPieces<sideToMove, check, BISHOP>(board, movesList);
		iterateOverPieces<sideToMove, check, ROOK>(board, movesList);
		iterateOverPieces<sideToMove, check, KNIGHT>(board, movesList);
	}
}


#endif
