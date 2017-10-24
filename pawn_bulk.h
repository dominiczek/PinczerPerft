#ifndef PAWNS_BULK_H_
#define PAWNS_BULK_H_

#include <iostream>
#include "chessboard.h"
#include "legality.h"
#include "moves.h"

template <bool sideToMove, CHECK_T isCheck>
inline U64 countPawnMoves(const ChessBoard &board,  const U64 pawns) {

	U64 notPinedPawns = exclude(pawns , board.pinnedPawns);
	U64 legalPawnMoves = exclude(moveForward<sideToMove>(notPinedPawns, 8), board.allPieces());

	if(isCheck == CHECK){
		legalPawnMoves &= board.checkMap;
	}

	return legalPawnMoves;
}


template <bool sideToMove, CHECK_T check>
inline U64 countFirstRankPawnMoves(const ChessBoard &board,  const U64 pawns) {

	U64 notPinedPawns = exclude(pawns , board.pinnedPawns);

	U64 legalPawnMoves = exclude(moveForward<sideToMove>(notPinedPawns, 8), board.allPieces());
	U64 legalTowSquareMoves = exclude(moveForward<sideToMove>(legalPawnMoves, 8), board.allPieces());

	if(check) {
		legalPawnMoves &= board.checkMap;
		legalTowSquareMoves &= board.checkMap;
	}
	return  legalPawnMoves + legalTowSquareMoves;
}

template <bool sideToMove>
inline int countEnPassant(const ChessBoard &board, const U64 pawns) {

	int result = 0;

	if(board.enPessantSqr) {
		U64 pawnsToDoEnPessant = PAWNS::getPawnAttacks<!sideToMove>(board.enPessantSqr) & pawns;

		if(pawnsToDoEnPessant) {
			U64 pawn = popFirstPieceMask(pawnsToDoEnPessant);
			PawnCapture capture(PAWN, pawn, board.enPessantSqr, moveForward<!sideToMove>(board.enPessantSqr, 8));
			if(isEnPassantMoveLegal<sideToMove>(board, capture)) {
				result++;
			}
			if(pawnsToDoEnPessant) {

				PawnCapture capture(PAWN, pawnsToDoEnPessant, board.enPessantSqr, moveForward<!sideToMove>(board.enPessantSqr, 8));
				if(isEnPassantMoveLegal<sideToMove>(board, capture)) {
					result++;
				}
			}
		}

	}

	return result;
}

template <bool sideToMove, CHECK_T check>
inline int countPawnCaptures(const ChessBoard &board, const U64 pawns) {


	U64 toCapture = board.piecesBySide<!sideToMove>();

	if(check == CHECK) {
		toCapture &= board.checkMap;
	}

	U64 pawnAttacksL = PAWNS::getPawnAttacksLeft<sideToMove>(pawns) & toCapture;
	U64 pawnAttacksR = PAWNS::getPawnAttacksRight<sideToMove>(pawns) & toCapture;

	U64 pinnedPawns = pawns & board.pinnedToBishop;

	if(!pinnedPawns) {
		return  popCount(pawnAttacksL) + popCount(pawnAttacksR);
	}

	int result = 0;
	while(pawnAttacksL) {
		U64 move = popFirstPieceMask(pawnAttacksL);
		U64 from =  PAWNS::getPawnBackwardMoveLeft<sideToMove>(move);

		if((from & ~board.pinnedToBishop) || (move & board.pinnedToBishop)) {
			result++;
		}
	}

	while(pawnAttacksR) {
		U64 move = popFirstPieceMask(pawnAttacksR);
		U64 from = PAWNS::getPawnBackwardMoveRight<sideToMove>(move);

		if((from & ~board.pinnedToBishop) || (move & board.pinnedToBishop)) {
			result++;
		}
	}

	return result;
}



template <bool sideToMove, CHECK_T check>
inline int countPromotions(const ChessBoard &board,  const U64 pawns) {

	U64 pawnsToMove = exclude(moveForward<sideToMove>( exclude(pawns, board.getPinnedPieces()), 8), board.allPieces());

	if(check) {
		pawnsToMove &= board.checkMap;
	}

	return 4 * popCount(pawnsToMove);
}

template <bool sideToMove, CHECK_T check>
inline int countPromotionCaptures(const ChessBoard &board,  const U64 pawns) {

	int result = 0;

	U64 toCapture = board.piecesBySide<!sideToMove>();

	if(check == CHECK) {
		toCapture &= board.checkMap;
	}

	U64 pawnAttacksL = PAWNS::getPawnAttacksLeft<sideToMove>(pawns) & toCapture;
	U64 pawnAttacksR = PAWNS::getPawnAttacksRight<sideToMove>(pawns) & toCapture;

	if(!(pawns & board.pinnedToBishop)) {
		return 4 * popCount(pawnAttacksL + pawnAttacksR);
	}

	while(pawnAttacksL) {
		U64 move = popFirstPieceMask(pawnAttacksL);
		U64 from = PAWNS::getPawnBackwardMoveLeft<sideToMove>(move);


		if((from & ~board.pinnedToBishop) || (move & board.pinnedToBishop)) {
			result +=4;
		}
	}

	while(pawnAttacksR) {
		U64 move = popFirstPieceMask(pawnAttacksR);
		U64 from = PAWNS::getPawnBackwardMoveRight<sideToMove>(move);

		if((from & ~board.pinnedToBishop) || (move & board.pinnedToBishop)) {
			result += 4;
		}
	}

	return result;
}

template <bool sideToMove, CHECK_T check>
inline int countPawnsMoves(const ChessBoard &board) {

	int result = 0;

	U64 firstRankMask = PAWNS::getFirstPawnRankMask<sideToMove>();
	U64 promotionRankMask = PAWNS::getPromotionRankMask<sideToMove>();

	U64 pawnsToMove = board.piecesByType<sideToMove>(PAWN);
	U64 firstRankPawns = pawnsToMove & firstRankMask;
	U64 promotionPawns = pawnsToMove & promotionRankMask;

	if(promotionPawns) {
		pawnsToMove -= promotionPawns;
		result += countPromotions<sideToMove, check>(board, promotionPawns);
		result += countPromotionCaptures<sideToMove, check>(board, exclude(promotionPawns, board.pinnedToRook));
	}

	U64 pawnsToMakeCapture = exclude(pawnsToMove, board.pinnedToRook);

	result += countEnPassant<sideToMove>(board, pawnsToMakeCapture);
	result += countPawnCaptures<sideToMove, check>(board, pawnsToMakeCapture);

	U64 allPawnMoves = countFirstRankPawnMoves<sideToMove, check>(board, firstRankPawns);

	pawnsToMove -= firstRankPawns;
	allPawnMoves  += countPawnMoves<sideToMove, check>(board, pawnsToMove);

	return result + popCount(allPawnMoves);
}


#endif
