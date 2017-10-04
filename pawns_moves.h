#ifndef MOVE_GENERATOR_H
#define MOVE_GENERATOR_H

#include <iostream>
#include "chessboard.h"
#include "move.h"

using namespace std;

template <bool sideToMove>
inline void generatePawnMoves(const ChessBoard &board, const U64 pawns, AllMoves &moveList);
template <bool sideToMove>
inline void generateFirstRankPawnMoves(const ChessBoard &board, const U64 pawns, AllMoves &moveList);
template <bool sideToMove>
inline void generatePawnPromotions(const ChessBoard &board, const U64 pawns, AllMoves &moveList);
template <bool sideToMove>
inline void generatePawnCaptures(const ChessBoard &board, const U64 pawns, AllMoves &moveList);

template <bool sideToMove>
inline U64 getFirstPawnRankMask();

template <>
inline U64 getFirstPawnRankMask<BLACK> () {
	return BLACK_FIRST_RANK_PAWNS;
}

template <>
inline U64 getFirstPawnRankMask<WHITE> () {
	return WHITE_FIRST_RANK_PAWNS;
}

template <bool sideToMove>
inline U64 getPromotionRankMask();

template <>
inline U64 getPromotionRankMask<BLACK> () {
	return BLACK_PROMOTION_RANK_PAWNS;
}

template <>
inline U64 getPromotionRankMask<WHITE> () {
	return WHITE_PROMOTION_RANK_PAWNS;
}

template <bool sideToMove>
void generatePawnsMoves(const ChessBoard &board, AllMoves &moveList) {

	
	U64 firstRankMask = getFirstPawnRankMask<sideToMove>();
	U64 promotionRankMask = getPromotionRankMask<sideToMove>();
	
	U64 pawnsToMove = board.piecesByType<sideToMove>(PAWN);
	U64 firstRankPawns = pawnsToMove & firstRankMask;
	U64 promotionPawns = pawnsToMove & promotionRankMask;

	if(promotionPawns) {
		generatePawnPromotions<sideToMove>(board, promotionPawns, moveList);
		pawnsToMove -= promotionPawns;
	}

	generatePawnCaptures<sideToMove>(board, pawnsToMove, moveList);


	generateFirstRankPawnMoves<sideToMove>(board, firstRankPawns, moveList);
	pawnsToMove -= firstRankPawns;
	generatePawnMoves<sideToMove>(board, pawnsToMove, moveList);

}


template <bool sideToMove>
void generatePawnMoves(const ChessBoard &board, const U64 pawns, AllMoves &moveList) {
	U64 move;

	U64 pawnsMoves = moveForward<sideToMove>(pawns, 8) & ~board.allPieces();
	while(pawnsMoves) {
		move = popFirstPiece3(pawnsMoves);
		U64 from = moveBackward<sideToMove>(move, 8);
		moveList.addMove(Move(PAWN, from, move));
	}
}


template <bool sideToMove>
void generateFirstRankPawnMoves(const ChessBoard &board, const U64 pawns, AllMoves &moveList) {
	U64 move;

	U64 pawnMoves = moveForward<sideToMove>(pawns, 8) & ~board.allPieces();
	//U64 pawnMovesTwoSqr = moveForward(pawns, 8, board.sideToMove) & ~board.allPieces;

	U64 moveByTwoSqr;// = moveForward(pawnMoves, 8, board.sideToMove) & ~board.allPieces;
	while(pawnMoves) {
		move = popFirstPiece3(pawnMoves);
		U64 from = moveBackward<sideToMove>(move, 8);
		moveList.addMove(Move(PAWN, from, move));
		moveByTwoSqr = moveForward<sideToMove>(move, 8);
		if(moveByTwoSqr & ~board.allPieces()) {
			moveList.addMove(Move(PAWN, from, moveByTwoSqr, move));
		}
	}

}

template <bool sideToMove>
void generatePawnCaptures(const ChessBoard &board, const U64 pawns, AllMoves &moveList) {

	U64 toCapture = board.piecesBySide<!sideToMove>();
	U64 move;

	U64 pawnsToMove = getPawnAttacksLeft<sideToMove>(pawns) & toCapture;
	while(pawnsToMove) {
		move = popFirstPiece3(pawnsToMove);
		U64 from = getPawnBackwardMoveLeft<sideToMove>(move);

		PIECE_T capturedPiece = board.getPieceOnSquare<!sideToMove>(move);
		moveList.addCapture(Capture(PAWN, from, move, capturedPiece));
	}

	pawnsToMove = getPawnAttacksRight<sideToMove>(pawns) & toCapture;
	while(pawnsToMove) {
		move = popFirstPiece3(pawnsToMove);
		U64 from = getPawnBackwardMoveRight<sideToMove>(move);

		PIECE_T capturedPiece = board.getPieceOnSquare<!sideToMove>(move);
		moveList.addCapture(Capture(PAWN, from, move, capturedPiece));
	}

	U64 pawnToDoEnPessant = getPawnAttacksRight<!sideToMove>(board.enPessantSqr) & pawns;

	if(pawnToDoEnPessant) {
		moveList.addCapture(
				Capture(PAWN, pawnToDoEnPessant, board.enPessantSqr, moveForward<!sideToMove>(board.enPessantSqr, 8) ,PAWN));
	}

	pawnToDoEnPessant = getPawnAttacksLeft<!sideToMove>(board.enPessantSqr) & pawns;
	if(pawnToDoEnPessant) {
		moveList.addCapture(
				Capture(PAWN, pawnToDoEnPessant, board.enPessantSqr, moveForward<!sideToMove>(board.enPessantSqr, 8), PAWN));
	}
}

template <bool sideToMove>
void generatePawnPromotions(const ChessBoard &board, const U64 pawns, AllMoves &moveList) {

	U64 move;
	U64 pawnsToMove = moveForward<sideToMove>(pawns, 8) & ~board.allPieces();
	while(pawnsToMove) {
		move = popFirstPiece3(pawnsToMove);
		U64 from = moveBackward<sideToMove>(move, 8);

		moveList.addPromotion(Promotion(PAWN, from, move, QUEEN));
		moveList.addPromotion(Promotion(PAWN, from, move, ROOK));
		moveList.addPromotion(Promotion(PAWN, from, move, KNIGHT));
		moveList.addPromotion(Promotion(PAWN, from, move, BISHOP));
	}

	pawnsToMove = getPawnAttacksLeft<sideToMove>(pawns) & board.piecesBySide<!sideToMove>();

	while(pawnsToMove) {
		move = popFirstPiece3(pawnsToMove);
		U64 from = getPawnBackwardMoveLeft<sideToMove>(move);

		PIECE_T capturedPiece = board.getPieceOnSquare<!sideToMove>(move);

		moveList.addPromotionCapture(PromotionCapture(PAWN, QUEEN, from, move, capturedPiece));
		moveList.addPromotionCapture(PromotionCapture(PAWN, ROOK, from, move, capturedPiece));
		moveList.addPromotionCapture(PromotionCapture(PAWN, KNIGHT, from, move, capturedPiece));
		moveList.addPromotionCapture(PromotionCapture(PAWN, BISHOP, from, move, capturedPiece));
	}

	pawnsToMove = getPawnAttacksRight<sideToMove>(pawns) & board.piecesBySide<!sideToMove>();

	while(pawnsToMove) {
		move = popFirstPiece3(pawnsToMove);
		U64 from = getPawnBackwardMoveRight<sideToMove>(move);

		PIECE_T capturedPiece = board.getPieceOnSquare<!sideToMove>(move);

		moveList.addPromotionCapture(PromotionCapture(PAWN, QUEEN, from, move, capturedPiece));
		moveList.addPromotionCapture(PromotionCapture(PAWN, ROOK, from, move, capturedPiece));
		moveList.addPromotionCapture(PromotionCapture(PAWN, KNIGHT, from, move, capturedPiece));
		moveList.addPromotionCapture(PromotionCapture(PAWN, BISHOP, from, move, capturedPiece));
	}
}




#endif
