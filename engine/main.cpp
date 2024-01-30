#include "Board.hpp"
#include "Eval.hpp"
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
	Eval *evaluator = new Eval();
	Board *board = new Board(moveGen);

	cin >> input;
	while (input != "quit") {

		if (input == "eval") {
			int tmp;
			double eval;
			Move bestMove = evaluator->getBestMove(board, 4, eval, tmp);

			cout << BOLD << "Best Move:\n" << UNBOLD;
			cout << TO_ALGEBRA(bestMove.from()) << TO_ALGEBRA(bestMove.to()) << endl;

			cout << BOLD << "Eval:\n" << UNBOLD;
			cout << eval << endl;
		} else {
			if (input == "reset") {
				board->setStartingPos();
			} else {
				board->movePiece(input);
			}
			board->genMoves(true);
			board->print(white);
			board->printMoves();
		}

		cin >> input;
	}
	return 0;
}