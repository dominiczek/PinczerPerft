#ifndef MOVES_PROVIDER_H_
#define MOVES_PROVIDER_H_

#include <cassert>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <vector>
#include "constants.h"

using namespace std;

extern const U64 kingMoves[64];
extern const U64 queenMoves[64][8];
extern const U64 knightMoves[64];

extern const U64 queenMovesNW[64];
extern const U64 queenMovesN[64];
extern const U64 queenMovesNE[64];
extern const U64 queenMovesE[64];
extern const U64 queenMovesSE[64];
extern const U64 queenMovesS[64];
extern const U64 queenMovesSW[64];
extern const U64 queenMovesW[64];

namespace {
const char rookMagicFileName[] = "magic/rook_magics.magic";
const char rookMasksFileName[] = "magic/rook_masks.magic";
const char rookShiftsFileName[] = "magic/rook_shifts.magic";
const char rookOffsetsFileName[] = "magic/rook_offsets.magic";
const char rookAttackTableFileName[] = "magic/rook_attack_table.magic";

const char bishopMagicsFileName[] = "magic/bishop_magics.magic";
const char bishopMasksFileName[] = "magic/bishop_masks.magic";
const char bishopShiftsFileNam[] = "magic/bishop_shifts.magic";
const char bishopOffsetsFileName[] = "magic/bishop_offsets.magic";
const char bishopAttackTableFileName[] = "magic/bishop_attack_table.magic";

class MovesProvider {
public:
	void Initialize();

	inline U64 rookAttacks(const U64 bitboard, const SQUARE_T index) const {
		U64 mask = rookMasks[index];
		U64 magic = rookMagics[index];
		int offset = rookOffsets[index];
		int shift = rookShifts[index];
		return rook_attack_table[attackTableIndex(bitboard, mask, magic, shift, offset)];
	}

	inline U64 bishopAttacks(const U64 bitboard, const SQUARE_T index) const {
		U64 mask = bishopMasks[index];
		U64 magic = bishopMagics[index];
		int offset = bishopOffsets[index];
		int shift = bishopShifts[index];
		return bishop_attack_table[attackTableIndex(bitboard, mask, magic, shift, offset)];
	}

	inline U64 queenAttacks(const U64 bitboard, const SQUARE_T index) const {
		return rookAttacks(bitboard, index) | bishopAttacks(bitboard, index);
	}

private:
	inline U64 attackTableIndex(const U64 bitboard, U64 mask, U64 magic, int shift, int offset) const {
		U64 occupancy = bitboard & mask;
		return ((occupancy * magic) >> shift) + offset;
	}

	U64 rookMasks[SQUARES_COUNT];
	U64 rookMagics[SQUARES_COUNT];
	int rookShifts[SQUARES_COUNT];
	int rookOffsets[SQUARES_COUNT];
	std::vector<U64> rook_attack_table;

	U64 bishopMasks[SQUARES_COUNT];
	U64 bishopMagics[SQUARES_COUNT];
	int bishopShifts[SQUARES_COUNT];
	int bishopOffsets[SQUARES_COUNT];
	std::vector<U64> bishop_attack_table;
};

template<typename T>
vector<T> readVectorFromFile(const std::string& filename) {
	vector<T> v;
	ifstream ifs(filename.c_str(), ios::in);
	if (!ifs.is_open()) {
		cerr << "Could not open file " << filename << endl;
		exit(-1);
	}
	string s;
	while (ifs >> s) {
		v.push_back(T(strtoull(s.c_str(), nullptr, 16)));
	}
	ifs.close();
	v.shrink_to_fit();
	return v;
}

void MovesProvider::Initialize() {
	vector<U64> rook_masks = readVectorFromFile<U64>(rookMasksFileName);
	vector<U64> bishop_masks = readVectorFromFile<U64>(bishopMasksFileName);
	vector<U64> rook_magics = readVectorFromFile<U64>(rookMagicFileName);
	vector<U64> bishop_magics = readVectorFromFile<U64>(bishopMagicsFileName);
	vector<int> rook_offsets = readVectorFromFile<int>(rookOffsetsFileName);
	vector<int> bishop_offsets = readVectorFromFile<int>(bishopOffsetsFileName);
	vector<int> rook_shifts = readVectorFromFile<int>(rookShiftsFileName);
	vector<int> bishop_shifts = readVectorFromFile<int>(bishopShiftsFileNam);
	rook_attack_table = readVectorFromFile<U64>(rookAttackTableFileName);
	bishop_attack_table = readVectorFromFile<U64>(bishopAttackTableFileName);

	for (int i = 0; i < SQUARES_COUNT; ++i) {
		rookMagics[i] = rook_magics.at(i);
		rookMasks[i] = rook_masks.at(i);
		rookShifts[i] = SQUARES_COUNT - rook_shifts.at(i);
		rookOffsets[i] = rook_offsets.at(i);

		bishopMagics[i] = bishop_magics.at(i);
		bishopMasks[i] = bishop_masks.at(i);
		bishopShifts[i] = SQUARES_COUNT - bishop_shifts.at(i);
		bishopOffsets[i] = bishop_offsets.at(i);
	}

	std::cout << "Magic - Initialized." << std::endl;
}

MovesProvider sliderAttacks;
}

namespace MOVE_PROVIDER {

inline void Initialize() {
	sliderAttacks.Initialize();
}

template<PIECE_T piece>
inline U64 getMoves(const U64 board, SQUARE_T sqr);

template<>
inline U64 getMoves<ROOK>(const U64 board, SQUARE_T sqr) {
	return sliderAttacks.rookAttacks(board, sqr);
}

template<>
inline U64 getMoves<QUEEN>(const U64 board, SQUARE_T sqr) {
	return sliderAttacks.queenAttacks(board, sqr);
}

template<>
inline U64 getMoves<BISHOP>(const U64 board, SQUARE_T sqr) {
	return sliderAttacks.bishopAttacks(board, sqr);
}

template<>
inline U64 getMoves<KNIGHT>(const U64 board, SQUARE_T sqr) {
	return knightMoves[sqr];
}

}
#endif
