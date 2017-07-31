#ifndef MOVELIST_H_
#define MOVELIST_H_

#include "move.h"

template <class M>
class MoveList {
private:
	M moves[64];
	M *movesHead = moves;
public:
	inline void addMove(const M &move) {
		*movesHead++=move;
	}
	inline M* getMoves() {
		return moves;
	}
	inline bool hasNextMove(M* current) const {
		return movesHead != current;
	}
};


class AllMoves {
public:
	MoveList<Move> moves;
	MoveList<Capture> captures;
	MoveList<Promotion> promotions;
	MoveList<PromotionCapture> promotionCaptures;

	inline void addMove(const Move& move) {
		moves.addMove(move);
	}

	inline void addCapture(const Capture& move) {
		captures.addMove(move);
	}

	inline void addPromotion(const Promotion& move) {
		promotions.addMove(move);
	}

	inline void addPromotionCapture(const PromotionCapture& move) {
		promotionCaptures.addMove(move);
	}
};

#endif
