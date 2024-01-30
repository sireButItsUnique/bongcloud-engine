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

	// castling checks
	if (from == 0 || to == 0 || from == 3) {
		whiteKingCastle = false;
	}
	if (from == 7 || to == 7 || from == 3) {
		whiteQueenCastle = false;
	}
	if (from == 56 || to == 56 || from == 59) {
		blackKingCastle = false;
	}
	if (from == 63 || to == 63 || from == 59) {
		blackQueenCastle = false;
	}

	// white = false/evens, black = true/odds
	unsigned ll fromMask = 1ULL << from;
	unsigned ll toMask = 1ULL << to;
	for (int i = turn; i < 12; i += 2) {

		// rmv piece at from, add piece at to (color that's moving)
		if (pieceBoards[i] & fromMask) {
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

	// castling checks
	if (from == 0 || to == 0 || from == 3) {
		whiteKingCastle = false;
	}
	if (from == 7 || to == 7 || from == 3) {
		whiteQueenCastle = false;
	}
	if (from == 56 || to == 56 || from == 59) {
		blackKingCastle = false;
	}
	if (from == 63 || to == 63 || from == 59) {
		blackQueenCastle = false;
	}

	// white = false/evens, black = true/odds
	unsigned ll fromMask = 1ULL << from;
	unsigned ll toMask = 1ULL << to;

	for (int i = turn; i < 12; i += 2) {

		// rmv piece at from, add piece at to (color that's moving)
		if (pieceBoards[i] & fromMask) {
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
	attackBoards[color] = 0;
	moveGen->genKingMovesA(pieceBoards[kingWhite + color], colorBoards[color], attackBoards[color]);
	moveGen->genKnightMovesA(pieceBoards[knightWhite + color], colorBoards[color], attackBoards[color]);
	moveGen->genBishopMovesA(pieceBoards[bishopWhite + color], colorBoards[color], colorBoards[!color], attackBoards[color]);
	moveGen->genRookMovesA(pieceBoards[rookWhite + color], colorBoards[color], colorBoards[!color], attackBoards[color]);
	moveGen->genQueenMovesA(pieceBoards[queenWhite + color], colorBoards[color], colorBoards[!color], attackBoards[color]);
	moveGen->genPawnMovesA(pieceBoards[pawnWhite + color], color, colorBoards[color], colorBoards[!color], attackBoards[color]);
}

void Board::genMoves(MoveGen *moveGen) {
	auto start = chrono::high_resolution_clock::now();
	moves.clear();

	genAttackBoard(!turn, moveGen);
	moveGen->genKingMoves(pieceBoards[kingWhite + turn], colorBoards[turn], moves);
	moveGen->genKnightMoves(pieceBoards[knightWhite + turn], colorBoards[turn], moves);
	moveGen->genBishopMoves(pieceBoards[bishopWhite + turn], colorBoards[turn], colorBoards[!turn], moves);
	moveGen->genRookMoves(pieceBoards[rookWhite + turn], colorBoards[turn], colorBoards[!turn], moves);
	moveGen->genQueenMoves(pieceBoards[queenWhite + turn], colorBoards[turn], colorBoards[!turn], moves);
	moveGen->genPawnMoves(pieceBoards[pawnWhite + turn], turn, colorBoards[turn], colorBoards[!turn], moves);

	if (turn) {
		moveGen->genCastleMoves(turn, blackKingCastle, blackQueenCastle, colorBoards[turn], colorBoards[!turn], attackBoards[!turn], moves);
	} else {
		moveGen->genCastleMoves(turn, whiteKingCastle, whiteQueenCastle, colorBoards[turn], colorBoards[!turn], attackBoards[!turn], moves);
	}

	auto end = chrono::high_resolution_clock::now();
	double time_taken = chrono::duration_cast<chrono::nanoseconds>(end - start).count();
	time_taken *= 1e-9;
	cout << GREEN << "Generated " << moves.size() << " moves in " << fixed << time_taken << setprecision(9) << " secs\n" << UNCOLOR;
}

void Board::print(bool c) {
	int idx = 0;
	int sqr = 0;
	cout << BOLD << "Board:\n" << UNBOLD;
	for (int square = 63 * (!c); (c ? square < 64 : square >= 0); (c ? square++ : square--)) {

		// check if there's a piece
		cout << ((sqr % 2) ? "\x1B[100m" : "\x1B[107m");
		bool found = false;
		for (int i = 0; i < 12; i++) {
			if (pieceBoards[i] & (1ULL << square)) {
				cout << ((enumToChar[i] - 'z' > 0) ? "\x1B[34m" : "\x1B[36m") << BOLD;
				cout << enumToChar[i];
				cout << ' ';
				found = true;
				break;
			}
		}
		if (!found) {
			cout << "  ";
		}
		cout << UNCOLOR;

		if (idx % 8 == 7) {
			cout << '\n';
			sqr++;
		}
		idx++;
		sqr++;
	}
	cout << BOLD << "State:\n" << UNBOLD;
	cout << (turn ? "Black to play" : "White to play") << endl;
	cout << "White " << (whiteKingCastle ? "can" : "cannot") << " castle kingside\n";
	cout << "White " << (whiteQueenCastle ? "can" : "cannot") << " castle queenside\n";
	cout << "Black " << (blackKingCastle ? "can" : "cannot") << " castle kingside\n";
	cout << "Black " << (blackQueenCastle ? "can" : "cannot") << " castle queenside\n";
}

void Board::printMoves() {

	cout << BOLD << "Pseudolegal Moves:\n" << UNBOLD;
	for (int i = 0; i < moves.size(); i++) {
		if (moves[i].isCastle()) {
			cout << (moves[i].castleSide() ? "O-O-O" : "O-O");
		} else {
			cout << TO_ALGEBRA(moves[i].from()) << TO_ALGEBRA(moves[i].to());
			if (moves[i].isPromotion()) {
				cout << "(" << (moves[i].promotionPiece() ? "knight" : "queen") << ")";
			}
		}
		cout << " ";

		if (i % 8 == 7) {
			cout << endl;
		}
	}
	cout << endl;
}