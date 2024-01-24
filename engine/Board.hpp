#pragma once
#include "MoveGen.hpp"
#include "includes.hpp"

class Board {
public:
	unsigned ll pieceBoards[12], colorBoards[2], attackBoards[2];
	bool whiteQueenCastle, whiteKingCastle, blackQueenCastle, blackKingCastle;
	bool turn;
	vector<Move> moves;

	// debug only
	string enumToChar = "pPnNbBrRqQkK";

	Board();

	/**
	 * @brief sets bitboards to standard starting pos
	 */
	void setStartingPos();

	/**
	 * @brief move piece in standard piece bitboards
	 *
	 * @param move algebraic chess notation of piece movement (e.g. e2e4)
	 */
	void movePiece(string);

	/**
	 * @brief move piece in standard piece bitboards
	 *
	 * @param from int representing the square piece is on
	 * @param to int representing the square piece will move to
	 */
	void movePiece(int, int);

	/**
	 * @brief generates all possible moves for the current board
	 *
	 */
	void genMoves();

	/**
	 * @brief debug only, prints the board
	 *
	 */
	void print(bool = true);
};