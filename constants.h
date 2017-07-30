#ifndef CONSTANTS_H
#define CONSTANTS_H

typedef int SQUARE_T;
typedef unsigned long long U64;
typedef int PIECE_T;
typedef int CASTLE_T;
typedef int MOVE_TYPE;


const bool WHITE = 0;
const bool BLACK = 1;

const PIECE_T KING = 0;
const PIECE_T QUEEN = 1;
const PIECE_T ROOK = 2;
const PIECE_T BISHOP = 3;
const PIECE_T KNIGHT = 4;
const PIECE_T PAWN = 5;

const CASTLE_T NO_CASTLE = 0;
const CASTLE_T WHITE_KING_SIDE = 1;
const CASTLE_T WHITE_QUEEN_SIDE = 2;
const CASTLE_T BLACK_KING_SIDE = 3;
const CASTLE_T BLACK_QUEEN_SIDE = 4;

const MOVE_TYPE MOVE = 1;
const MOVE_TYPE CAPTURE = 2;
const MOVE_TYPE PROMOTION = 3;

enum FIELD {A1=0, B1, C1, D1, E1, F1, G1, H1, A2, B2, C2, D2, E2, F2, G2, H2,
			A3, B3, C3, D3, E3, F3, G3, H3, A4, B4, C4, D4, E4, F4, G4, H4,
			A5, B5, C5, D5, E5, F5, G5, H5, A6, B6, C6, D6, E6, F6, G6, H6,
			A7, B7, C7, D7, E7, F7, G7, H7, A8, B8, C8, D8, E8, F8, G8, H8};

#endif
