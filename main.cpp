//============================================================================
// Name        :
// Author      :
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <time.h>


#include "chessboard.h"
#include "fen_parser.h"
#include "perft.h"
#include "cache.h"


using namespace std;

void run() {
	ChessBoard board;
	board.reset();

	bool sideToMove = FEN_PARSER::parseFen(board, "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq -");
//
//	board = FEN_PARSER::makeMovesList(sideToMove, board, "d2d4 g8f6 c2c4 c7c5 d4d5 e7e6 b1c3 e6d5 c4d5 d7d6 g1f3 g7g6 c1g5 f8g7 f3d2 h7h6 g5h4 g6g5 h4g3 f6h5 d2c4 h5g3 h2g3 e8g8");
//
//	printChessBoard(board);

//	bool sideToMove = parseFen(board, "r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - -");
//	bool sideToMove = parseFen(board, "rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ -");
//	bool sideToMove = FEN_PARSER::parseFen(board, "8/PPP4k/8/8/8/8/4Kppp/8 w - - 0 1");
//	bool sideToMove = FEN_PARSER::parseFen(board, "r3k2r/p1p1qpb1/bn1ppnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/1K1R3R b kq -");
//	bool sideToMove = FEN_PARSER::parseFen(board, "r3k2r/p1p1qpb1/1n1ppnp1/3PN3/1p2P3/2N2Q1p/PPPBbPPP/K2R3R b kq -");
//	bool sideToMove = FEN_PARSER::parseFen(board, "r3k2r/p1p1qpb1/1n1ppnp1/3PN3/1p2P3/2N2Q1p/PPPB1PPP/K2b3R w kq -");
//	bool sideToMove = parseFen(board, "4k3/8/8/2b5/8/8/3P4/4K3 w -");
//	bool sideToMove = parseFen(board, "4kb2/8/8/8/8/8/3P4/4K3 b -");
//	bool sideToMove = parseFen(board, "rnbqk3/pppppppp/7P/8/8/8/PPPPPPP1/RNBQKBNR w");
//	bool sideToMove = parseFen(board, "rnbq1bnr/pppp1kpp/4p3/5P2/8/5Q2/PPPP1PPP/RNB1KBNR w KQkq -");
//	bool sideToMove = parseFen(board, "rnbqk2r/ppppppBp/6pb/8/8/1P6/P1PPPPPP/RN1QKBNR b KQkq -");
//	bool sideToMove = parseFen(board, "rnbqkbnr/pppp1ppp/8/4p3/3P4/8/PPP1PPPP/RNBQKBNR w KQkq -");
//	bool sideToMove = parseFen(board, "8/2p5/3p4/KP6/5p1k/8/4P1P1/8 w - -");
//	bool sideToMove = FEN_PARSER::parseFen(board, "8/7p/p5pb/4k3/P1pPn3/8/P5PP/1rB2RK1 b - d3 0 28");
//	bool sideToMove = parseFen(board, "8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - -");//9 OK :)
//	bool sideToMove = parseFen(board, "8/2p5/3p4/KP5r/1R3p1k/6P1/4P3/8 b - -");
//	bool sideToMove = parseFen(board, "8/3K4/2p5/p2b2r1/5k2/8/8/1q6 b - -");//8 OK, OK !
	//parseFen(board, "r1bqkbnr/1ppp1ppp/p1n5/1B2p3/4P3/5N2/PPPP1PPP/RNBQK2R w KQkq e6");
//	bool sideToMove = FEN_PARSER::parseFen(board, "2Q5/PP5k/8/8/8/8/4Kppp/8 b - -");//8 OK! OK:)
//	bool sideToMove = FEN_PARSER::parseFen(board, "r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - - 0 10");

	//parseFen(board, "r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - - 0 10");

	printChessBoard(board);

    AllMoves allMoves;

    board.generateKey(sideToMove);

    int level = 7;

    const bool useCache = false;

    cout<<"Level: " << level <<endl;

    U64 result = 0;

    if(sideToMove) {

        U64 kingMoves = setCheckAndPinners<WHITE>(board);
        if(board.isCheck()) {
        	result = Perft<BLACK, CHECK, useCache>(board, kingMoves, allMoves, level);
        } else {
        	result = Perft<BLACK, NO_CHECK, useCache>(board, kingMoves, allMoves, level);
        }
    } else {
    	U64 kingMoves = setCheckAndPinners<BLACK>(board);
    	if(board.isCheck()) {
    		result = Perft<WHITE, CHECK, useCache>(board, kingMoves, allMoves, level);
		} else {
			result = Perft<WHITE, NO_CHECK, useCache>(board, kingMoves, allMoves, level);
		}
    }

    cout<<"Moves: "<<result<<endl;
}

int main() {

	ZOBRIST::Initialize();
	MOVE_PROVIDER::Initialize();
	int CACHE_SIZE = 2*1024*1024 - 1;

//	int CACHE_SIZE = 1024*1024;

	CACHE::Initialize(CACHE_SIZE*1024/12); //16 B

	clock_t tStart = clock();

	run();

	cout<<"Time taken: "<< ( ( (double) (clock() - tStart)) / CLOCKS_PER_SEC) <<" s.";

	CACHE::CleanUp();

	return 0;
}
