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

inline int popCount(const U64 board) {
	return __builtin_popcountll(board);
}

//inline int popCount(U64 board) {
//   int count = 0;
//   while (board) {
//       count++;
//       board &= board - 1; // reset LS1B
//   }
//   return count;
//}

//constexpr U64 k1 = (0x5555555555555555); /*  -1/3   */
//constexpr U64 k2 = (0x3333333333333333); /*  -1/5   */
//constexpr U64 k4 = (0x0f0f0f0f0f0f0f0f); /*  -1/17  */
//constexpr U64 kf = (0x0101010101010101); /*  -1/255 */
//
//inline int popCount(U64 x) {
//    x =  x       - ((x >> 1)  & k1); /* put count of each 2 bits into those 2 bits */
//    x = (x & k2) + ((x >> 2)  & k2); /* put count of each 4 bits into those 4 bits */
//    x = (x       +  (x >> 4)) & k4 ; /* put count of each 8 bits into those 8 bits */
//    x = (x * kf) >> 56; /* returns 8 most significant bits of x + (x<<8) + (x<<16) + (x<<24) + ...  */
//    return (int) x;
//}

inline U64 exclude(const U64 board, const U64 toExclude) {
	return board - (board & toExclude);
}


#endif
