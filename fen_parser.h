#ifndef FEN_PARSER_H
#define FEN_PARSER_H

#include "chessboard.h"

namespace FEN_PARSER {

	bool parseFen(ChessBoard &board, string fenString);

	SQUARE_T parseSquare(string squareCode);

	U64 parseSquareMap(string squareCode);


	template <bool sideToMove>
	ChessBoard makeMove(const ChessBoard &board, string move);

	ChessBoard makeMove(bool &siedToMove, ChessBoard &board, string move);

	ChessBoard makeMovesList(bool &siedToMove, ChessBoard &board, string moves);

}
#endif
