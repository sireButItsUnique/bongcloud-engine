#include "Board.hpp"
#include "MoveGen.hpp"
#include "includes.hpp"

using namespace std;

/*
NOTE NOTE NOTE NOTE
-cannot make a castle move on board
-cannot make a promotion move on board
*/
int main() {
	setlocale(LC_ALL, "en_US.utf8");
	string input;
	MoveGen *moveGen = new MoveGen();
	Board *board = new Board(moveGen);

	cin >> input;
	while (input != "quit") {

		if (input == "reset") {
			board->setStartingPos();
		} else {
			board->movePiece(input);
		}
		board->genMoves();
		board->print(white);
		board->printMoves();

		cin >> input;
	}
	return 0;
}