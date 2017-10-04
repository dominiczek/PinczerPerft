#ifndef CONSTANTS_H
#define CONSTANTS_H

typedef int SQUARE_T;
typedef unsigned long long U64;
typedef int PIECE_T;
typedef int CASTLE_T;
typedef short CHECK_T;

const int SQUARES_COUNT = 64;

const bool WHITE = 0;
const bool BLACK = 1;

const PIECE_T KING = 0;
const PIECE_T QUEEN = 1;
const PIECE_T ROOK = 2;
const PIECE_T BISHOP = 3;
const PIECE_T KNIGHT = 4;
const PIECE_T PAWN = 5;

const CASTLE_T NO_CASTLE = 0;
const CASTLE_T KING_SIDE = 1;
const CASTLE_T QUEEN_SIDE = 2;
const CASTLE_T BOTH_SIDE = 3;

const CHECK_T NO_CHECK = 0;
const CHECK_T CHECK = 1;
const CHECK_T DOUBLE_CHECK = 2;

const U64 FULL_FIRST_RANK = 255;

const U64 EN_PASSANT_RANK[] = {FULL_FIRST_RANK << 32 ,FULL_FIRST_RANK << 24};

enum DIRECTION {N=0, NE, E, SE, S, SW, W, NW};

const U64 A1=1;
const U64 A2=(A1<<8);
const U64 A3=(A2<<8);
const U64 A4=(A3<<8);
const U64 A5=(A4<<8);
const U64 A6=(A5<<8);
const U64 A7=(A6<<8);
const U64 A8=(A7<<8);

const U64 B1=(2);
const U64 B2=(B1<<8);
const U64 B3=(B2<<8);
const U64 B4=(B3<<8);
const U64 B5=(B4<<8);
const U64 B6=(B5<<8);
const U64 B7=(B6<<8);
const U64 B8=(B7<<8);

const U64 C1=(4);
const U64 C2=(C1<<8);
const U64 C3=(C2<<8);
const U64 C4=(C3<<8);
const U64 C5=(C4<<8);
const U64 C6=(C5<<8);
const U64 C7=(C6<<8);
const U64 C8=(C7<<8);

const U64 D1=(8);
const U64 D2=(D1<<8);
const U64 D3=(D2<<8);
const U64 D4=(D3<<8);
const U64 D5=(D4<<8);
const U64 D6=(D5<<8);
const U64 D7=(D6<<8);
const U64 D8=(D7<<8);

const U64 E1=(16);
const U64 E2=(E1<<8);
const U64 E3=(E2<<8);
const U64 E4=(E3<<8);
const U64 E5=(E4<<8);
const U64 E6=(E5<<8);
const U64 E7=(E6<<8);
const U64 E8=(E7<<8);

const U64 F1=(32);
const U64 F2=(F1<<8);
const U64 F3=(F2<<8);
const U64 F4=(F3<<8);
const U64 F5=(F4<<8);
const U64 F6=(F5<<8);
const U64 F7=(F6<<8);
const U64 F8=(F7<<8);

const U64 G1=(64);
const U64 G2=(G1<<8);
const U64 G3=(G2<<8);
const U64 G4=(G3<<8);
const U64 G5=(G4<<8);
const U64 G6=(G5<<8);
const U64 G7=(G6<<8);
const U64 G8=(G7<<8);

const U64 H1=(128);
const U64 H2=(H1<<8);
const U64 H3=(H2<<8);
const U64 H4=(H3<<8);
const U64 H5=(H4<<8);
const U64 H6=(H5<<8);
const U64 H7=(H6<<8);
const U64 H8=(H7<<8);

const U64 kingStartSqr[] = {E1, E8};
const U64 rookStartSqrA[] = {A1, A8};
const U64 rookStartSqrH[] = {H1, H8};
const U64 kingShortCastleSqr[] = {G1, G8};
const U64 kingLongCastleSqr[] = {C1, C8};
const U64 rookShortCastleSqr[] = {F1, F8};
const U64 rookLongCastleSqr[] = {D1, D8};

const U64 shortCastleBlockers[] = {F1|G1, F8|G8};
const U64 longCastleBlockers[] = {D1|C1|B1, D8|C8|B8};



#endif
