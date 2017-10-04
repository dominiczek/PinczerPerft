#ifndef MOVE_LIST_H_
#define MOVE_LIST_H_

#include "moves.h"

namespace {

	template <class M>
	constexpr int moveListSize();

	template<>
	constexpr int moveListSize<Move>() {
		return 128;
	}

	template<>
	constexpr int moveListSize<Capture>() {
		return 64;
	}

	template<>
	constexpr int moveListSize<Promotion>() {
		return 32;
	}

	template<>
	constexpr int moveListSize<PromotionCapture>() {
		return 32;
	}

}

template <class M>
class MoveList {
private:
	M moves[moveListSize<M>()];
	M *movesHead = moves;
public:
	inline void addMove(const M &move) {
		*movesHead++=move;
	}
	inline M* getMoves() {
		return moves;
	}
	inline bool hasNextMove(const M* current) const {
		return movesHead != current;
	}
	inline int size() const {
		return movesHead - moves;
	}
};

class AllMoves {
public:
	bool longCastle = 0;
	bool shortCastle = 0;
	MoveList<Move> moves;
	MoveList<Capture> captures;
	MoveList<Promotion> promotions;
	MoveList<PromotionCapture> promotionCaptures;

	inline void addQueenSideCastle() {
		longCastle = true;
	}

	inline void addKingSideCastle() {
		shortCastle = true;
	}

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

	inline U64 allMovesCount() {
		return moves.size() + captures.size() + promotionCaptures.size() + promotions.size() + longCastle + shortCastle;
	}
};

#endif
