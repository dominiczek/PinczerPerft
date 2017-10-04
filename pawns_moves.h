#ifndef PAWNS_H_
#define PAWNS_H_

#include <iostream>
#include "chessboard.h"
#include "legality.h"
#include "moves.h"

template <bool sideToMove>
inline void generatePawnMoves(const ChessBoard &board, const U64 pawns, AllMoves &moveList);
template <bool sideToMove>
inline void generateFirstRankPawnMoves(const ChessBoard &board, const U64 pawns, AllMoves &moveList);
template <bool sideToMove>
inline void generatePawnPromotions(const ChessBoard &board, const U64 pawns, AllMoves &moveList);
template <bool sideToMove>
inline void generatePawnCaptures(const ChessBoard &board, const U64 pawns, AllMoves &moveList);


template <bool sideToMove, CHECK_T check>
void generatePawnsMoves(const ChessBoard &board, AllMoves &moveList) {

	U64 firstRankMask = PAWNS::getFirstPawnRankMask<sideToMove>();
	U64 promotionRankMask = PAWNS::getPromotionRankMask<sideToMove>();
	
	U64 pawnsToMove = board.piecesByType<sideToMove>(PAWN);
	U64 firstRankPawns = pawnsToMove & firstRankMask;
	U64 promotionPawns = pawnsToMove & promotionRankMask;

	if(promotionPawns) {
		generatePawnPromotions<sideToMove, check>(board, promotionPawns, moveList);
		pawnsToMove -= promotionPawns;
	}


	generatePawnCaptures<sideToMove, check>(board, pawnsToMove, moveList);



	generateFirstRankPawnMoves<sideToMove, check>(board, firstRankPawns, moveList);

	pawnsToMove -= firstRankPawns;
	generatePawnMoves<sideToMove, check>(board, pawnsToMove, moveList);
}


template <bool sideToMove, CHECK_T isCheck>
void generatePawnMoves(const ChessBoard &board,  const U64 pawns, AllMoves &moveList) {

	U64 notPinedPawns = pawns & ~board.pinnedPawns;
	U64 legalPawnMoves = moveForward<sideToMove>(notPinedPawns, 8) & ~board.allPieces();

	if(isCheck == CHECK){
		legalPawnMoves &= board.checkMap;
	}

	while(legalPawnMoves) {
		U64 move = popFirstPieceMask(legalPawnMoves);
		U64 from = moveBackward<sideToMove>(move, 8);
		Move m(PAWN, from, move);
		moveList.addMove(m);
	}
}


template <bool sideToMove>
void generateFirstRankPawnMovesCheck(const ChessBoard &board,  const U64 pawns, AllMoves &moveList) {

	U64 legalPawnMoves = moveForward<sideToMove>(pawns, 8) & ~board.allPieces();
	U64 legalTowSquareMoves = moveForward<sideToMove>(legalPawnMoves, 8) & ~board.allPieces();

	legalPawnMoves &= board.checkMap;
	legalTowSquareMoves &= board.checkMap;



	while(legalPawnMoves) {
		U64 move = popFirstPieceMask(legalPawnMoves);
		U64 from = moveBackward<sideToMove>(move, 8);

		Move m(PAWN, from, move);
		moveList.addMove(m);
	}

	U64 enPassantPawns = board.piecesByType<!sideToMove>(PAWN)& EN_PASSANT_RANK[!sideToMove];
	U64 enPassantAttacks = PAWNS::getPawnAttacks<!sideToMove>(enPassantPawns);


	while(legalTowSquareMoves) {
		U64 move = popFirstPieceMask(legalTowSquareMoves);
		U64 enPassant = moveBackward<sideToMove>(move, 8);
		U64 from = moveBackward<sideToMove>(enPassant, 8);

		PawnMove m(PAWN, from, move, (enPassant & enPassantAttacks)? enPassant:0);

		moveList.addMove(m);
	}
}

template <bool sideToMove, CHECK_T check>
void generateFirstRankPawnMoves(const ChessBoard &board,  const U64 pawns, AllMoves &moveList) {

	U64 notPinedPawns = pawns & (~board.pinnedPawns);

	if(check == CHECK) {
		generateFirstRankPawnMovesCheck<sideToMove>(board, notPinedPawns, moveList);
	} else {

		U64 enPassantPawns = board.piecesByType<!sideToMove>(PAWN)& EN_PASSANT_RANK[!sideToMove];

		U64 enPassantAttacks = PAWNS::getPawnAttacks<!sideToMove>(enPassantPawns);

		U64 legalPawnMoves = moveForward<sideToMove>(notPinedPawns, 8) & ~board.allPieces();

		while(legalPawnMoves) {
			U64 move = popFirstPieceMask(legalPawnMoves);
			U64 from = moveBackward<sideToMove>(move, 8);

			Move m(PAWN, from, move);

			moveList.addMove(m);

			U64 moveByTwoSqr = moveForward<sideToMove>(move, 8);
			if(moveByTwoSqr & ~board.allPieces()) {

				PawnMove m2(PAWN, from, moveByTwoSqr, (move & enPassantAttacks)? move:0);
				moveList.addMove(m2);
			}
		}
	}


}

template <bool sideToMove, CHECK_T check>
void generatePawnCaptures(const ChessBoard &board,  const U64 pawns, AllMoves &moveList) {

	U64 pawnAttacksL = PAWNS::getPawnAttacksLeft<sideToMove>(pawns);
	U64 pawnAttacksR = PAWNS::getPawnAttacksRight<sideToMove>(pawns);

	if(pawnAttacksL & board.enPessantSqr) {
	    U64 pawnToDoEnPessant = PAWNS::getPawnAttacksRight<!sideToMove>(board.enPessantSqr);
	    PawnCapture capture(PAWN, pawnToDoEnPessant, board.enPessantSqr, moveForward<!sideToMove>(board.enPessantSqr, 8));

		if(isEnPassantMoveLegal<sideToMove>(board, capture)) {
			moveList.addCapture(capture);
		}
	}

	if(pawnAttacksR & board.enPessantSqr) {
		U64 pawnToDoEnPessant = PAWNS::getPawnAttacksLeft<!sideToMove>(board.enPessantSqr);
		PawnCapture capture(PAWN, pawnToDoEnPessant, board.enPessantSqr, moveForward<!sideToMove>(board.enPessantSqr, 8));

		if(isEnPassantMoveLegal<sideToMove>(board, capture)) {
			moveList.addCapture(capture);
		}
	}

	U64 toCapture = board.piecesBySide<!sideToMove>();

	if(check == CHECK) {
		toCapture &= board.checkMap;
	}

	U64 pawnsToMoveL = pawnAttacksL & toCapture;
	U64 pawnsToMoveR = pawnAttacksR & toCapture;


	while(pawnsToMoveL) {
		U64 move = popFirstPieceMask(pawnsToMoveL);
		U64 from = PAWNS::getPawnBackwardMoveLeft<sideToMove>(move);

		if(from & board.getPinnedPieces()) {
			if(move & board.pinnedPiecesB) {
				Capture capture(PAWN, from, move);
				moveList.addCapture(capture);
			}
		} else {
			Capture capture(PAWN, from, move);
			moveList.addCapture(capture);
		}
	}

	while(pawnsToMoveR) {
		U64 move = popFirstPieceMask(pawnsToMoveR);
		U64 from = PAWNS::getPawnBackwardMoveRight<sideToMove>(move);

		if(from & board.getPinnedPieces()) {
			if(move & board.pinnedPiecesB) {
				Capture capture(PAWN, from, move);
				moveList.addCapture(capture);
			}
		} else {
			Capture capture(PAWN, from, move);
			moveList.addCapture(capture);
		}
	}
}



template <bool sideToMove, CHECK_T check>
void generatePawnPromotions(const ChessBoard &board,  const U64 pawns, AllMoves &moveList) {

	U64 pawnsToMove = moveForward<sideToMove>(pawns & ~board.getPinnedPieces(), 8) & ~board.allPieces();

	if(check) {
		pawnsToMove &= board.checkMap;
	}

	while(pawnsToMove) {
		U64 move = popFirstPieceMask(pawnsToMove);
		U64 from = moveBackward<sideToMove>(move, 8);

		moveList.addPromotion(Promotion(from, move, QUEEN));
		moveList.addPromotion(Promotion(from, move, ROOK));
		moveList.addPromotion(Promotion(from, move, KNIGHT));
		moveList.addPromotion(Promotion(from, move, BISHOP));
	}

	U64 toCapture = board.piecesBySide<!sideToMove>();

	if(check == CHECK) {
		toCapture &= board.checkMap;
	}


	U64 pawnAttacksL = PAWNS::getPawnAttacksLeft<sideToMove>(pawns);
	U64 pawnAttacksR = PAWNS::getPawnAttacksRight<sideToMove>(pawns);

	U64 pawnsToMoveL = pawnAttacksL & toCapture;
	U64 pawnsToMoveR = pawnAttacksR & toCapture;


	while(pawnsToMoveL) {
		U64 move = popFirstPieceMask(pawnsToMoveL);
		U64 from = PAWNS::getPawnBackwardMoveLeft<sideToMove>(move);

		if(from & board.getPinnedPieces()) {
			if(isPinnedCaptureLegal(board.piecesByType<sideToMove>(KING), from, move)) {
				moveList.addPromotionCapture(PromotionCapture(from, move, QUEEN));
				moveList.addPromotionCapture(PromotionCapture(from, move, ROOK));
				moveList.addPromotionCapture(PromotionCapture(from, move, KNIGHT));
				moveList.addPromotionCapture(PromotionCapture(from, move, BISHOP));
			}
		} else {
			moveList.addPromotionCapture(PromotionCapture(from, move, QUEEN));
			moveList.addPromotionCapture(PromotionCapture(from, move, ROOK));
			moveList.addPromotionCapture(PromotionCapture(from, move, KNIGHT));
			moveList.addPromotionCapture(PromotionCapture(from, move, BISHOP));
		}
	}

	while(pawnsToMoveR) {
		U64 move = popFirstPieceMask(pawnsToMoveR);
		U64 from = PAWNS::getPawnBackwardMoveRight<sideToMove>(move);

		if(from & board.getPinnedPieces()) {
			if(isPinnedCaptureLegal(board.piecesByType<sideToMove>(KING), from, move)) {
				moveList.addPromotionCapture(PromotionCapture(from, move, QUEEN));
				moveList.addPromotionCapture(PromotionCapture(from, move, ROOK));
				moveList.addPromotionCapture(PromotionCapture(from, move, KNIGHT));
				moveList.addPromotionCapture(PromotionCapture(from, move, BISHOP));
			}
		} else {
			PromotionCapture promotionCapture(from, move, QUEEN);
			moveList.addPromotionCapture(promotionCapture);
			moveList.addPromotionCapture(PromotionCapture(from, move, ROOK));
			moveList.addPromotionCapture(PromotionCapture(from, move, KNIGHT));
			moveList.addPromotionCapture(PromotionCapture(from, move, BISHOP));
		}
	}
}


#endif
