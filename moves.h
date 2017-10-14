#ifndef MOVES_H_
#define MOVES_H_

#include "bitboard.h"
#include "zobrist.h"

class Move {
public:

	U64 maskFrom;
	U64 maskTo;
	PIECE_T piece;
	U64 enPessant;

	Move() {

	}

	inline Move(const Move& move) {
		this->piece = move.piece;
		this->maskFrom = move.maskFrom;
		this->maskTo = move.maskTo;
		this->enPessant = move.enPessant;
	}

	inline Move(const PIECE_T piece, const U64 from, const U64 to) {
		this->piece = piece;
		this->maskFrom = from;
		this->maskTo = to;
		this->enPessant = 0;
	}
};

class Capture : public Move {

public:

	U64 capturedPieceSquare;
	PIECE_T capturedPiece;

	inline Capture(const Capture& move) {
		this->piece = move.piece;
		this->maskFrom = move.maskFrom;
		this->maskTo = move.maskTo;
		this->capturedPieceSquare = move.capturedPieceSquare;
		this->capturedPiece = move.capturedPiece;
	}

	inline Capture() {

	}

	inline Capture(const PIECE_T piece,  const U64 from, const U64 to, PIECE_T capturedPiece) {
		this->piece = piece;
		this->maskFrom = from;
		this->maskTo = to;
		this->capturedPieceSquare = to;
		this->capturedPiece = capturedPiece;
	}
};

class PawnMove : public Move {

public:
	inline PawnMove(const PIECE_T piece,  const U64 from, const U64 to, const U64 enPessant) {
		this->piece = piece;
		this->maskFrom = from;
		this->maskTo = to;

		this->enPessant = enPessant;
	}
};

class PawnCapture : public Capture {

public:
	inline PawnCapture(const PIECE_T piece,  const U64 from, const U64 to, const U64 capturedPieceSquare) {
		this->piece = piece;
		this->maskFrom = from;
		this->maskTo = to;
		this->capturedPieceSquare = capturedPieceSquare;
		this->capturedPiece = PAWN;
	}
};


class Promotion : public Move {

public:

	PIECE_T promotion;

	inline Promotion(const U64 from, const U64 to, const PIECE_T promotion) {
		this->piece = PAWN;
		this->maskFrom = from;
		this->maskTo = to;

		this->promotion = promotion;
	}

	inline Promotion(const Promotion& move) {
		this->piece = move.piece;
		this->maskFrom = move.maskFrom;
		this->maskTo = move.maskTo;
		this->promotion = move.promotion;
	}

	Promotion() {

	}
};



class PromotionCapture : public Capture {

public:

	PIECE_T promotion;

	inline PromotionCapture(const PromotionCapture& move) {
		this->piece = move.piece;
		this->maskFrom = move.maskFrom;
		this->maskTo = move.maskTo;
		this->capturedPieceSquare = move.capturedPieceSquare;
		this->capturedPiece = move.capturedPiece;
		this->promotion = move.promotion;
	}

	inline PromotionCapture(const U64 from, const U64 to, const PIECE_T promotion, const PIECE_T captured) {
		this->piece = PAWN;
		this->maskFrom = from;
		this->maskTo = to;
		this->capturedPieceSquare = to;
		this->capturedPiece = captured;

		this->promotion = promotion;
	}

	PromotionCapture() {

	}
};


#endif
