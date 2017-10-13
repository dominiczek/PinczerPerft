
#ifndef ZOBRIST_H_
#define ZOBRIST_H_

#include <random>

namespace {
	U64 zobrist_keys[2][64][6];
	U64 en_pessant_key[64];
	U64 castle_keys[2][4];
	U64 black_move_key;


	U64 depths[10];

	const U64 maxU64 = 0xFFFFFFFFFFFFFFFF;

	bool isReady = false;
}

namespace ZOBRIST {

	inline void Initialize() {

		if(isReady) {
			return;
		}

	    std::random_device rd;
	    std::mt19937 gen(rd());
	    std::uniform_int_distribution<U64> randomU64(0, maxU64);

	    black_move_key = randomU64(gen);

		for(int i= 0; i< 64; i++) {
			for(int j= 0; j< 6; j++) {
				zobrist_keys[WHITE][i][j] = randomU64(gen);
				zobrist_keys[BLACK][i][j] = randomU64(gen);
			}
			en_pessant_key[i] = randomU64(gen);
		}

		castle_keys[WHITE][NO_CASTLE] = randomU64(gen);
		castle_keys[WHITE][KING_SIDE] = randomU64(gen);
		castle_keys[WHITE][QUEEN_SIDE] = randomU64(gen);
		castle_keys[WHITE][BOTH_SIDE] = randomU64(gen);

		castle_keys[BLACK][NO_CASTLE] = randomU64(gen);
		castle_keys[BLACK][KING_SIDE] = randomU64(gen);
		castle_keys[BLACK][QUEEN_SIDE] = randomU64(gen);
		castle_keys[BLACK][BOTH_SIDE] = randomU64(gen);

		for(int i = 0;i<10;i++) {
			depths[i] = randomU64(gen);
		}

		isReady = true;
	}

	inline void CleanUp() {
		isReady = false;
	}

	inline void generateForPieces(U64 pieces, PIECE_T piece, bool side, U64 &result) {
		while(pieces) {
			SQUARE_T sqr = getFirstPieceSquare(pieces);
			popFirstPieceMask(pieces);

			result ^= zobrist_keys[sqr][piece][side];
		}
	}

	template<bool side>
	inline U64 getZobristPieceKey(const U64 mask, const PIECE_T piece) {
		return zobrist_keys[side][getFirstPieceSquare(mask)][piece];
	}

	template<bool side>
	inline U64 getZobristCastelKey(const CASTLE_T castle) {
		return castle_keys[side][castle];
	}

	inline U64 getZobristBlackMoveKey() {
		return black_move_key;
	}

	inline U64 getEnPassantKey(const U64 mask) {
		return en_pessant_key[getFirstPieceSquare(mask)];
	}

	inline U64 getDepthKey(const int d) {
		return depths[d];
	}
}

#endif
