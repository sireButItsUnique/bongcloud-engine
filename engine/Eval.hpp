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
	 * @brief returns eval after finishing all captures to avoid miscaulculations
	 *
	 * @param board
	 * @param alpha
	 * @param beta
	 * @param evaluated
	 * @return double
	 */
	double finishCaptures(Board *, double, double, int &);

	/**
	 * @brief Get the eval of the board, taking future moves into account
	 *
	 * @param board
	 * @param ply
	 * @param alpha
	 * @param beta
	 * @param evaluated how many nodes evaluated so far
	 * @return eval, negative = black advantage, positive = white advantage
	 */
	double getBoardEvalRec(Board *, int, double, double, int &);

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