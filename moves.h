#ifndef MOVES_H_
#define MOVES_H_

#include "bitboard.h"
#include "zobrist.h"

union MoveKey {

	unsigned int key = 0;

	bool moveOrCapture;
	bool isPromotion;

	bool isCastle;
	bool KingOrQueenSide;

	bool isEnPassant;
	bool isPawnTwoSqure;

	bool empty1;
	bool empty2;

	char to;
	char from;
	char otherInfo;
};

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

	inline Move(MoveKey key) {

		this->maskFrom = 2<<key.from;
		this->maskTo = 2<<key.to;

		if(key.isPawnTwoSqure) {
			this->piece = PAWN;
			this->enPessant = key.otherInfo;
		} else {
			this->piece = key.otherInfo;
		}
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

	inline Capture(MoveKey key) {

		this->maskFrom = 2<<key.from;
		this->maskTo = 2<<key.to;

		if(key.isEnPassant) {
			this->piece = PAWN;
			this->capturedPieceSquare = key.otherInfo;
		} else {
			this->piece = key.otherInfo;
			this->capturedPieceSquare = maskTo;
		}
	}


	inline unsigned int createKey() {
		MoveKey moveKey;

		moveKey.moveOrCapture = 1;
		moveKey.isPromotion = 0;
		moveKey.isCastle = 0;

		moveKey.from = popFirstPieceMask(maskFrom);
		moveKey.to = popFirstPieceMask(maskTo);

		if(maskTo == capturedPieceSquare) {
			moveKey.otherInfo = piece;
		} else {
			moveKey.isEnPassant = 1;
			moveKey.otherInfo = popFirstPieceMask(capturedPieceSquare);
		}

		return moveKey.key;
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

	inline Promotion(MoveKey key) {
		this->piece = PAWN;
		this->maskFrom = 2<<key.from;
		this->maskTo = 2<<key.to;

		this->promotion = key.otherInfo;
	}

	Promotion() {

	}

	inline unsigned int createKey() {
		MoveKey moveKey;

		moveKey.moveOrCapture = 0;
		moveKey.isPromotion = 1;
		moveKey.isCastle = 0;

		moveKey.from = popFirstPieceMask(maskFrom);
		moveKey.to = popFirstPieceMask(maskTo);

		moveKey.otherInfo = promotion;

		return moveKey.key;
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

	inline PromotionCapture(MoveKey key) {
		this->piece = PAWN;
		this->maskFrom = 2<<key.from;
		this->maskTo = 2<<key.to;
		this->capturedPieceSquare = maskTo;

		this->promotion = key.otherInfo;
	}

	PromotionCapture() {

	}

	inline unsigned int createKey() {
		MoveKey moveKey;

		moveKey.moveOrCapture = 1;
		moveKey.isPromotion = 1;
		moveKey.isCastle = 0;

		moveKey.from = popFirstPieceMask(maskFrom);
		moveKey.to = popFirstPieceMask(maskTo);

		moveKey.otherInfo = promotion;

		return moveKey.key;
	}
};


#endif
