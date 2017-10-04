#include "fen_parser.h"

#include <iostream>
#include <ctype.h>
#include <string.h>
#include "chessboard.h"
#include <vector>

using namespace std;

int FEN_PARSER::fieldCodeToField(char* moveCode) {
	char x = tolower(moveCode[0]) - 97;
	char y = moveCode[1] - 49;
	
	return y*8 + x;
}

vector<string> split(const char *str, char c = ' ')
{
    vector<string> result;

    do
    {
        const char *begin = str;

        while(*str != c && *str)
            str++;

        result.push_back(string(begin, str));
    } while (0 != *str++);

    return result;
}

bool FEN_PARSER::parseFen(ChessBoard& board, char* fenString) {
	
	bool sideToMove;

	char c = 'x';
	
	int boardIndexY = 7;
	int boardIndexX = 1;
	int stringIndex = 0;
	
	for(stringIndex = 0; c !=' '; stringIndex++) {
		c = fenString[stringIndex];
		
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

	c = fenString[stringIndex];
	
	if(c=='b') {
		sideToMove = 1;
		cout<<"BLACK"<<endl;
	} else {
		sideToMove = 0;
		cout<<"WHITE"<<endl;
	}
	
	c = fenString[++++stringIndex];

	CASTLE_T castleW = 0, castleB = 0;
	
	if(c == '-') {
	} else {
		while(c!=' ') {
			if(c=='K') {
				castleW += KING_SIDE;
				cout<<"King side W"<<endl;
			}
			if(c=='k') {
				castleB += KING_SIDE;
				cout<<"King side B"<<endl;
			}
			if(c=='Q') {
				castleW += QUEEN_SIDE;
				cout<<"Queen side W"<<endl;
			}
			if(c=='q') {
				castleB += QUEEN_SIDE;
				cout<<"Queen side B"<<endl;
			}
			c = fenString[++stringIndex];
		}
		board.setCastle(castleW, WHITE);
		board.setCastle(castleB, BLACK);
	}
		
	char enpessant[2];	
	stringIndex++;
	
	c = fenString[++stringIndex];
	if(c != '-' && c!='\0') {
		cout<<c;
		enpessant[0] = c;
		c = fenString[++stringIndex];
		cout<<c<<endl;
		enpessant[1] = c;
		
		cout<<enpessant<<endl;
		
		int enpessantSqr = fieldCodeToField(enpessant);
		cout<<"EN - "<<enpessantSqr<<endl;
		board.enPessantSqr = 1LL<<(enpessantSqr);
		
//		if(board.sideToMove) {
////			board.enPessantPawn = board.enPessantSqr<<8;b
//		} else {
//			board.enPessantPawn = board.enPessantSqr>>8;
//		}
		
	}
	
	return sideToMove;
	
}
