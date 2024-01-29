#include "Board.hpp"
#include "MoveGen.hpp"
#include "includes.hpp"

using namespace std;

int main() {
	string input;

	MoveGen *moveGen = new MoveGen();
	Board *board = new Board(moveGen);

	cin >> input;
	while (input != "quit") {
		board->movePiece(input);
		board->genMoves(moveGen);
		board->print(white);
		board->printMoves();
		cin >> input;
	}
	return 0;
}