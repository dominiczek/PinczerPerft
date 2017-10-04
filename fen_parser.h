#ifndef FEN_PARSER_H
#define FEN_PARSER_H

#include "chessboard.h"
#include <string>

namespace FEN_PARSER {

	bool parseFen(ChessBoard &board, string fenString);

	SQUARE_T parseSquare(string squareCode);

	U64 parseSquareMap(string squareCode);

}
#endif
