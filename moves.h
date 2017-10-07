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

	inline Move(const PIECE_T piece, const U64 from, const U64 to) {
		this->piece = piece;
		this->maskFrom = from;
		this->maskTo = to;
		this->enPessant = 0;
	}

	template <bool siedToMove>
	inline U64 createKey() {

		U64 key = 0;

		key ^= ZOBRIST::getZobristPieceKey<siedToMove>(this->maskTo, this->piece);
		key ^= this->maskFrom;
	}
};

class Capture : public Move {

public:

	U64 capturedPieceSquare;

	inline Capture() {

	}

	inline Capture(const PIECE_T piece,  const U64 from, const U64 to) {
		this->piece = piece;
		this->maskFrom = from;
		this->maskTo = to;
		this->capturedPieceSquare = to;
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

	Promotion() {

	}
};



class PromotionCapture : public Capture {

public:

	PIECE_T promotion;

	inline PromotionCapture(const U64 from, const U64 to, const PIECE_T promotion) {
		this->piece = PAWN;
		this->maskFrom = from;
		this->maskTo = to;
		this->capturedPieceSquare = to;

		this->promotion = promotion;
	}

	PromotionCapture() {

	}
};


#endif
