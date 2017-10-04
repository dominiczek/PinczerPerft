#include <iostream>
#include <time.h>

#include "chessboard.h"
#include "fenParser.h"
#include "Perft.h"

using namespace std;



void run() {
	ChessBoard board;
	board.reset();

	bool sideToMove = parseFen(board, "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq -");
//	bool sideToMove = parseFen(board, "rnbqkbnr/pppp1ppp/8/4p3/3P4/8/PPP1PPPP/RNBQKBNR w KQkq -");
//	bool sideToMove = parseFen(board, "rnbqkbnr/pppp1ppp/8/4p3/3P4/8/PPP1PPPP/RNBQKBNR w KQkq -");
//	bool sideToMove = parseFen(board, "8/2p5/3p4/KP6/5p1k/8/4P1P1/8 w - -");
//	bool sideToMove = parseFen(board, "8/7p/p5pb/4k3/P1pPn3/8/P5PP/1rB2RK1 b - d3 0 28");
//	bool sideToMove = parseFen(board, "8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - -");//8 OK, 6 zle:(
	//  szach
//	bool sideToMove = parseFen(board, "8/2p5/3p4/KP5r/1R3p1k/6P1/4P3/8 b - -");
//	bool sideToMove = parseFen(board, "8/3K4/2p5/p2b2r1/5k2/8/8/1q6 b - -");//8 OK, OK !
	//parseFen(board, "r1bqkbnr/1ppp1ppp/p1n5/1B2p3/4P3/5N2/PPPP1PPP/RNBQK2R w KQkq e6");

//	parseFen(board, "8/PPP4k/8/8/8/8/4Kppp/8 w - -");//8 OK! OK:)
//	parseFen(board, "rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8");^


	//parseFen(board, "r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - - 0 10");

//	cout<<"Move: "<<board.sideToMove<<endl;b
//	cout<<"init EN "<<board.enPessantSqr<<endl;
	printChessBoard(board);

	//cout<< getFirstPiece(board->king[WHITE])<<endl;
	//4 - 197,281
	//5  4,865,609 - OK
	//6  119060324 - OK
	//	 249753873

    AllMoves allMoves;


    int level = 6;
    cout<<"Level: " << level <<endl;

    if(sideToMove) {
    	generateMoves<BLACK>(board, allMoves);
    	generatePawnsMoves<BLACK>(board, allMoves);
    	cout<<"Moves: "<<Perft<BLACK>(board, allMoves, level)<<endl;
    } else {
    	generateMoves<WHITE>(board, allMoves);
    	generatePawnsMoves<WHITE>(board, allMoves);
    	cout<<"Moves: "<<Perft<WHITE>(board, allMoves, level)<<endl;
    }


//	printBitMap(1+(1<<8), '1');
}

int main() {
	clock_t tStart = clock();

	run();

	cout<<"Time taken: "<< ( ( (double) (clock() - tStart)) / CLOCKS_PER_SEC) <<" s.";
	return 0;
}
