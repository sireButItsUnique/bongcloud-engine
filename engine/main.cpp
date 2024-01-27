#include "Board.hpp"
#include "MoveGen.hpp"
#include "includes.hpp"

using namespace std;

int main() {
	string input;
	Board *board = new Board();

	vector<Move> moves;
	MoveGen *moveGen = new MoveGen();

	cin >> input;
	while (input != "quit") {
		board->movePiece(input);
		board->print("white");
		cin >> input;
	}
	return 0;
}