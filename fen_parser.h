#ifndef FEN_PARSER_H
#define FEN_PARSER_H

#include "chessboard.h"

namespace FEN_PARSER {

	bool parseFen(ChessBoard &board, std::string fenString);

	SQUARE_T parseSquare(std::string squareCode);

	U64 parseSquareMap(std::string squareCode);


	template <bool sideToMove>
	ChessBoard makeMove(const ChessBoard &board, std::string move);

	ChessBoard makeMove(bool &siedToMove, ChessBoard &board, std::string move);

	ChessBoard makeMovesList(bool &siedToMove, ChessBoard &board, std::string moves);

}
#endif
