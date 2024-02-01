#include "Eval.hpp"

Eval::Eval() {
	return;
}

double Eval::getBoardEval(Board *board) {

	// king dead
	if (!board->pieceBoards[kingBlack]) {
		return numeric_limits<double>::infinity();
	}
	if (!board->pieceBoards[kingWhite]) {
		return -1 * numeric_limits<double>::infinity();
	}

	// simple sum piece val
	double res = 0;
	res += 1 * __popcnt64(board->pieceBoards[pawnWhite]);
	res -= 1 * __popcnt64(board->pieceBoards[pawnBlack]);
	res += 3 * __popcnt64(board->pieceBoards[knightWhite]);
	res -= 3 * __popcnt64(board->pieceBoards[knightBlack]);
	res += 3.5 * __popcnt64(board->pieceBoards[bishopWhite]);
	res -= 3.5 * __popcnt64(board->pieceBoards[bishopBlack]);
	res += 5 * __popcnt64(board->pieceBoards[rookWhite]);
	res -= 5 * __popcnt64(board->pieceBoards[rookBlack]);
	res += 9 * __popcnt64(board->pieceBoards[queenWhite]);
	res -= 9 * __popcnt64(board->pieceBoards[queenBlack]);

	// give slight edge to whoever's turn it is
	res += (board->turn ? -0.1 : 0.1);

	return res;
}

double Eval::getBoardEvalRec(Board *board, int ply, int &evaluated) {

	evaluated++;
	if (ply <= 0) {
		return getBoardEval(board);
	}

	// set up board + eval
	board->genMoves();
	double eval = (board->turn ? 1 : -1) * numeric_limits<double>::infinity();

	// stalemate check
	if (!board->moves.size()) {
		if (board->inCheck(board->turn)) {
			eval = 0;
		}
		return eval;
	}

	// branch out to all moves
	for (Move &move : board->moves) {
		Board *newBoard = new Board(*board);
		newBoard->movePiece(move.from(), move.to());

		// make it good for black
		if (board->turn) {
			eval = min(eval, getBoardEvalRec(newBoard, ply - 1, evaluated));
		}

		// make it good for white
		else {
			eval = max(eval, getBoardEvalRec(newBoard, ply - 1, evaluated));
		}
	}
	return eval;
}

Move Eval::getBestMove(Board *board, int ply, double &eval, int &evaluated, bool debug) {

	// set up board + eval
	auto start = chrono::high_resolution_clock::now();
	evaluated = 1;
	board->genMoves();
	eval = (board->turn ? 1 : -1) * numeric_limits<double>::infinity();
	Move res = Move(16, 16);

	// stalemate check
	if (!board->moves.size()) {
		if (board->inCheck(board->turn)) {
			eval = 0;
		}
	}

	// branch out to all moves
	for (Move &move : board->moves) {
		Board *newBoard = new Board(*board);
		newBoard->movePiece(move.from(), move.to());

		// make it good for black
		if (board->turn) {
			double moveEval = getBoardEvalRec(newBoard, ply - 1, evaluated);
			if (eval > moveEval) {
				eval = moveEval;
				res = move;
			}
		}

		// make it good for white
		else {
			double moveEval = getBoardEvalRec(newBoard, ply - 1, evaluated);
			if (eval < moveEval) {
				eval = moveEval;
				res = move;
			}
		}
	}

	if (debug) {
		auto end = chrono::high_resolution_clock::now();
		double time_taken = chrono::duration_cast<chrono::nanoseconds>(end - start).count();
		time_taken *= 1e-9;
		cout << GREEN << "Evaluated " << evaluated << " positions in " << fixed << time_taken << setprecision(9) << " secs\n" << UNCOLOR;
	}
	return res;
}