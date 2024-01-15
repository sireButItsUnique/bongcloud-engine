#include <iostream>
#include <string>
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
void startingPos(unsigned ll bitboards[12]) {
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
}

void movePiece(string move, bool color, unsigned ll bitboards[12]) {
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
}

}; // namespace BitboardHelper

int main() {
	unsigned ll bitboards[12];
	BitboardHelper::startingPos(bitboards);

	string move;
	bool color = false;
	cout << bitboards[pawnWhite] << endl;
	while (cin >> move) {
		BitboardHelper::movePiece(move, color, bitboards);
		cout << bitboards[pawnWhite] << endl;
		cout << bitboards[rookWhite] << endl;
		color = !color;
	}
	return 0;
}