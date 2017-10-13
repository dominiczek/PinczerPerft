#include <iostream>
#include <algorithm>
#include <cctype>
#include <string>


#include "fen_parser.h"
#include "cache.h"

using namespace std;

string START_POS = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq -";

int main2(int argc, char** argv) {
	
	ChessBoard chessBoard;

	string line;
	while(true) {
		
		getline(cin, line);
		
		line.shrink_to_fit();

		std::transform(line.begin(), line.end(), line.begin(), ::tolower);

		if(line == "uci") {
			cout<<"id name PinczerChess"<<endl;
			cout<<"id author Dominik Kunicki"<<endl;

			//send option here
			cout<<"uciok"<<endl;
		}
		if(line == "debug") {
		
		}
		if(line == "isready") {

			ZOBRIST::Initialize();
			MOVE_PROVIDER::Initialize();
			int CACHE_SIZE = 2*1024*1024 - 1;
			CACHE::Initialize(CACHE_SIZE*1024/12);
			// create engine
			cout<<"readyok\r\n"<<endl;
		}
		if(line.find("setoption") == 0) {
			
		}
		if(line.find("register") == 0) {
			
		}
		if(line.find("ucinewgame") == 0) {
			chessBoard.reset();
		}
		if(line.find("position") == 0) {
			line = line.substr(9);
			if(line.find("fen")==0) {
				unsigned int movesTagPosition = line.find("moves");
				if(movesTagPosition == string::npos) {
					line = line.substr(4, movesTagPosition);
					FEN_PARSER::parseFen(chessBoard, line.substr(4));
				} else {
					line = line.substr(4, movesTagPosition);
					FEN_PARSER::parseFen(chessBoard, line);
				}
			} else if( line.find("startpos")==0) {
				unsigned int movesTagPosition = line.find("moves");
				if(movesTagPosition == string::npos) {
					FEN_PARSER::parseFen(chessBoard, START_POS);
				} else {
					line = line.substr(movesTagPosition + 6);
					bool side = FEN_PARSER::parseFen(chessBoard, START_POS);
					FEN_PARSER::makeMovesList(side, chessBoard, line);
				}
			}
			printChessBoard(chessBoard);
			
		}	
		if(line.find("go") == 0) {
			
		}
		if(line.find("stop") == 0) {
			cout<<"Stop searching.";
		}
		if(line.find("ponderhit") == 0) {
			
		}
		if(line.find("quit") == 0) {
			cout<<"Quit!";
			break;
		}
	
	}
}
