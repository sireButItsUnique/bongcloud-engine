#include "Board.hpp"
#include "MoveGen.hpp"
#include "includes.hpp"

using namespace std;

int main() {
	string input;
	Board *board = new Board();
	board->print(false);

	vector<Move> moves;
	MoveGen *moveGen = new MoveGen();
	moveGen->genBishopMoves(68719476736, 0, 0, moves);
	// for (int i = 0; i < moves.size(); i++) {
	// 	cout << TO_ALGEBRA(moves[i].first) << TO_ALGEBRA(moves[i].second) << endl;
	// }

	// cin >> input;
	// while (input != "quit") {
	// 	board->movePiece(input);
	// 	board->print(false);
	// 	cin >> input;
	// }
	return 0;
}