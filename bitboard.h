#ifndef BITBOARD
#define BITBOARD

#include "constants.h"

const U64 TWO_TO_63 = 0x8000000000000000;

template <bool sideToMove>
inline U64 moveForward(const U64 sqrMask, const int delta);

template <>
inline U64 moveForward<BLACK>(const U64 sqrMask, const int delta) {
	return sqrMask >> delta;
}

template <>
inline U64 moveForward<WHITE>(const U64 sqrMask, const int delta) {
	return sqrMask << delta;
}

template <bool sideToMove>
inline U64 moveBackward(const U64 sqrMask, const int delta);

template <>
inline U64 moveBackward<BLACK>(const U64 sqrMask, const int delta) {
	return sqrMask << delta;
}

template <>
inline U64 moveBackward<WHITE>(const U64 sqrMask, const int delta) {
	return sqrMask >> delta;
}

inline SQUARE_T getFirstPieceSquare(const U64 board) {
	return __builtin_ctzll(board);
}

inline U64 getFirstPieceMask(const U64 board) {
	return board & -board;
}

inline U64 getLastPieceMask(const U64 board) {
	return TWO_TO_63 >> __builtin_clzll(board);
}

inline U64 popFirstPieceMask(U64 &board) {
	U64 old = board;
	board &= board - 1LL;
	return old - board;
}

inline int popCount(U64 board) {
	return __builtin_popcountll(board);
}

inline U64 exclude(const U64 board, const U64 toExclude) {
	return board - (board & toExclude);
}


#endif
