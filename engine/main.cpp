#include <iostream>
#include <string>
#include <vector>
#define ll long long
using namespace std;

#define pawnWhite 0
#define pawnBlack 1
#define knightWhite 2
#define knightBlack 3
#define bishopWhite 4
#define bishopBlack 5
#define rookWhite 6
#define rookBlack 7
#define queenWhite 8
#define queenBlack 9
#define kingWhite 10
#define kingBlack 11

#define TO_SQUARE(col, row) 8 * (row - '1') + 7 - (col - 'a')
#define TO_ALGEBRA(sq)                                                         \
	string(1, 'a' + (7 - (sq % 8))) + string(1, '1' + (sq / 8))

namespace BitboardHelper {

/**
 * @brief sets bitboards to standard starting pos
 *
 * @param bitboards
 */
void startingPos(unsigned ll bitboards[12], unsigned ll colorBoards[2]) {
	bitboards[pawnWhite] = 0xff00;
	bitboards[pawnBlack] = 0xff000000000000;
	bitboards[knightWhite] = 0x42;
	bitboards[knightBlack] = 0x42000000000000;
	bitboards[bishopWhite] = 0x24;
	bitboards[bishopBlack] = 0x24000000000000;
	bitboards[rookWhite] = 0x81;
	bitboards[rookBlack] = 0x8100000000000000;
	bitboards[queenWhite] = 0x10;
	bitboards[queenBlack] = 0x1000000000000000;
	bitboards[kingWhite] = 0x8;
	bitboards[kingBlack] = 0x800000000000000;
	colorBoards[0] = 0xffff;
	colorBoards[1] = 0xffff000000000000;
}

/**
 * @brief move piece in standard piece bitboards
 *
 * @param move algebraic chess notation of piece movement (e.g. e2e4)
 * @param color white = false, black = true
 * @param bitboards output
 * @param colorboards output
 */
void movePiece(string move, bool color, unsigned ll bitboards[12],
			   unsigned ll colorboards[2]) {
	int from = TO_SQUARE(move[0], move[1]);
	int to = TO_SQUARE(move[2], move[3]);

	// white = false/evens, black = true/odds
	unsigned ll fromMask = 1ULL << from;
	unsigned ll toMask = 1ULL << to;
	for (int i = color; i < 12; i += 2) {

		// rmv piece at from, add piece at to (color that's moving)
		if (bitboards[i] != (bitboards[i] & ~fromMask)) {
			bitboards[i] &= ~fromMask;
			bitboards[i] |= toMask;
			break;
		}
	}
	for (int i = !color; i < 12; i += 2) {

		// rmv piece at to (oping color)
		bitboards[i] &= ~toMask;
	}

	// move piece in color boards
	colorboards[color] &= ~fromMask;
	colorboards[color] |= toMask;
	colorboards[!color] &= ~toMask;
}

/**
 * @brief move piece in standard piece bitboards
 *
 * @param from int representing the square piece is on
 * @param to int representing the square piece will move to
 * @param color white = false, black = true
 * @param bitboards output
 * @param coloboards output
 */
void movePiece(int from, int to, bool color, unsigned ll bitboards[12],
			   unsigned ll colorboards[2]) {

	// white = false/evens, black = true/odds
	unsigned ll fromMask = 1ULL << from;
	unsigned ll toMask = 1ULL << to;
	for (int i = color; i < 12; i += 2) {

		// rmv piece at from, add piece at to (color that's moving)
		if (bitboards[i] != (bitboards[i] & ~fromMask)) {
			bitboards[i] &= ~fromMask;
			bitboards[i] |= toMask;
			break;
		}
	}
	for (int i = !color; i < 12; i += 2) {

		// rmv piece at to (oping color)
		bitboards[i] &= ~toMask;
	}

	// move piece in color boards
	colorboards[color] &= ~fromMask;
	colorboards[color] |= toMask;
	colorboards[!color] &= ~toMask;
}

void genKnightMoves(int pos, bool color, unsigned ll colorboards[2],
					vector<int> &res) {
	const int offsets[8] = {-17, -15, -10, -6, 6, 10, 15, 17};
	for (int i = 0; i < 8; i++) {
		unsigned ll newPos = pos + offsets[i];
		if (newPos < 0 || newPos > 63) {
			continue;
		}
		unsigned ll mask = 1 << newPos;
		if (colorboards[color] & mask) {
			continue;
		}
		res.push_back(newPos);
	}
}

}; // namespace BitboardHelper

int main() {
	unsigned ll pieceBoards[12], colorBoards[2], attackBoards[2];
	BitboardHelper::startingPos(pieceBoards, colorBoards);

	string move;
	bool color = false;
	cout << pieceBoards[pawnWhite] << endl;
	while (true) {
		cin >> move;
		if (move == "exit") {
			return 0;
		}
		BitboardHelper::movePiece(move, color, pieceBoards, colorBoards);
		cout << pieceBoards[pawnWhite] << endl;
		cout << pieceBoards[rookWhite] << endl;
		color = !color;
	}
	return 1;
}