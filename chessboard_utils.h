#ifndef CHESSBOARD_UTILS_H_
#define CHESSBOARD_UTILS_H_


using namespace std;

namespace {
	const U64 RANKS_A_TO_G = 0xfefefefefefefefe;
	const U64 RANKS_B_TO_H = 0x7f7f7f7f7f7f7f7f;

	const U64 BLACK_FIRST_RANK_PAWNS = FULL_FIRST_RANK << 48;
	const U64 WHITE_FIRST_RANK_PAWNS = FULL_FIRST_RANK << 8;

	const U64 BLACK_PROMOTION_RANK_PAWNS = FULL_FIRST_RANK << 8;
	const U64 WHITE_PROMOTION_RANK_PAWNS = FULL_FIRST_RANK << 48;
}

namespace PAWNS {

	template <bool sideToMove>
	inline U64 getPawnAttacksLeft(const U64 pawns);

	template <bool sideToMove>
	inline U64 getPawnBackwardMoveLeft(const U64 pawn);

	template <bool sideToMove>
	inline U64 getPawnAttacksRight(const U64 pawns);

	template <bool sideToMove>
	inline U64 getPawnBackwardMoveRight(const U64 pawn);

	template <bool sideToMove>
	constexpr inline U64 getFirstPawnRankMask();

	template <bool sideToMove>
	constexpr inline U64 getPromotionRankMask();

	template <>
	constexpr inline U64 getFirstPawnRankMask<BLACK> () {
		return BLACK_FIRST_RANK_PAWNS;
	}

	template <>
	constexpr inline U64 getFirstPawnRankMask<WHITE> () {
		return WHITE_FIRST_RANK_PAWNS;
	}

	template <bool sideToMove>
	constexpr inline U64 getPromotionRankMask();

	template <>
	constexpr inline U64 getPromotionRankMask<BLACK> () {
		return BLACK_PROMOTION_RANK_PAWNS;
	}

	template <>
	constexpr inline U64 getPromotionRankMask<WHITE> () {
		return WHITE_PROMOTION_RANK_PAWNS;
	}

	template <>
	inline U64 getPawnAttacksLeft<WHITE>(const U64 pawns) {
		return (pawns & RANKS_B_TO_H) <<9;
	}

	template <>
	inline U64 getPawnAttacksLeft<BLACK>(const U64 pawns) {
		return (pawns & RANKS_B_TO_H) >>7;
	}

	template <>
	inline U64 getPawnBackwardMoveLeft<WHITE>(const U64 pawn) {
		return pawn >>9;
	}

	template <>
	inline U64 getPawnBackwardMoveLeft<BLACK>(const U64 pawn) {
		return pawn <<7;
	}

	template <>
	inline U64 getPawnAttacksRight<WHITE>(const U64 pawns) {
		return (pawns & RANKS_A_TO_G) << 7;
	}

	template <>
	inline U64 getPawnAttacksRight<BLACK>(const U64 pawns) {
		return (pawns & RANKS_A_TO_G) >> 9;
	}

	template <bool sideToMove>
	inline U64 getPawnAttacks(const U64 pawns) {
		return getPawnAttacksLeft<sideToMove>(pawns) | getPawnAttacksRight<sideToMove>(pawns);
	}


	template <>
	inline U64 getPawnBackwardMoveRight<WHITE>(const U64 pawn) {
		return pawn >> 7;
	}

	template <>
	inline U64 getPawnBackwardMoveRight<BLACK>(const U64 pawn) {
		return pawn << 9;
	}
}

inline void printBitMap(U64 bitMap, char c) {

	for(int y=7; y >-1; y--) {
		for(int x=0; x<8; x++) {
			int index=8*y + x;

			U64 mask = 1LL<<index;

			if(mask & bitMap) {
				cout<<c;
			} else {
				cout<<"o";
			}
		}
		cout<<endl;
	}

	cout<<endl;
}

inline void printChessBoard(const ChessBoard &chessBoard) {

	cout<<"   |A|B|C|D|E|F|G|H|"<<endl<<endl;
	for(int y=7; y >-1; y--) {
		cout<<y+1<<"  |";
		for(int x=0; x<8; x++) {
			int index=8*y + x;
			U64 mask = 1LL<<index;

			if(mask & chessBoard.piecesByType<BLACK>(PAWN)) {
				cout<<"p|";
			} else if(mask & chessBoard.piecesByType<WHITE>(PAWN)) {
				cout<<"P|";
			} else if(mask & chessBoard.piecesByType<BLACK>(ROOK)) {
				cout<<"r|";
			} else if(mask & chessBoard.piecesByType<WHITE>(ROOK)) {
				cout<<"R|";
			} else if(mask & chessBoard.piecesByType<BLACK>(KNIGHT)) {
				cout<<"n|";
			} else if(mask & chessBoard.piecesByType<WHITE>(KNIGHT)) {
				cout<<"N|";
			} else if(mask & chessBoard.piecesByType<BLACK>(BISHOP)) {
				cout<<"b|";
			} else if(mask & chessBoard.piecesByType<WHITE>(BISHOP)) {
				cout<<"B|";
			} else if(mask & chessBoard.piecesByType<BLACK>(KING)) {
				cout<<"k|";
			} else if(mask & chessBoard.piecesByType<WHITE>(KING)) {
				cout<<"K|";
			}  else if(mask & chessBoard.piecesByType<BLACK>(QUEEN)) {
				cout<<"q|";
			} else if(mask & chessBoard.piecesByType<WHITE>(QUEEN)) {
				cout<<"Q|";
			} else {
				cout<<"_|";
			}
		}
		cout<<"  "<<y+1<<endl;
	}
	cout<<endl<<"   |A|B|C|D|E|F|G|H|"<<endl;
}


#endif
