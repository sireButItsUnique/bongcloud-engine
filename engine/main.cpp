#include "Board.hpp"
#include "MoveGen.hpp"
#include "includes.hpp"

using namespace std;

int main() {
	string input;
	Board *board = new Board();
	board->print(false);
	unsigned ll tmp[12];
	vector<Move> moves;
	MoveGen::genKnightMoves(0, tmp, moves);

	while (input != "quit") {
		cin >> input;
		board->movePiece(input);
		board->print(false);
	}
	return 0;
}