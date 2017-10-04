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
//	bool sideToMove = parseFen(board, "r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - -");


	//bool sideToMove = parseFen(board, "rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ -");


//	bool sideToMove = FEN_PARSER::parseFen(board, "8/PPP4k/8/8/8/8/4Kppp/8 w - - 0 1");
//	bool sideToMove = FEN_PARSER::parseFen(board, "r3k2r/p1p1qpb1/bn1ppnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/1K1R3R b kq -");

//	bool sideToMove = FEN_PARSER::parseFen(board, "r3k2r/p1p1qpb1/1n1ppnp1/3PN3/1p2P3/2N2Q1p/PPPBbPPP/K2R3R b kq -");

//	bool sideToMove = FEN_PARSER::parseFen(board, "r3k2r/p1p1qpb1/1n1ppnp1/3PN3/1p2P3/2N2Q1p/PPPB1PPP/K2b3R w kq -");


// 7 374190009323
// 7 374190005483 x

//	bool sideToMove = parseFen(board, "4k3/8/8/2b5/8/8/3P4/4K3 w -");
//	bool sideToMove = parseFen(board, "4kb2/8/8/8/8/8/3P4/4K3 b -");
//	bool sideToMove = parseFen(board, "rnbqk3/pppppppp/7P/8/8/8/PPPPPPP1/RNBQKBNR w");

	bool sideToMove = FEN_PARSER::parseFen(board, "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq -");


//	bool sideToMove = parseFen(board, "rnbq1bnr/pppp1kpp/4p3/5P2/8/5Q2/PPPP1PPP/RNB1KBNR w KQkq -");

//	bool sideToMove = parseFen(board, "rnbqk2r/ppppppBp/6pb/8/8/1P6/P1PPPPPP/RN1QKBNR b KQkq -");

//	bool sideToMove = parseFen(board, "rnbqkbnr/pppp1ppp/8/4p3/3P4/8/PPP1PPPP/RNBQKBNR w KQkq -");
//	bool sideToMove = parseFen(board, "8/2p5/3p4/KP6/5p1k/8/4P1P1/8 w - -");
//	bool sideToMove = FEN_PARSER::parseFen(board, "8/7p/p5pb/4k3/P1pPn3/8/P5PP/1rB2RK1 b - d3 0 28");



	//8OK, 9 Zle

//	bool sideToMove = parseFen(board, "8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - -");//9 OK :)
	//  szach
//	bool sideToMove = parseFen(board, "8/2p5/3p4/KP5r/1R3p1k/6P1/4P3/8 b - -");
//	bool sideToMove = parseFen(board, "8/3K4/2p5/p2b2r1/5k2/8/8/1q6 b - -");//8 OK, OK !
	//parseFen(board, "r1bqkbnr/1ppp1ppp/p1n5/1B2p3/4P3/5N2/PPPP1PPP/RNBQK2R w KQkq e6");

//	bool sideToMove = FEN_PARSER::parseFen(board, "2Q5/PP5k/8/8/8/8/4Kppp/8 b - -");//8 OK! OK:)
//	9 389736558441 v
//	9 389736558661 x


//	bool sideToMove = FEN_PARSER::parseFen(board, "r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - - 0 10");


	//parseFen(board, "r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - - 0 10");

//	cout<<"Move: "<<board.sideToMove<<endl;b
//	cout<<"init EN "<<board.enPessantSqr<<endl;

	printChessBoard(board);

	//cout<< getFirstPiece(board->king[WHITE])<<endl;
	//4 - 197,281
	//5  4,865,609 - OK
	//6  119060324 - OK
	//7 3195901860 - OK
	//8 84998978956 - OK
	//9 2439530234167 - OK
	//10 69352859712417 - OK

    AllMoves allMoves;

    board.generateKey(sideToMove);

    int level = 7;
//13166245948270
//13166245942927

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

//2238101974693

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