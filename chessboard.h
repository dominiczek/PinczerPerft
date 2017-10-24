#ifndef CHESS_BOARD_H
#define CHESS_BOARD_H

#include <iostream>
#include <cstring>

#include "moves.h"
#include "zobrist.h"

class ChessBoard {
	
private:

	U64 all_pieces;
	U64 pieces[2];
	U64 pieces2[2][6];
	CASTLE_T castle[2];

	U64 key;
public:
	U64 checkMap = 0;
	U64 pinnedToBishop = 0;
	U64 pinnedToRook = 0;
	U64 pinnedPawns;

	U64 enPessantSqr;

	inline void reset() {
		all_pieces = 0;

		for(int i=0;i<2;i++) {
			pieces[i] = 0;
			pieces2[i][KING] = 0;
			pieces2[i][QUEEN] = 0;
			pieces2[i][KNIGHT] = 0;
			pieces2[i][ROOK] = 0;
			pieces2[i][BISHOP] = 0;
			pieces2[i][PAWN] = 0;

			castle[i] = 0;
		}

		enPessantSqr = 0;
	}

	void addNewPiece(int square, PIECE_T piece, bool side) {
		U64 mask = 1LL<<(square-1);
		pieces[side] |= mask;
		all_pieces |= mask;
		pieces2[side][piece] |= mask;
	}

	void setCastle(CASTLE_T castleRight, bool side) {
		castle[side] = castleRight;
	}

	inline U64 getKey() const {
		return key;
	}

	inline void updateKey(U64 moveKey) {
		key ^= moveKey;
	}

	inline void initKey(bool sideToMove) {
		U64 result = 0;

		for(int pieceType=0;pieceType<6;pieceType++) {
			U64 pieces = piecesByType<WHITE>(pieceType);
			ZOBRIST::generateForPieces(pieces, pieceType, WHITE, result);
		}
		for(int pieceType=0;pieceType<6;pieceType++) {
			U64 pieces = piecesByType<BLACK>(pieceType);
			ZOBRIST::generateForPieces(pieces, pieceType, BLACK, result);
		}

		if(enPessantSqr) {
			result ^= ZOBRIST::getEnPassantKey(enPessantSqr);
		}

		result^= ZOBRIST::getZobristCastelKey<WHITE>(castle[WHITE]);
		result^= ZOBRIST::getZobristCastelKey<BLACK>(castle[BLACK]);

		if(sideToMove) {
			result ^= ZOBRIST::getZobristBlackMoveKey();
		}

		this->key = result;
	}

	template <bool sideToMove>
	inline bool hasCastleRight(const CASTLE_T castleType) const {
		return castle[sideToMove] & castleType;
	}

	template <bool sideToMove>
	inline ChessBoard makeShortCastle() const {
		ChessBoard copy = createCopy();

		copy.removePiece<sideToMove>(kingStartSqr[sideToMove], KING);
		copy.addPiece<sideToMove>(kingShortCastleSqr[sideToMove], KING);
		copy.removePiece<sideToMove>(rookStartSqrH[sideToMove], ROOK);
		copy.addPiece<sideToMove>(rookShortCastleSqr[sideToMove], ROOK);

		copy.enPessantSqr = 0;

		copy.castle[sideToMove] = NO_CASTLE;

		return copy;
	}

	template <bool sideToMove>
	inline ChessBoard makeLongCastle() const {
		ChessBoard copy = createCopy();

		copy.removePiece<sideToMove>(kingStartSqr[sideToMove], KING);
		copy.addPiece<sideToMove>(kingLongCastleSqr[sideToMove], KING);
		copy.removePiece<sideToMove>(rookStartSqrA[sideToMove], ROOK);
		copy.addPiece<sideToMove>(rookLongCastleSqr[sideToMove], ROOK);

		copy.enPessantSqr = 0;
		copy.castle[sideToMove] = NO_CASTLE;

		return copy;
	}

	template <bool sideToMove>
	inline ChessBoard makeMove(const Move& move) const {

		ChessBoard copy = createCopy();

		copy.removePiece<sideToMove>(move.maskFrom, move.piece);
		copy.addPiece<sideToMove>(move.maskTo, move.piece);

		copy.enPessantSqr = move.enPessant;

		copy.setCastleRights<sideToMove>(move.maskFrom);

		return copy;
	}

	template <bool sideToMove>
	inline ChessBoard makeMove(const Promotion& move) const {

		ChessBoard copy = createCopy();

		copy.removePiece<sideToMove>(move.maskFrom, PAWN);
		copy.addPiece<sideToMove>(move.maskTo, move.promotion);

		copy.enPessantSqr = 0;

		return copy;
	}

	template <bool sideToMove>
	inline ChessBoard makeMove(const Capture& move) const {

		ChessBoard copy = createCopy();

		copy.removePiece<!sideToMove>(move.capturedPieceSquare, move.capturedPiece);
		copy.removePiece<sideToMove>(move.maskFrom, move.piece);
		copy.addPiece<sideToMove>(move.maskTo, move.piece);

		copy.enPessantSqr = 0;
		copy.setCastleRights<sideToMove>(move.maskFrom);
		copy.setCastleRightsRook<!sideToMove>(move.capturedPieceSquare);

		return copy;
	}

	template <bool sideToMove>
	inline ChessBoard makeMove(const PromotionCapture& move) const {

		ChessBoard copy = createCopy();

		copy.removePiece<!sideToMove>(move.maskTo, move.capturedPiece);
		copy.removePiece<sideToMove>(move.maskFrom, PAWN);
		copy.addPiece<sideToMove>(move.maskTo, move.promotion);

		copy.enPessantSqr = 0;
		copy.setCastleRightsRook<!sideToMove>(move.maskTo);

		return copy;
	}


	template <bool sideToMove>
	inline PIECE_T getPieceOnSquare(const U64 sqrMask) const {
		return 	pieces2[sideToMove][PAWN]   & sqrMask ? PAWN :
				pieces2[sideToMove][KNIGHT] & sqrMask ? KNIGHT :
				pieces2[sideToMove][BISHOP] & sqrMask ? BISHOP :
				pieces2[sideToMove][QUEEN]  & sqrMask ? QUEEN : ROOK;
	}

	template <bool sideToMove>
	inline PIECE_T getPieceOnSquare2(const U64 sqrMask) const {
		return 	pieces2[sideToMove][PAWN]   & sqrMask ? PAWN :
				pieces2[sideToMove][KNIGHT] & sqrMask ? KNIGHT :
				pieces2[sideToMove][BISHOP] & sqrMask ? BISHOP :
				pieces2[sideToMove][QUEEN]  & sqrMask ? QUEEN :
				pieces2[sideToMove][ROOK]  & sqrMask ? ROOK : KING;
	}

	template <bool sideToMove>
	inline PIECE_T removePieceAt(const U64 sqrMask) {
		all_pieces ^= sqrMask;
		pieces[sideToMove] ^= sqrMask;
		if(pieces2[sideToMove][PAWN] & sqrMask) {
			pieces2[sideToMove][PAWN]^= sqrMask;
			return PAWN;
		}
		if(pieces2[sideToMove][KNIGHT] & sqrMask) {
			pieces2[sideToMove][KNIGHT]^= sqrMask;
			return KNIGHT;
		}
		if(pieces2[sideToMove][BISHOP] & sqrMask) {
			pieces2[sideToMove][BISHOP]^= sqrMask;
			return BISHOP;
		}
		if(pieces2[sideToMove][QUEEN] & sqrMask) {
			pieces2[sideToMove][QUEEN]^= sqrMask;
			return QUEEN;
		}
		if(pieces2[sideToMove][ROOK] & sqrMask) {
			pieces2[sideToMove][ROOK]^= sqrMask;
			return ROOK;
		}
	}

	template <bool sideToMove>
	inline U64 piecesByType(PIECE_T pieceType) const {
		return pieces2[sideToMove][pieceType];
	}

	template <bool sideToMove, PIECE_T pieceType>
	inline U64 pinnedPiecesByType() const {

		if(pieceType == BISHOP) {
			return pieces2[sideToMove][pieceType] & pinnedToBishop;
		}
		if(pieceType == ROOK) {
			return pieces2[sideToMove][pieceType] & pinnedToRook;
		}

//		if(pieceType == QUEEN) {
//			return pieces2[sideToMove][pieceType] & pinnedToRook;
//		}

		return pieces2[sideToMove][pieceType] & getPinnedPieces();
	}

	template <bool sideToMove, PIECE_T pieceType>
	inline U64 notPinnedPiecesByType() const {
		return exclude(pieces2[sideToMove][pieceType], getPinnedPieces());
	}

	template <bool sideToMove>
	inline U64 notPinnedPiecesByType( PIECE_T pieceType) const {
		return exclude(pieces2[sideToMove][pieceType], getPinnedPieces());
	}

	template <bool sideToMove>
	inline U64 piecesBySide() const {
		return pieces[sideToMove];
	}

	inline U64 allPieces() const {
		return all_pieces;
	}

	inline U64 getCheckers() const {
		return all_pieces & checkMap;
	}

	inline bool isCheck() const {
		return checkMap;
	}

	inline U64 getPinnedPieces() const {
		return pinnedToBishop + pinnedToRook;
	}

	inline bool isDoubleCheck() const {
		U64 checkers = getCheckers();
		return checkers & (checkers - 1);
	}

	template <bool sideToMove>
	inline U64 createBasicKey(const Move &move) const {

		U64 key = 0;

		key = enPessantSqr ? key^ZOBRIST::getEnPassantKey(enPessantSqr) : key;

		key ^= ZOBRIST::getZobristBlackMoveKey();

		key ^= ZOBRIST::getZobristPieceKey<sideToMove>(move.maskFrom, move.piece);

		key = (castle[sideToMove] && move.maskFrom == kingStartSqr[sideToMove]) ? (key ^ ZOBRIST::getZobristCastelKey<sideToMove>(castle[sideToMove])) : key;
		key = (castle[sideToMove] && move.maskFrom == kingStartSqr[sideToMove]) ? (key ^ ZOBRIST::getZobristCastelKey<sideToMove>(NO_CASTLE)) : key;

		if((castle[sideToMove] & QUEEN_SIDE) && move.maskFrom == rookStartSqrA[sideToMove]) {
			key ^= ZOBRIST::getZobristCastelKey<sideToMove>(castle[sideToMove]);
			key ^= ZOBRIST::getZobristCastelKey<sideToMove>(castle[sideToMove] - QUEEN_SIDE);
		}
		if((castle[sideToMove] & KING_SIDE) && move.maskFrom == rookStartSqrH[sideToMove]) {
			key ^= ZOBRIST::getZobristCastelKey<sideToMove>(castle[sideToMove]);
			key ^= ZOBRIST::getZobristCastelKey<sideToMove>(castle[sideToMove] - KING_SIDE);
		}

		return key;
	}

	template <bool sideToMove>
	inline U64 createKey(const Move &move) const {

		U64 key = createBasicKey<sideToMove>(move);

		key ^= ZOBRIST::getZobristPieceKey<sideToMove>(move.maskTo, move.piece);

		if(move.enPessant) {
			key^=ZOBRIST::getEnPassantKey(move.enPessant);
		}

		return key;
	}

	template <bool sideToMove>
	inline U64 createKey(const Capture &move) const {

		U64 key = createBasicKey<sideToMove>(move);


		key ^= ZOBRIST::getZobristPieceKey<sideToMove>(move.maskTo, move.piece);
		key ^= ZOBRIST::getZobristPieceKey<!sideToMove>(move.capturedPieceSquare, move.capturedPiece);

		if((castle[!sideToMove] & QUEEN_SIDE) && move.maskTo == rookStartSqrA[!sideToMove]) {
			key ^= ZOBRIST::getZobristCastelKey<!sideToMove>(castle[!sideToMove]);
			key ^= ZOBRIST::getZobristCastelKey<!sideToMove>(castle[!sideToMove] - QUEEN_SIDE);
		}
		if((castle[!sideToMove] & KING_SIDE) && move.maskTo == rookStartSqrH[!sideToMove]) {
			key ^= ZOBRIST::getZobristCastelKey<!sideToMove>(castle[!sideToMove]);
			key ^= ZOBRIST::getZobristCastelKey<!sideToMove>(castle[!sideToMove] - KING_SIDE);
		}

		return key;
	}

	template <bool sideToMove>
	inline U64 createKey(const Promotion &move) const {

		U64 key = createBasicKey<sideToMove>(move);

		key ^= ZOBRIST::getZobristPieceKey<sideToMove>(move.maskTo, move.promotion);


		return key;
	}

	template <bool sideToMove>
	inline U64 createKey(const PromotionCapture &move)  const {

		U64 key = createBasicKey<sideToMove>(move);


		key ^= ZOBRIST::getZobristPieceKey<sideToMove>(move.maskTo, move.promotion);
		key ^= ZOBRIST::getZobristPieceKey<!sideToMove>(move.capturedPieceSquare, move.capturedPiece);

		if((castle[!sideToMove] & QUEEN_SIDE) && move.maskTo == rookStartSqrA[!sideToMove]) {
			key ^= ZOBRIST::getZobristCastelKey<!sideToMove>(castle[!sideToMove]);
			key ^= ZOBRIST::getZobristCastelKey<!sideToMove>(castle[!sideToMove] - QUEEN_SIDE);
		}
		if((castle[!sideToMove] & KING_SIDE) && move.maskTo == rookStartSqrH[!sideToMove]) {
			key ^= ZOBRIST::getZobristCastelKey<!sideToMove>(castle[!sideToMove]);
			key ^= ZOBRIST::getZobristCastelKey<!sideToMove>(castle[!sideToMove] - KING_SIDE);
		}

		return key;
	}

	template <bool sideToMove>
	constexpr inline U64 createKeyKingSideCastle()  const {

		U64 key = 0;

		key ^= ZOBRIST::getZobristBlackMoveKey();

		key = enPessantSqr ? key^ZOBRIST::getEnPassantKey(enPessantSqr) : key;

		key ^= ZOBRIST::getZobristPieceKey<sideToMove>(kingStartSqr[sideToMove], KING);
		key ^= ZOBRIST::getZobristPieceKey<sideToMove>(kingShortCastleSqr[sideToMove], KING);

		key ^= ZOBRIST::getZobristPieceKey<sideToMove>(rookStartSqrH[sideToMove], ROOK);
		key ^= ZOBRIST::getZobristPieceKey<sideToMove>(rookShortCastleSqr[sideToMove], ROOK);

		key ^= ZOBRIST::getZobristCastelKey<sideToMove>(castle[sideToMove]);
		key ^= ZOBRIST::getZobristCastelKey<sideToMove>(NO_CASTLE);

		return key;
	}

	template <bool sideToMove>
	constexpr inline U64 createKeyQueenSideCastle()  const {

		U64 key = 0;

		key ^= ZOBRIST::getZobristBlackMoveKey();

		key = enPessantSqr ? key^ZOBRIST::getEnPassantKey(enPessantSqr) : key;

		key ^= ZOBRIST::getZobristPieceKey<sideToMove>(kingStartSqr[sideToMove], KING);
		key ^= ZOBRIST::getZobristPieceKey<sideToMove>(kingLongCastleSqr[sideToMove], KING);

		key ^= ZOBRIST::getZobristPieceKey<sideToMove>(rookStartSqrA[sideToMove], ROOK);
		key ^= ZOBRIST::getZobristPieceKey<sideToMove>(rookLongCastleSqr[sideToMove], ROOK);

		key ^= ZOBRIST::getZobristCastelKey<sideToMove>(castle[sideToMove]);
		key ^= ZOBRIST::getZobristCastelKey<sideToMove>(NO_CASTLE);

		return key;
	}

private:

	inline ChessBoard createCopy() const {
		ChessBoard copy;

		std::memcpy(&copy, this, 136);

//		copy.key = enPessantSqr ? key^ZOBRIST::getEnPassantKey(enPessantSqr) : key;
//		copy.key ^= ZOBRIST::getZobristBlackMoveKey();

		return copy;
	}



	template <bool sideToMove>
	inline void addPiece(const U64 mask, const PIECE_T piece) {
		pieces[sideToMove] |= mask;
		all_pieces |= mask;
		pieces2[sideToMove][piece] |= mask;

//		key ^= ZOBRIST::getZobristPieceKey<sideToMove>(mask, piece);
	}

	template <bool sideToMove>
	inline void removePiece(const U64 mask, const PIECE_T piece) {
		all_pieces ^= mask;
		pieces[sideToMove] ^= mask;
		pieces2[sideToMove][piece] ^= mask;

//		key ^= ZOBRIST::getZobristPieceKey<sideToMove>(mask, piece);
	}

	template <bool sideToMove>
	inline void setCastleRights(const U64 maskFrom) {


//		if(castle[sideToMove]) {
//			if(maskFrom == kingStartSqr[sideToMove]) {
//
//				key ^= ZOBRIST::getZobristCastelKey<sideToMove>(castle[sideToMove]);
//				key ^= ZOBRIST::getZobristCastelKey<sideToMove>(NO_CASTLE);
//
//			}
//		}
		castle[sideToMove] = (maskFrom == kingStartSqr[sideToMove] ? NO_CASTLE : castle[sideToMove]);

		setCastleRightsRook<sideToMove>(maskFrom);
	}

	template <bool sideToMove>
	inline void setCastleRightsRook(const U64 square) {

//		if((castle[sideToMove] & QUEEN_SIDE) && square == rookStartSqrA[sideToMove]) {
//			key ^= ZOBRIST::getZobristCastelKey<sideToMove>(castle[sideToMove]);
//			key ^= ZOBRIST::getZobristCastelKey<sideToMove>(castle[sideToMove]-QUEEN_SIDE);
//		}
//		if((castle[sideToMove] & KING_SIDE) && square == rookStartSqrH[sideToMove]) {
//			key ^= ZOBRIST::getZobristCastelKey<sideToMove>(castle[sideToMove]);
//			key ^= ZOBRIST::getZobristCastelKey<sideToMove>(castle[sideToMove]-KING_SIDE);
//		}
		castle[sideToMove] -=
				((castle[sideToMove] & QUEEN_SIDE) && square == rookStartSqrA[sideToMove]) ? QUEEN_SIDE:0;
		castle[sideToMove] -=
				((castle[sideToMove] & KING_SIDE) && square == rookStartSqrH[sideToMove]) ? KING_SIDE:0;


	}

};


#endif
