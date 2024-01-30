#pragma once
#include "Board.cpp"
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
	double getEval(Board *);

	/**
	 * @brief Get the best move
	 *
	 * @param board
	 * @param ply
	 * @param timeLeft
	 */
	Move getBestMove(Board *, int, float &);
};