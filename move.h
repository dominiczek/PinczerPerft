#ifndef MOVE_H
#define MOVE_H


#include "constants.h"

class Move {
public:

	U64 maskFrom;
	U64 maskTo;
	PIECE_T piece;
	U64 enPessant;
	int castle;

	inline Move(const PIECE_T piece,  const U64 from, const U64 to, const U64 enPessant = 0) {
		this->piece = piece;
		this->maskFrom = from;
		this->maskTo = to;

		this->castle = 0;
		this->enPessant = enPessant;
	}

	Move() {

	}
};

class Promotion : public Move {

public:

	PIECE_T promotion;

	inline Promotion(const PIECE_T piece, const U64 from, const U64 to, const PIECE_T promotion) {
		this->piece = piece;
		this->maskFrom = from;
		this->maskTo = to;

		this->promotion = promotion;
	}

	Promotion() {

	}
};

class Capture : public Move {

public:

	U64 capturedPieceSquare;
	PIECE_T capturedPiece;

	inline Capture(const PIECE_T piece,  const U64 from, const U64 to, const PIECE_T capturecPiece) {
		this->piece = piece;
		this->maskFrom = from;
		this->maskTo = to;
		this->capturedPieceSquare = to;

		this->capturedPiece = capturecPiece;
	}

	inline Capture(const PIECE_T piece,  const U64 from, const U64 to, const U64 capturedPieceSquare, const PIECE_T capturecPiece) {
		this->piece = piece;
		this->maskFrom = from;
		this->maskTo = to;
		this->capturedPieceSquare = capturedPieceSquare;

		this->capturedPiece = capturecPiece;
	}

	Capture() {

	}
};

class PromotionCapture : public Capture {

public:

	PIECE_T promotion;

	inline PromotionCapture(const PIECE_T piece, const U64 from, const U64 to, const PIECE_T promotion, const PIECE_T capturedPiece) {
		this->piece = piece;
		this->maskFrom = from;
		this->maskTo = to;
		this->capturedPieceSquare = to;

		this->promotion = promotion;
		this->capturedPiece = capturedPiece;
	}

	PromotionCapture() {

	}
};


#endif
