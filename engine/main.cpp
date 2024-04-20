#include "Board.hpp"
#include "Eval.hpp"
#include "MoveGen.hpp"
#include "includes.hpp"

using namespace std;

int main() {
	string input;
	MoveGen *moveGen = new MoveGen();
	Eval *evaluator = new Eval();
	Board *board = new Board(moveGen);

	cin >> input;
	while (true) {
		if (input == "quit") {
			return 0;
		}

		// lichess interface
		if (input == "lichessmove") {
			string move;
			cin >> move;
			board->movePiece(move);
		} else if (input == "lichesseval") {
			int tmp, ply = 5;
			double eval;
			Move bestMove = evaluator->getBestMove(board, ply, eval, tmp);
			if (bestMove.from() == 16 && bestMove.to() == 16) {
				cout << "enginemove itFuckedItself " << eval << " " << endl;
			} else {
				cout << "enginemove " << bestMove.toAlgebra(false, board->turn) << " " << eval << " " << endl;
			}
		} else if (input == "lichessskip") {
			continue;
		}

		// human interface
		else {
			if (input == "eval") {
				int tmp, ply;
				double eval;
				cin >> ply;

				if (ply) {
					Move bestMove = evaluator->getBestMove(board, ply, eval, tmp, true);

					cout << BOLD << "Best Move:\n" << UNBOLD;
					cout << bestMove.toAlgebra(true) << endl;
				} else {
					eval = evaluator->getBoardEval(board);
				}

				cout << BOLD << "Eval:\n" << UNBOLD;
				cout << eval << endl;
			} else if (input == "print") {
				board->print(white);
			} else if (input == "printmoves") {
				board->genMoves();
				board->printMoves();
				cout << board->colorBoards[white] << " " << board->colorBoards[black] << endl;
			} else if (input == "setpos") {
				// analyze this game K4b2/3P4/8/8/8/P7/8/7k b - - 0 1
				string fen = "k6Q/pp6/q2r4/8/8/8/6NN/6NK b - - 0 1";
				board->parseFEN(fen);
			} else {
				if (input == "reset") {
					board->setStartingPos();
				} else {
					Move *moveObj = new Move(input);
					board->movePiece(moveObj);
				}
				board->genMoves(true);
				board->print(white);
			}
		}

		cin >> input;
	}
	return 1;
}