#include "Board.hpp"

Board::Board(MoveGen *moveGen) {
	setStartingPos(moveGen);
}

void Board::setStartingPos(MoveGen *moveGen) {
	pieceBoards[pawnWhite] = 0xff00;
	pieceBoards[pawnBlack] = 0xff000000000000;
	pieceBoards[knightWhite] = 0x42;
	pieceBoards[knightBlack] = 0x4200000000000000;
	pieceBoards[bishopWhite] = 0x24;
	pieceBoards[bishopBlack] = 0x2400000000000000;
	pieceBoards[rookWhite] = 0x81;
	pieceBoards[rookBlack] = 0x8100000000000000;
	pieceBoards[queenWhite] = 0x10;
	pieceBoards[queenBlack] = 0x1000000000000000;
	pieceBoards[kingWhite] = 0x8;
	pieceBoards[kingBlack] = 0x800000000000000;
	colorBoards[0] = 0xffff;
	colorBoards[1] = 0xffff000000000000;
	genAttackBoard(white, moveGen);
	genAttackBoard(black, moveGen);
	turn = white;
	whiteKingCastle = true;
	whiteQueenCastle = true;
	blackKingCastle = true;
	blackQueenCastle = true;
}

void Board::movePiece(string move) {
	int from = TO_SQUARE(move[0], move[1]);
	int to = TO_SQUARE(move[2], move[3]);

	// white = false/evens, black = true/odds
	unsigned ll fromMask = 1ULL << from;
	unsigned ll toMask = 1ULL << to;
	for (int i = turn; i < 12; i += 2) {

		// rmv piece at from, add piece at to (color that's moving)
		if (pieceBoards[i] != (pieceBoards[i] & ~fromMask)) {
			pieceBoards[i] &= ~fromMask;
			pieceBoards[i] |= toMask;
			break;
		}
	}
	for (int i = !turn; i < 12; i += 2) {

		// rmv piece at to (oping color)
		pieceBoards[i] &= ~toMask;
	}

	// move piece in color boards
	colorBoards[turn] &= ~fromMask;
	colorBoards[turn] |= toMask;
	colorBoards[!turn] &= ~toMask;

	turn = !turn;
}

void Board::movePiece(int from, int to) {

	// white = false/evens, black = true/odds
	unsigned ll fromMask = 1ULL << from;
	unsigned ll toMask = 1ULL << to;

	for (int i = turn; i < 12; i += 2) {

		// rmv piece at from, add piece at to (color that's moving)
		if (pieceBoards[i] != (pieceBoards[i] & ~fromMask)) {
			pieceBoards[i] &= ~fromMask;
			pieceBoards[i] |= toMask;
			break;
		}
	}
	for (int i = !turn; i < 12; i += 2) {

		// rmv piece at to (oping color)
		pieceBoards[i] &= ~toMask;
	}

	// move piece in color boards
	colorBoards[turn] &= ~fromMask;
	colorBoards[turn] |= toMask;
	colorBoards[!turn] &= ~toMask;

	turn = !turn;
}

void Board::genAttackBoard(bool color, MoveGen *moveGen) {
	moveGen->genKingMovesA(pieceBoards[kingWhite + color], colorBoards[color], attackBoards[color]);
	moveGen->genKnightMovesA(pieceBoards[knightWhite + color], colorBoards[color], attackBoards[color]);
	moveGen->genBishopMovesA(pieceBoards[bishopWhite + color], colorBoards[color], colorBoards[!color], attackBoards[color]);
	moveGen->genRookMovesA(pieceBoards[rookWhite + color], colorBoards[color], colorBoards[!color], attackBoards[color]);
	moveGen->genQueenMovesA(pieceBoards[queenWhite + color], colorBoards[color], colorBoards[!color], attackBoards[color]);
	moveGen->genPawnMovesA(pieceBoards[pawnWhite + color], color, colorBoards[color], colorBoards[!color], attackBoards[color]);
}

void Board::genMoves(MoveGen *moveGen) {
	moves.clear();

	moveGen->genKingMoves(pieceBoards[kingWhite + turn], colorBoards[turn], moves);
	moveGen->genKnightMoves(pieceBoards[knightWhite + turn], colorBoards[turn], moves);
	moveGen->genBishopMoves(pieceBoards[bishopWhite + turn], colorBoards[turn], colorBoards[!turn], moves);
	moveGen->genRookMoves(pieceBoards[rookWhite + turn], colorBoards[turn], colorBoards[!turn], moves);
	moveGen->genQueenMoves(pieceBoards[queenWhite + turn], colorBoards[turn], colorBoards[!turn], moves);
	moveGen->genPawnMoves(pieceBoards[pawnWhite + turn], turn, colorBoards[turn], colorBoards[!turn], moves);
}

void Board::print(bool c) {
	int i = 0;
	for (int square = 63 * (!c); (c ? square < 64 : square >= 0); (c ? square++ : square--)) {

		// check if there's a piece
		bool found = false;
		for (int i = 0; i < 12; i++) {
			if (pieceBoards[i] & (1ULL << square)) {
				cout << enumToChar[i] << ' ';
				found = true;
				break;
			}
		}
		if (!found)
			cout << ". ";

		if (i % 8 == 7)
			cout << '\n';
		i++;
	}
}

void Board::printMoves() {
	for (int i = 0; i < moves.size(); i++) {
		cout << TO_ALGEBRA(moves[i].from()) << TO_ALGEBRA(moves[i].to()) << " ";
		if (i % 8 == 1) {
			cout << endl;
		}
	}
}