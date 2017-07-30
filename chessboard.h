#ifndef CHESS_BOARD_H
#define CHESS_BOARD_H

#include <iostream>

#include "attacks.h"
#include "move.h"

using namespace std;



class ChessBoard {
	
private:
	U64 all_pieces;
	U64 pieces[2];
	U64 pieces2[2][6];

public:

	bool castleKingW;
	bool castleQueenW;
	bool castleKingB;
	bool castleQueenB;
	
	U64 enPessantSqr;

	ChessBoard() {

	}

	inline void reset() {
		all_pieces = 0;

		for(int i=0;i<2;i++) {
			pieces[i] = 0;
			pieces2[i][KING] = 0;
			pieces2[i][QUEEN] = 0;
			pieces2[i][KNIGHT] = 0;
			pieces2[i][ROOK] = 0;
			pieces2[i][BISHOP] = 0;
			pieces2[i][PAWN] = 0;
		}

		enPessantSqr = 0;
	}

	void addNewPiece(int square, PIECE_T piece, bool side) {
		U64 mask = 1LL<<(square-1);
		pieces[side] |= mask;
		all_pieces |= mask;
		pieces2[side][piece] |= mask;
	}


	template <bool sideToMove>
	inline ChessBoard makeMove(const Move& move) const {

		ChessBoard copy = createCopy(*this);

		copy.removePiece<sideToMove>(move.maskFrom, move.piece);
		copy.addPiece<sideToMove>(move.maskTo, move.piece);

		copy.enPessantSqr = move.enPessant;

		return copy;
	}

	template <bool sideToMove>
	inline ChessBoard makePromotion(const Move& move) const {

		ChessBoard copy = createCopy(*this);

		copy.removePiece<sideToMove>(move.maskFrom, PAWN);
		copy.addPiece<sideToMove>(move.maskTo, move.promotion);

		copy.enPessantSqr = 0;

		return copy;
	}

	template <bool sideToMove>
	inline ChessBoard makeCapture(const Capture& move) const {

		ChessBoard copy = createCopy(*this);


		copy.take<!sideToMove>(move.maskTo, move.capturedPiece);

		PIECE_T pieceToAdd = move.promotion ? move.promotion : move.piece;

		if(move.piece == PAWN && move.maskTo == enPessantSqr) {
			copy.removePiece<!sideToMove>(moveForward<!sideToMove>(enPessantSqr, 8), PAWN);
		}

		copy.removePiece<sideToMove>(move.maskFrom, move.piece);
		copy.addPiece<sideToMove>(move.maskTo, pieceToAdd);

		copy.enPessantSqr = 0;

		return copy;
	}



	template <bool sideToMove>
	inline ChessBoard makePromotionLight(const Move& move) const {

		ChessBoard copy = createLightCopy(*this, !sideToMove);

		copy.removePiece<sideToMove>(move.maskFrom, PAWN);
		copy.addPiece<sideToMove>(move.maskTo, move.promotion);

		return copy;
	}

	template <bool sideToMove>
	inline ChessBoard makeCaptureLight(const Capture& move) const {

		ChessBoard copy = createLightCopy(*this, !sideToMove);

		copy.take<!sideToMove>(move.maskTo, move.capturedPiece);

		PIECE_T pieceToAdd = move.promotion ? move.promotion : move.piece;

		if(move.piece == PAWN && move.maskTo == enPessantSqr) {
			copy.removePiece<!sideToMove>(moveForward<!sideToMove>(enPessantSqr, 8), PAWN);
		}

		copy.removePiece<sideToMove>(move.maskFrom, move.piece);
		copy.addPiece<sideToMove>(move.maskTo, pieceToAdd);

		return copy;
	}



	template <bool sideToMove>
	inline PIECE_T getPieceOnSquare(const U64 sqrMask) const {
		if(pieces2[sideToMove][PAWN] & sqrMask) {
			return PAWN;
		}
		if(pieces2[sideToMove][KNIGHT] & sqrMask) {
			return KNIGHT;
		}
		if(pieces2[sideToMove][BISHOP] & sqrMask) {
			return BISHOP;
		}
		if(pieces2[sideToMove][QUEEN] & sqrMask) {
			return QUEEN;
		}
		if(pieces2[sideToMove][ROOK] & sqrMask) {
			return ROOK;
		}

		return 0;
	}

	template <bool sideToMove>
	inline U64 piecesByType(PIECE_T pieceType) const {
		return pieces2[sideToMove][pieceType];
	}

	template <bool sideToMove>
	inline U64 piecesBySide() const {
		return pieces[sideToMove];
	}

	inline U64 allPieces() const {
		return all_pieces;
	}


private:

	inline ChessBoard createCopy(const ChessBoard &board) const {
		ChessBoard copy;
		copy.all_pieces = all_pieces;

		copy.pieces[WHITE] = pieces[WHITE];
		copy.pieces[BLACK] = pieces[BLACK];

		for(int i=0; i<6;i++) {
			copy.pieces2[WHITE][i] = pieces2[WHITE][i];
		}
		for(int i=0; i<6;i++) {
			copy.pieces2[BLACK][i] = pieces2[BLACK][i];
		}

		return copy;
	}

	inline ChessBoard createLightCopy(const ChessBoard &board, const bool side) const {
		ChessBoard copy;
		copy.all_pieces = all_pieces;

		copy.pieces[side] = pieces[side];

		copy.pieces2[!side][KING] = pieces2[!side][KING];

		for(int i=0; i<6;i++) {
			copy.pieces2[side][i] = pieces2[side][i];
		}

		return copy;
	}

	template <bool sideToMove>
	inline void addPiece(const U64 mask, const PIECE_T piece) {
		pieces[sideToMove] += mask;
		all_pieces += mask;
		pieces2[sideToMove][piece] += mask;
	}

	template <bool sideToMove>
	inline void removePiece(const U64 mask, const PIECE_T piece) {
		pieces[sideToMove] -= mask;
		all_pieces -= mask;
		pieces2[sideToMove][piece] -= mask;
	}

	template <bool sideToMove>
	inline void take(U64 mask, PIECE_T capturedPiece) {
		all_pieces &= ~mask;
		pieces[sideToMove] &= all_pieces;
		pieces2[sideToMove][capturedPiece] &= all_pieces;
	}

};

void printChessBoard(const ChessBoard &chessBoard);
void printBitMap(U64 bitMap, char c);



bool isOccupated(U64 mask, U64 board);


void printBitMap(U64 bitMap, char c);

extern char movesCodes[65][3];			
			



#endif
