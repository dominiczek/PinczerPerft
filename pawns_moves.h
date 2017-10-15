#ifndef PAWNS_H_
#define PAWNS_H_

#include <iostream>
#include "chessboard.h"
#include "legality.h"
#include "moves.h"

template <bool sideToMove, CHECK_T isCheck>
inline void generatePawnMoves(const ChessBoard &board,  const U64 pawns, AllMoves &moveList) {

	U64 notPinedPawns = exclude(pawns , board.pinnedPawns);
	U64 legalPawnMoves = exclude(moveForward<sideToMove>(notPinedPawns, 8), board.allPieces());

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
inline void generateFirstRankPawnMovesCheck(const ChessBoard &board,  const U64 pawns, AllMoves &moveList) {

	U64 legalPawnMoves = exclude(moveForward<sideToMove>(pawns, 8), board.allPieces());
	U64 legalTowSquareMoves = exclude(moveForward<sideToMove>(legalPawnMoves, 8), board.allPieces());

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

		PawnMove m(PAWN, from, move, (enPassant & enPassantAttacks));

		moveList.addMove(m);
	}
}

template <bool sideToMove, CHECK_T check>
inline void generateFirstRankPawnMoves(const ChessBoard &board, const U64 pawns, AllMoves &moveList) {

	U64 notPinedPawns = pawns & (~board.pinnedPawns);

	if(check == CHECK) {
		generateFirstRankPawnMovesCheck<sideToMove>(board, notPinedPawns, moveList);
	} else {
		U64 enPassantPawns = board.piecesByType<!sideToMove>(PAWN) & EN_PASSANT_RANK[!sideToMove];
		U64 enPassantAttacks = PAWNS::getPawnAttacks<!sideToMove>(enPassantPawns);

		U64 legalPawnMoves = exclude(moveForward<sideToMove>(notPinedPawns, 8), board.allPieces());

		while(legalPawnMoves) {
			U64 move = popFirstPieceMask(legalPawnMoves);
			U64 from = moveBackward<sideToMove>(move, 8);

			Move m(PAWN, from, move);
			moveList.addMove(m);

			U64 moveByTwoSqr = moveForward<sideToMove>(move, 8);
			//TODO: change!
			if(!(moveByTwoSqr & board.allPieces())) {
				PawnMove m2(PAWN, from, moveByTwoSqr, (move & enPassantAttacks));
				moveList.addMove(m2);
			}
		}
	}


}

template <bool sideToMove>
inline void generateEnPassant(const ChessBoard &board, const U64 pawns, AllMoves &moveList) {

	if(board.enPessantSqr) {
		U64 pawnsToDoEnPessant = PAWNS::getPawnAttacks<!sideToMove>(board.enPessantSqr) & pawns;

		if(pawnsToDoEnPessant) {
			U64 pawn = popFirstPieceMask(pawnsToDoEnPessant);
			PawnCapture capture(PAWN, pawn, board.enPessantSqr, moveForward<!sideToMove>(board.enPessantSqr, 8));
			if(isEnPassantMoveLegal<sideToMove>(board, capture)) {
				moveList.addCapture(capture);
			}
			if(pawnsToDoEnPessant) {

				PawnCapture capture(PAWN, pawnsToDoEnPessant, board.enPessantSqr, moveForward<!sideToMove>(board.enPessantSqr, 8));
				if(isEnPassantMoveLegal<sideToMove>(board, capture)) {
					moveList.addCapture(capture);
				}
			}
		}

	}
}

template <bool sideToMove, CHECK_T check>
inline void generatePawnCaptures(const ChessBoard &board, const U64 pawns, AllMoves &moveList) {

	U64 pawnAttacksL = PAWNS::getPawnAttacksLeft<sideToMove>(pawns);
	U64 pawnAttacksR = PAWNS::getPawnAttacksRight<sideToMove>(pawns);

	U64 toCapture = board.piecesBySide<!sideToMove>();

	if(check == CHECK) {
		toCapture &= board.checkMap;
	}

	U64 pawnsToMoveL = pawnAttacksL & toCapture;
	U64 pawnsToMoveR = pawnAttacksR & toCapture;


	while(pawnsToMoveL) {
		U64 move = popFirstPieceMask(pawnsToMoveL);
		U64 from = PAWNS::getPawnBackwardMoveLeft<sideToMove>(move);

		if(from & board.pinnedToBishop) {
			if(move & board.pinnedToBishop) {
				PIECE_T captured = board.getPieceOnSquare<!sideToMove>(move);
				Capture capture(PAWN, from, move, captured);
				moveList.addCapture(capture);
			}
		} else {

			PIECE_T captured = board.getPieceOnSquare<!sideToMove>(move);
			Capture capture(PAWN, from, move, captured);
			moveList.addCapture(capture);
		}
	}

	while(pawnsToMoveR) {
		U64 move = popFirstPieceMask(pawnsToMoveR);
		U64 from = PAWNS::getPawnBackwardMoveRight<sideToMove>(move);

		if(from & board.pinnedToBishop) {
			if(move & board.pinnedToBishop) {
				PIECE_T captured = board.getPieceOnSquare<!sideToMove>(move);
				Capture capture(PAWN, from, move, captured);
				moveList.addCapture(capture);
			}
		} else {
			PIECE_T captured = board.getPieceOnSquare<!sideToMove>(move);
			Capture capture(PAWN, from, move, captured);
			moveList.addCapture(capture);
		}
	}
}



template <bool sideToMove, CHECK_T check>
inline void generatePawnPromotions(const ChessBoard &board,  const U64 pawns, AllMoves &moveList) {

	U64 pawnsToMove = exclude(moveForward<sideToMove>( exclude(pawns, board.getPinnedPieces()), 8), board.allPieces());

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
			if(move & board.pinnedToBishop) {
				PIECE_T captured = board.getPieceOnSquare<!sideToMove>(move);

				moveList.addPromotionCapture(PromotionCapture(from, move, QUEEN, captured));
				moveList.addPromotionCapture(PromotionCapture(from, move, ROOK, captured));
				moveList.addPromotionCapture(PromotionCapture(from, move, KNIGHT, captured));
				moveList.addPromotionCapture(PromotionCapture(from, move, BISHOP, captured));
			}
		} else {
			PIECE_T captured = board.getPieceOnSquare<!sideToMove>(move);

			moveList.addPromotionCapture(PromotionCapture(from, move, QUEEN, captured));
			moveList.addPromotionCapture(PromotionCapture(from, move, ROOK, captured));
			moveList.addPromotionCapture(PromotionCapture(from, move, KNIGHT, captured));
			moveList.addPromotionCapture(PromotionCapture(from, move, BISHOP, captured));
		}
	}

	while(pawnsToMoveR) {
		U64 move = popFirstPieceMask(pawnsToMoveR);
		U64 from = PAWNS::getPawnBackwardMoveRight<sideToMove>(move);

		if(from & board.getPinnedPieces()) {
			if(move & board.pinnedToBishop) {
				PIECE_T captured = board.getPieceOnSquare<!sideToMove>(move);

				moveList.addPromotionCapture(PromotionCapture(from, move, QUEEN, captured));
				moveList.addPromotionCapture(PromotionCapture(from, move, ROOK, captured));
				moveList.addPromotionCapture(PromotionCapture(from, move, KNIGHT, captured));
				moveList.addPromotionCapture(PromotionCapture(from, move, BISHOP, captured));
			}
		} else {
			PIECE_T captured = board.getPieceOnSquare<!sideToMove>(move);

			moveList.addPromotionCapture(PromotionCapture(from, move, QUEEN, captured));
			moveList.addPromotionCapture(PromotionCapture(from, move, ROOK, captured));
			moveList.addPromotionCapture(PromotionCapture(from, move, KNIGHT, captured));
			moveList.addPromotionCapture(PromotionCapture(from, move, BISHOP, captured));
		}
	}
}

template <bool sideToMove, CHECK_T check>
inline void generatePawnsMoves(const ChessBoard &board, AllMoves &moveList) {

	U64 firstRankMask = PAWNS::getFirstPawnRankMask<sideToMove>();
	U64 promotionRankMask = PAWNS::getPromotionRankMask<sideToMove>();

	U64 pawnsToMove = board.piecesByType<sideToMove>(PAWN);
	U64 firstRankPawns = pawnsToMove & firstRankMask;
	U64 promotionPawns = pawnsToMove & promotionRankMask;

	if(promotionPawns) {
		generatePawnPromotions<sideToMove, check>(board, promotionPawns, moveList);
		pawnsToMove -= promotionPawns;
	}

	U64 pawnsToMakeCapture = exclude(pawnsToMove, board.pinnedToRook);

	generateEnPassant<sideToMove>(board, pawnsToMakeCapture, moveList);
	generatePawnCaptures<sideToMove, check>(board, pawnsToMakeCapture, moveList);

	generateFirstRankPawnMoves<sideToMove, check>(board, firstRankPawns, moveList);

	pawnsToMove -= firstRankPawns;
	generatePawnMoves<sideToMove, check>(board, pawnsToMove, moveList);
}


#endif
