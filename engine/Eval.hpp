#pragma once
#include "Board.hpp"
#include "includes.hpp"

class Eval {
public:
	Eval();

	/**
	 * @brief Get the CURRENT eval, not considering future moves
	 *
	 * @param board
	 * @return eval, negative = black advantage, positive = white advantage
	 */
	double getBoardEval(Board *);

	/**
	 * @brief Get the eval of the board, taking future moves into account
	 *
	 * @param board
	 * @param ply
	 * @param evaluated how many nodes evaluated so far
	 * @return eval, negative = black advantage, positive = white advantage
	 */
	double getBoardEvalRec(Board *, int, int &);

	/**
	 * @brief Get the best move
	 *
	 * @param board
	 * @param ply
	 * @param eval
	 * @param evaluated how many nodes evaluated so far
	 * @param debug print debug msgs
	 */
	Move getBestMove(Board *, int, double &, int &, bool = false);
};