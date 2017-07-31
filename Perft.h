#ifndef PERFT_H
#define PERFT_H

#include "move.h"
#include "moveList.h"
#include "pawnsMoves.h"
#include "movesGenerator.h"

template <bool sideToMove>
signed long long PerftPromotion(const ChessBoard &board, AllMoves &allMoves, int depth);

template <bool sideToMove>
signed long long Perft(const ChessBoard &board, AllMoves &allMoves, int depth);


template <bool sideToMove, class M>
signed long long iterateOverMoves(const ChessBoard &board, MoveList<M> &moveList, int depth) {

	signed long long nodes = 0;

	M* moves = moveList.getMoves();
    while(moveList.hasNextMove(moves))  {

    	M move = *moves++;
		ChessBoard copy = board.makeMove<sideToMove>(move);

		if(isSquareAttacked<!sideToMove>(copy.piecesByType<sideToMove>(KING), copy)) {
			continue;
		}
		AllMoves allMoves;
		generateMoves<!sideToMove>(copy, allMoves);
		generatePawnsMoves<!sideToMove>(copy, allMoves);
		nodes += Perft<!sideToMove>(copy, allMoves, depth);
    }
    return nodes;
}


template <bool sideToMove>
signed long long Perft(const ChessBoard &board, AllMoves &allMoves, int depth) {

	depth--;

    signed long long nodes = 0;

    if(depth) {


    	nodes += iterateOverMoves<sideToMove, Move>(board, allMoves.moves, depth);
    	nodes += iterateOverMoves<sideToMove, Capture>(board, allMoves.captures, depth);
    	nodes += iterateOverMoves<sideToMove, Promotion>(board, allMoves.promotions, depth);
    	nodes += iterateOverMoves<sideToMove, PromotionCapture>(board, allMoves.promotionCaptures, depth);

    } else {

    	Move* moves = allMoves.moves.getMoves();
        while(allMoves.moves.hasNextMove(moves))  {
        	Move move = *moves++;
			if(isMoveLegal<!sideToMove>(board, move)) {
				nodes++;
			}
        }

        Capture* captures = allMoves.captures.getMoves();

    	while(allMoves.captures.hasNextMove(captures)) {
    		Capture move = *captures++;

    		if(isMoveLegal<!sideToMove>(board, move)) {
    			nodes++;
    		}

//    		ChessBoard copy = board.makeMoveLight<sideToMove>(move);
//
//    		if(!isSquareAttacked<!sideToMove>(copy.piecesByType<sideToMove>(KING), copy)) {
//				nodes++;
//    		}


        }
    }

    return nodes;
}

//template <bool sideToMove>
//signed long long PerftPromotion(const ChessBoard &board, MovesList &move_list, int depth) {
//
//	depth--;
//
//    signed long long nodes = 0;
//
//    Move* moves = move_list.getPromotions();
//
//    while(move_list.hasNextPromotion(moves))  {
//    	Move move = *moves++;
//        ChessBoard copy = board.makePromotion(move);
//
//        if(isSquareAttacked<!sideToMove>(copy.opositeKing(), copy)) {
//        	continue;
//        }
//
//        if(depth) {
//        	MovesList new_move_list;
//            generateMovesPromotion<!sideToMove>(copy, new_move_list);
//            generatePawnsMoves<!sideToMove>(copy, new_move_list);
//        	nodes += PerftPromotion<!sideToMove>(copy, new_move_list, depth);
//        } else {
//        	nodes++;
//        }
////        printChessBoard(copy);
//    }
//
//    moves = move_list.getMoves();
//
//
//    while(move_list.hasNextMove(moves))  {
//    	Move move = *moves++;
//        ChessBoard copy = board.makeMove(move);
//
//        if(isSquareAttacked<!sideToMove>(copy.opositeKing(), copy)) {
//        	continue;
//        }
//
//        if(depth) {
//        	MovesList new_move_list;
//            generateMovesPromotion<!sideToMove>(copy, new_move_list);
//            generatePawnsMoves<!sideToMove>(copy, new_move_list);
//        	nodes += PerftPromotion<!sideToMove>(copy, new_move_list, depth);
//        } else {
//        	nodes++;
//        }
////        printChessBoard(copy);
//    }
//
//    Capture* captures = move_list.getCaptures();
//
//	while(move_list.hasNextCapture(captures)) {
//		Capture move = *captures++;
//        ChessBoard copy = board.makeCapture(move);
//
//        if(isSquareAttacked<!sideToMove>(copy.opositeKing(), copy)) {
//        	continue;
//        }
//
//        if(depth) {
//        	MovesList new_move_list;
//			generateMovesPromotion<!sideToMove>(copy, new_move_list);
//			generatePawnsMoves<!sideToMove>(copy, new_move_list);
//			nodes += PerftPromotion<!sideToMove>(copy, new_move_list, depth);
//		} else {
//			nodes++;
//		}
//      //  printChessBoard(copy);
//    }
//    return nodes;
//}


#endif
