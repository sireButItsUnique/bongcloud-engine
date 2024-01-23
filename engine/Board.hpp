#pragma once
#include "MoveGen.hpp"
#include <iostream>
#include <string>
#include <utility>
#include <vector>
using namespace std;

#define ll long long
#define pawnWhite 0
#define pawnBlack 1
#define knightWhite 2
#define knightBlack 3
#define bishopWhite 4
#define bishopBlack 5
#define rookWhite 6
#define rookBlack 7
#define queenWhite 8
#define queenBlack 9
#define kingWhite 10
#define kingBlack 11
#define Move pair<uint8_t, uint8_t>

#define TO_SQUARE(col, row) 8 * (row - '1') + 7 - (col - 'a')
#define TO_ALGEBRA(sq)                                                         \
	string(1, 'a' + (7 - (sq % 8))) + string(1, '1' + (sq / 8))

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