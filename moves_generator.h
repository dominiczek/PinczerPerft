#ifndef MOVESGENERATOR_H_
#define MOVESGENERATOR_H_

template <bool sideToMove>
inline void generateMovesAndCaptures(const ChessBoard &board, const U64 sqrMask,
		const int piece, const U64 attacks, AllMoves &moveList) {

	if (!attacks) {
		return;
	}

	U64 captures = attacks & board.piecesBySide<!sideToMove>();
	U64 legalMoves = attacks - (attacks & board.allPieces());

	U64 to;
	while (captures) {
		to = popFirstPiece3(captures);

		PIECE_T capturedPiece = board.getPieceOnSquare<!sideToMove>(to);

		moveList.addCapture(Capture(piece, sqrMask, to, capturedPiece));
	}
	while (legalMoves) {
		to = popFirstPiece3(legalMoves);
		moveList.addMove(Move(piece, sqrMask, to));
	}
}

template <bool sideToMove>
inline bool isSquareAttacked(U64 sqrMask, const ChessBoard& board) {
	SQUARE_T sqr = getFirstPiece2(sqrMask);

	U64 attacks = kingMoves[sqr];
	if (attacks & board.piecesByType<sideToMove>(KING)) {
		return true;
	}

	U64 pawnsToMove = board.piecesByType<sideToMove>(PAWN);

	if(pawnsToMove & kingMoves[sqr]) {
		attacks = getPawnAttacks<sideToMove>(pawnsToMove);
		if (attacks & sqrMask) {
			return true;
		}
	}

	const U64 (*moves) = queenMoves[sqr];

	U64 queenAndRook = board.piecesByType<sideToMove>(ROOK) | board.piecesByType<sideToMove>(QUEEN);
	U64 queenAndBishop = board.piecesByType<sideToMove>(BISHOP) | board.piecesByType<sideToMove>(QUEEN);

	if (checkCaptureOnPositiveDirection(board.allPieces(), moves[NW], queenAndBishop)) {
		return true;
	}
	if (checkCaptureOnPositiveDirection(board.allPieces(), moves[N], queenAndRook)) {
		return true;
	}
	if (checkCaptureOnPositiveDirection(board.allPieces(), moves[NE], queenAndBishop)) {
		return true;
	}
	if (checkCaptureOnPositiveDirection(board.allPieces(), moves[E], queenAndRook)) {
		return true;
	}


	if (checkCaptureOnNegativeDirection(board.allPieces(), moves[SE], queenAndBishop)) {
		return true;
	}
	if (checkCaptureOnNegativeDirection(board.allPieces(), moves[S], queenAndRook)) {
		return true;
	}
	if (checkCaptureOnNegativeDirection(board.allPieces(), moves[SW], queenAndBishop)) {
		return true;
	}
	if (checkCaptureOnNegativeDirection(board.allPieces(), moves[W], queenAndRook)) {
		return true;
	}

	attacks = knightMoves[sqr];
	if (attacks & board.piecesByType<sideToMove>(KNIGHT)) {
		return true;
	}

	return false;
}


template <bool sideToMove>
inline bool isMoveLegal(const ChessBoard& board, const Move& move) {

	U64 allPieces = board.allPieces();
	allPieces += move.maskTo;
	allPieces ^= move.maskFrom;

	U64 king = board.piecesByType<!sideToMove>(KING);

	if(king == move.maskFrom) {
		king = move.maskTo;
	}

//	U64 king = move.piece ? board.myPieces(KING) : move.maskTo;


	SQUARE_T sqr = getFirstPiece2(king);

	U64 attacks = kingMoves[sqr];
	if (attacks & board.piecesByType<sideToMove>(KING)) {
		return false;
	}

	U64 pawnsToMove = board.piecesByType<sideToMove>(PAWN);

	if(pawnsToMove & kingMoves[sqr]) {
		attacks = getPawnAttacks<sideToMove>(pawnsToMove);
		if (attacks & king) {
			return false;
		}
	}

	const U64 (*moves) = queenMoves[sqr];

	U64 queenAndRook = board.piecesByType<sideToMove>(ROOK) | board.piecesByType<sideToMove>(QUEEN);
	U64 queenAndBishop = board.piecesByType<sideToMove>(BISHOP) | board.piecesByType<sideToMove>(QUEEN);

	if (checkCaptureOnPositiveDirection(allPieces, moves[NW], queenAndBishop)) {
		return false;
	}
	if (checkCaptureOnPositiveDirection(allPieces, moves[N], queenAndRook)) {
		return false;
	}
	if (checkCaptureOnPositiveDirection(allPieces, moves[NE], queenAndBishop)) {
		return false;
	}
	if (checkCaptureOnPositiveDirection(allPieces, moves[E], queenAndRook)) {
		return false;
	}


	if (checkCaptureOnNegativeDirection(allPieces, moves[SE], queenAndBishop)) {
		return false;
	}
	if (checkCaptureOnNegativeDirection(allPieces, moves[S], queenAndRook)) {
		return false;
	}
	if (checkCaptureOnNegativeDirection(allPieces, moves[SW], queenAndBishop)) {
		return false;
	}
	if (checkCaptureOnNegativeDirection(allPieces, moves[W], queenAndRook)) {
		return false;
	}

	attacks = knightMoves[sqr];
	if (attacks & board.piecesByType<sideToMove>(KNIGHT)) {
		return false;
	}

	return true;
}

template <bool sideToMove>
inline bool isMoveLegal(const ChessBoard& board, const Capture& move) {

	U64 allPieces = board.allPieces();
	allPieces -= move.maskFrom;

	U64 king = board.piecesByType<!sideToMove>(KING);

	U64 notCapturedPiecesMask = ~move.capturedPieceSquare;

	if(king == move.maskFrom) {
		king = move.maskTo;
	}

	SQUARE_T sqr = getFirstPiece2(king);

	U64 attacks = kingMoves[sqr];
	if (attacks & board.piecesByType<sideToMove>(KING)) {
		return false;
	}

	U64 pawnsToMove = board.piecesByType<sideToMove>(PAWN) & notCapturedPiecesMask;

	if(pawnsToMove & kingMoves[sqr]) {

		attacks = getPawnAttacks<sideToMove>(pawnsToMove);
		if (attacks & king) {
			return false;
		}
	}

	const U64 (*moves) = queenMoves[sqr];

	U64 queenAndRook = (board.piecesByType<sideToMove>(ROOK) | board.piecesByType<sideToMove>(QUEEN))
			& notCapturedPiecesMask;
	U64 queenAndBishop = (board.piecesByType<sideToMove>(BISHOP) | board.piecesByType<sideToMove>(QUEEN))
			& notCapturedPiecesMask;

	if (checkCaptureOnPositiveDirection(allPieces, moves[NW], queenAndBishop)) {
		return false;
	}
	if (checkCaptureOnPositiveDirection(allPieces, moves[N], queenAndRook)) {
		return false;
	}
	if (checkCaptureOnPositiveDirection(allPieces, moves[NE], queenAndBishop)) {
		return false;
	}
	if (checkCaptureOnPositiveDirection(allPieces, moves[E], queenAndRook)) {
		return false;
	}


	if (checkCaptureOnNegativeDirection(allPieces, moves[SE], queenAndBishop)) {
		return false;
	}
	if (checkCaptureOnNegativeDirection(allPieces, moves[S], queenAndRook)) {
		return false;
	}
	if (checkCaptureOnNegativeDirection(allPieces, moves[SW], queenAndBishop)) {
		return false;
	}
	if (checkCaptureOnNegativeDirection(allPieces, moves[W], queenAndRook)) {
		return false;
	}

	attacks = knightMoves[sqr];
	if (attacks & (board.piecesByType<sideToMove>(KNIGHT) & notCapturedPiecesMask)) {
		return false;
	}

	return true;
}

template <bool sideToMove>
inline void generateMoves(const ChessBoard& board, AllMoves& movesList) {

	SQUARE_T toMove;
	U64 attacks;
	U64 sqrMask;

	U64 queensToMove = board.piecesByType<sideToMove>(QUEEN);
	while (queensToMove) {
		sqrMask = popFirstPiece3(queensToMove, toMove);
		attacks = getPieceAttacks<QUEEN>(board.allPieces(), toMove);
		generateMovesAndCaptures<sideToMove>(board, sqrMask, QUEEN, attacks, movesList);
	}

	U64 bishopsToMove = board.piecesByType<sideToMove>(BISHOP);
	while (bishopsToMove) {
		sqrMask = popFirstPiece3(bishopsToMove, toMove);
		attacks = getPieceAttacks<BISHOP>(board.allPieces(), toMove);
		generateMovesAndCaptures<sideToMove>(board, sqrMask, BISHOP, attacks, movesList);
	}

	U64 rooksToMove = board.piecesByType<sideToMove>(ROOK);
	while (rooksToMove) {
		sqrMask = popFirstPiece3(rooksToMove, toMove);
		attacks = getPieceAttacks<ROOK>(board.allPieces(), toMove);
		generateMovesAndCaptures<sideToMove>(board, sqrMask, ROOK, attacks, movesList);
	}

	U64 knightsToMove = board.piecesByType<sideToMove>(KNIGHT);
	while (knightsToMove) {
		sqrMask = popFirstPiece3(knightsToMove, toMove);
		attacks = getPieceAttacks<KNIGHT>(board.allPieces(), toMove);
		generateMovesAndCaptures<sideToMove>(board, sqrMask, KNIGHT, attacks, movesList);
	}

	U64 kingToMove = board.piecesByType<sideToMove>(KING);
	toMove = getFirstPiece2(kingToMove);

	attacks = getKingAttack(toMove);

	generateMovesAndCaptures<sideToMove>(board, kingToMove, KING, attacks, movesList);
}


template <bool sideToMove>
inline void generateMoves2(const ChessBoard& board, AllMoves& movesList) {

	SQUARE_T toMove;
	U64 attacks;
	U64 sqrMask;

	U64 queensToMove = board.piecesByType<sideToMove>(QUEEN);
	if (queensToMove) {
		//sqrMask=getFirstPiece3(queensToMove, toMove);
		toMove = getFirstPiece2(queensToMove);
		attacks = getPieceAttacks<QUEEN>(board.allPieces(), toMove);
		generateMovesAndCaptures<sideToMove>(board, queensToMove, QUEEN, attacks, movesList);
	}

	U64 bishopsToMove = board.piecesByType<sideToMove>(BISHOP);
	if (bishopsToMove) {
		sqrMask = popFirstPiece3(bishopsToMove, toMove);
		attacks = getPieceAttacks<BISHOP>(board.allPieces(), toMove);
		generateMovesAndCaptures<sideToMove>(board, sqrMask, BISHOP, attacks, movesList);
		if (bishopsToMove) {
			toMove = getFirstPiece2(bishopsToMove);
			attacks = getPieceAttacks<BISHOP>(board.allPieces(), toMove);
			generateMovesAndCaptures<sideToMove>(board, bishopsToMove, BISHOP, attacks, movesList);
		}
	}

	U64 rooksToMove = board.piecesByType<sideToMove>(ROOK);
	if (rooksToMove) {
		sqrMask = popFirstPiece3(rooksToMove, toMove);
		attacks = getPieceAttacks<ROOK>(board.allPieces(), toMove);
		generateMovesAndCaptures<sideToMove>(board, sqrMask, ROOK, attacks, movesList);
		if (rooksToMove) {
			toMove = getFirstPiece2(rooksToMove);
			attacks = getPieceAttacks<ROOK>(board.allPieces(), toMove);
			generateMovesAndCaptures<sideToMove>(board, rooksToMove, ROOK, attacks, movesList);
		}
	}

	U64 knightsToMove = board.piecesByType<sideToMove>(KNIGHT);
	if (knightsToMove) {
		sqrMask = popFirstPiece3(knightsToMove, toMove);
		attacks = getPieceAttacks<KNIGHT>(board.allPieces(), toMove);
		generateMovesAndCaptures<sideToMove>(board, sqrMask, KNIGHT, attacks, movesList);
		if (knightsToMove) {
			toMove = getFirstPiece2(knightsToMove);
			attacks = getPieceAttacks<KNIGHT>(board.allPieces(), toMove);
			generateMovesAndCaptures<sideToMove>(board, knightsToMove, KNIGHT, attacks, movesList);
		}
	}

	U64 kingToMove = board.piecesByType<sideToMove>(KING);
	toMove = getFirstPiece2(kingToMove);

	attacks = getKingAttack(toMove);

	generateMovesAndCaptures<sideToMove>(board, kingToMove, KING, attacks, movesList);
}

#endif
