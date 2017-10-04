#ifndef FEN_PARSER_H
#define FEN_PARSER_H

#include "chessboard.h"

namespace FEN_PARSER {

	bool parseFen(ChessBoard &board, char* fenString);

	int fieldCodeToField(char* moveCode);

}
#endif
