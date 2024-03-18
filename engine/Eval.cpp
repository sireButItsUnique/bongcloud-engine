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

	return res;
}

double Eval::finishCaptures(Board *board, double eval, double alpha, double beta, int &evaluated) {
	evaluated++;

	// set up board + eval
	board->genMoves();

	// stalemate check
	if (!board->moves.size()) {
		if (!board->inCheck(board->turn)) {
			eval = 0;
		} else {
			eval = (board->turn ? 1 : -1) * numeric_limits<double>::infinity();
		}
		return eval;
	}

	// treat cur position as valid end node & prune accordingly
	if (board->turn) {
		if (eval <= alpha) {
			return eval;
		}
		beta = min(eval, beta);
	} else {
		if (eval >= beta) {
			return eval;
		}
		alpha = max(eval, alpha);
	}

	// order all moves by evaling them right now (iterative deepening)
	vector<pair<float, Board *>> moves;
	for (int i = 0; i < board->moves.size(); i++) {

		// contains capture
		if (board->colorBoards[!board->turn] & (1ULL << board->moves[i].to())) {
			Board *newBoard = new Board(*board);
			newBoard->movePiece(board->moves[i].from(), board->moves[i].to());
			moves.push_back({getBoardEval(newBoard), newBoard});
		}
	}
	sort(moves.begin(), moves.end());

	// search all moves
	if (board->turn) {

		// make it good for black -> start w/ worst moves
		for (int i = 0; i < moves.size(); i++) {
			eval = min(eval, finishCaptures(moves[i].second, moves[i].first, alpha, beta, evaluated));

			// prune if move is too good -> white has a better move last ply
			if (eval <= alpha) {
				return eval;
			}
			beta = min(beta, eval);
		}
	} else {

		// make it good for white -> start w/ best ones
		for (int i = moves.size() - 1; i >= 0; i--) {
			eval = max(eval, finishCaptures(moves[i].second, moves[i].first, alpha, beta, evaluated));

			// prune if move is too good -> black has a better move last ply
			if (eval >= beta) {
				return eval;
			}
			alpha = max(alpha, eval);
		}
	}
	return eval;
}

double Eval::getBoardEvalRec(Board *board, int ply, double alpha, double beta, int &evaluated) {

	// return heurestic eval
	evaluated++;
	if (ply <= 0) {
		return finishCaptures(board, getBoardEval(board), alpha, beta, evaluated);
	}

	// set up board + eval
	board->genMoves();
	double eval = (board->turn ? 1 : -1) * numeric_limits<double>::infinity();

	// stalemate check
	if (!board->moves.size()) {
		if (!board->inCheck(board->turn)) {
			eval = 0;
		}
		return eval;
	}

	// order all moves by evaling them right now (iterative deepening)
	vector<pair<float, Board *>> moves(board->moves.size());
	for (int i = 0; i < board->moves.size(); i++) {
		Board *newBoard = new Board(*board);
		newBoard->movePiece(board->moves[i].from(), board->moves[i].to());
		moves[i] = {getBoardEval(newBoard), newBoard};
	}
	sort(moves.begin(), moves.end());

	// search all moves
	if (board->turn) {

		// make it good for black -> start w/ worst moves
		for (int i = 0; i < moves.size(); i++) {
			eval = min(eval, getBoardEvalRec(moves[i].second, ply - 1, alpha, beta, evaluated));

			// prune if move is too good -> white has a better move last ply
			if (eval <= alpha) {
				return eval;
			}
			beta = min(beta, eval);
		}
	} else {

		// make it good for white -> start w/ best ones
		for (int i = moves.size() - 1; i >= 0; i--) {
			eval = max(eval, getBoardEvalRec(moves[i].second, ply - 1, alpha, beta, evaluated));

			// prune if move is too good -> black has a better move last ply
			if (eval >= beta) {
				return eval;
			}
			alpha = max(alpha, eval);
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

	// stalemate check not needed bc game is over if no more moves

	// branch out to all moves
	double alpha = -1 * numeric_limits<double>::infinity();
	double beta = numeric_limits<double>::infinity();
	for (Move &move : board->moves) {
		Board *newBoard = new Board(*board);
		newBoard->movePiece(move.from(), move.to());

		// make it good for black
		if (board->turn) {
			double moveEval = getBoardEvalRec(newBoard, ply - 1, alpha, beta, evaluated);
			if (eval >= moveEval) {
				eval = moveEval;
				res = move;
			}
		}

		// make it good for white
		else {
			double moveEval = getBoardEvalRec(newBoard, ply - 1, alpha, beta, evaluated);
			if (eval <= moveEval) {
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