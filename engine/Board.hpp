#pragma once
#include "Move.hpp"
#include "MoveGen.hpp"
#include "includes.hpp"

class Board {
public:
	unsigned ll pieceBoards[12], colorBoards[2], attackBoards[2];
	bool whiteQueenCastle, whiteKingCastle, blackQueenCastle, blackKingCastle;

	/**
	 * @brief who's turn it is to move and therefore generate moves, white=false; black=true
	 *
	 */
	bool turn;
	vector<Move> moves;

	// debug only
	string enumToChar = "pPnNbBrRqQkK";

	Board(MoveGen *);

	/**
	 * @brief sets bitboards to standard starting pos
	 *
	 * @param moveGen helper class used to generate pseudo legal moves
	 */
	void setStartingPos(MoveGen *);

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
	 * @brief
	 *
	 * @param color bool representing the color to generate attack board for, white = false; black = true
	 * @param moveGen helper class used to generate pseudo legal moves (and in this case attacks)
	 */
	void genAttackBoard(bool, MoveGen *);

	/**
	 * @brief generates all possible moves for the current board
	 *
	 * @param moveGen helper class used to generate pseudo legal moves
	 */
	void genMoves(MoveGen *);

	/**
	 * @brief debug only, prints the board
	 *
	 * @param color color of perspective, white = false; black = true
	 */
	void print(bool = white);

	/**
	 * @brief debug only, prints the legal moves of the current position
	 *
	 */
	void printMoves();
};