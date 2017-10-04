#include "fen_parser.h"

#include <iostream>
#include <ctype.h>
#include <vector>
#include "chessboard.h"

using namespace std;

SQUARE_T FEN_PARSER::parseSquare(string squareCode) {
	char x = tolower(squareCode[0]) - 97;
	char y = squareCode[1] - 49;

	return y*8 + x;
}

U64 FEN_PARSER::parseSquareMap(string squareCode) {
	return 1ll<<parseSquare(squareCode);
}

vector<string> split(string toSplit, char c){
    vector<string> result;
	const char *str = toSplit.c_str();
    do {
        const char *begin = str;
        while(*str != c && *str) {
            str++;
        }
        result.push_back(string(begin, str));
    }
    while(*str++ != 0);
    return result;
}

bool FEN_PARSER::parseFen(ChessBoard& board, string fenRawString) {
	
	bool sideToMove;

	int boardIndexY = 7;
	int boardIndexX = 1;
	
	vector<string> fen = split(fenRawString, ' ');

	string positionString = fen[0];

	for(auto it=positionString.begin(); it!=positionString.end(); ++it) {
		char c = *it;
		
		if(isdigit(c)) {
			int offset = c - 48;
			
			boardIndexX += offset;
		} else {
			switch(c) {
				case 'r':  board.addNewPiece(boardIndexY * 8 + boardIndexX++, ROOK, BLACK); break;
				case 'n':  board.addNewPiece(boardIndexY * 8 + boardIndexX++, KNIGHT, BLACK); break;
				case 'b':  board.addNewPiece(boardIndexY * 8 + boardIndexX++, BISHOP, BLACK); break;
				case 'k':  board.addNewPiece(boardIndexY * 8 + boardIndexX++, KING, BLACK); break;
				case 'q':  board.addNewPiece(boardIndexY * 8 + boardIndexX++, QUEEN, BLACK); break;
				case 'p':  board.addNewPiece(boardIndexY * 8 + boardIndexX++, PAWN, BLACK); break;
				
				case 'R':  board.addNewPiece(boardIndexY * 8 + boardIndexX++, ROOK, WHITE); break;
				case 'N':  board.addNewPiece(boardIndexY * 8 + boardIndexX++, KNIGHT, WHITE); break;
				case 'B':  board.addNewPiece(boardIndexY * 8 + boardIndexX++, BISHOP, WHITE); break;
				case 'K':  board.addNewPiece(boardIndexY * 8 + boardIndexX++, KING, WHITE); break;
				case 'Q':  board.addNewPiece(boardIndexY * 8 + boardIndexX++, QUEEN, WHITE); break;
				case 'P':  board.addNewPiece(boardIndexY * 8 + boardIndexX++, PAWN, WHITE); break;
				
				case '/': boardIndexX = 1; boardIndexY--; break;
			}
			
		}			
	}

	string sideString = fen[1];
	
	if(sideString == "b") {
		sideToMove = 1;
	} else {
		sideToMove = 0;
	}
	
	string castle = fen[2];

	CASTLE_T castleW = 0, castleB = 0;

	if(castle.find('K') != string::npos) {
		castleW += KING_SIDE;
	}
	if(castle.find('k') != string::npos) {
		castleB += KING_SIDE;
	}
	if(castle.find('Q') != string::npos) {
		castleW += QUEEN_SIDE;
	}
	if(castle.find('k') != string::npos) {
		castleB += QUEEN_SIDE;
	}
		
	board.setCastle(castleW, WHITE);
	board.setCastle(castleB, BLACK);

	string enpessant = fen[3];
	
	if(enpessant != "-") {
		board.enPessantSqr = parseSquareMap(enpessant);
	} else {
		board.enPessantSqr = 0;
	}
	
	return sideToMove;
}
