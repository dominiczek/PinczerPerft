#include "fen_parser.h"

#include "chessboard.h"

using namespace std;

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

SQUARE_T FEN_PARSER::parseSquare(string squareCode) {
	char x = tolower(squareCode[0]) - 97;
	char y = squareCode[1] - 49;

	return y*8 + x;
}

U64 FEN_PARSER::parseSquareMap(string squareCode) {
	int sqrIndex = parseSquare(squareCode) - 1;
	return 2LL<<sqrIndex;
}


template <bool sideToMove>
ChessBoard FEN_PARSER::makeMove(const ChessBoard &board, string move) {

	move.shrink_to_fit();


	string fromStr = move.substr(0, 2);
	U64 from = FEN_PARSER::parseSquareMap(fromStr);

	PIECE_T piece = board.getPieceOnSquare2<sideToMove>(from);

	if(piece == KING) {
		if(move == "e1g1") {
			return board.makeShortCastle<WHITE>();
		}
		if(move == "e1c1") {
			return board.makeLongCastle<WHITE>();
		}
		if(move == "e8g8") {
			return board.makeShortCastle<BLACK>();
		}
		if(move == "e8c8") {
			return board.makeLongCastle<BLACK>();
		}
	}

	string toStr = move.substr(2, 4);
	U64 to = FEN_PARSER::parseSquareMap(toStr);

	if(move.size() == 5) {
		PIECE_T promotionePiece;
		switch(move[4]) {
			case 'Q': promotionePiece = QUEEN; break;
			case 'R': promotionePiece = ROOK; break;
			case 'B': promotionePiece = BISHOP; break;
			case 'N': promotionePiece = KNIGHT; break;
		};

		if(board.allPieces() & to) {
			PIECE_T captured = board.getPieceOnSquare<!sideToMove>(to);
			PromotionCapture capture(from, to, promotionePiece, captured);
			return board.makeMove<sideToMove>(capture);
		} else {
			Promotion move(from, to, promotionePiece);
			return board.makeMove<sideToMove>(move);
		}
	}

	if(piece == PAWN) {

		if(moveBackward<sideToMove>(to, 16) == from) {
			PawnMove moveTwSqr(PAWN, from, to, moveForward<sideToMove>(from, 8));
			return board.makeMove<sideToMove>(moveTwSqr);
		}
		if(board.enPessantSqr & to) {
			PawnCapture capture(PAWN, from, to, moveBackward<!sideToMove>(to, 8));
			return board.makeMove<sideToMove>(capture);
		}
		if(board.allPieces() & to) {
			PIECE_T captured = board.getPieceOnSquare<!sideToMove>(to);
			Capture capture(PAWN, from, to, captured);
			return board.makeMove<sideToMove>(capture);
		} else {
			Move move(PAWN, from, to);
			return board.makeMove<sideToMove>(move);
		}
	} else {
		if(board.allPieces() & to) {
			PIECE_T captured = board.getPieceOnSquare<!sideToMove>(to);
			Capture capture(piece, from, to, captured);
			return board.makeMove<sideToMove>(capture);
		} else {
			Move move(piece, from, to);
			return board.makeMove<sideToMove>(move);
		}
	}
}

ChessBoard FEN_PARSER::makeMove(bool &sideToMove, ChessBoard &board, string move) {

	if(sideToMove) {
		sideToMove=!sideToMove;
		return makeMove<BLACK>(board, move);
	} else {
		sideToMove=!sideToMove;
		return makeMove<WHITE>(board, move);
	}

}

ChessBoard FEN_PARSER::makeMovesList(bool &sideToMove, ChessBoard &board, string movesStr) {

	vector<string> moves = split(movesStr, ' ');

	for(auto it=moves.begin(); it!=moves.end(); ++it) {
		string moveStr = *it;

		board = makeMove(sideToMove, board, moveStr);
		cout<<"Move made ["<<moveStr<<"], ";
	}
	cout<<endl<<"Position set up."<<endl;
	return board;
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
		sideToMove = BLACK;
	} else {
		sideToMove = WHITE;
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
