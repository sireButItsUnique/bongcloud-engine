#include "Eval.hpp"

double Eval::getEval(Board *board) {

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
	res -= 9 * __popcnt64(board->pieceBoards[pawnBlack]);

	// give slight edge to whoever's turn it is
	res += (board->turn ? -0.1 : 0.1);

	return res;
}

Move Eval::getBestMove(Board *board, int ply, float &timeLeft) {

	if (!ply) {
		return this->getEval(board);
	}

	board->genMoves();
	double eval = (board->turn ? 1 : -1) * numeric_limits<double>::infinity();
	for (Move &move : board->moves) {
		Board *newBoard = new Board(*board);
		newBoard->movePiece(move.from(), move.to());
	}
}