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
	 */
	bool turn;
	vector<Move> moves;
	MoveGen *moveGen;

	// debug only
	string enumToChar = "pPnNbBrRqQkK";

	Board(MoveGen *);

	/**
	 * @brief sets bitboards to standard starting pos
	 *
	 * @param moveGen helper class used to generate pseudo legal moves
	 */
	void setStartingPos();

	/**
	 * @brief move piece given wtv lichess gives -> have to make modifications
	 *
	 * @param move move in string
	 */
	void movePiece(string);
	/**
	 * @brief move piece in standard piece bitboards
	 *
	 * @param move pointer to move object
	 */
	void movePiece(Move *);

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
	void genAttackBoard(bool);

	/**
	 * @brief generates all possible moves for the current board
	 *
	 * @param debug whether or not to print time msgs
	 */
	void genMoves(bool = false);

	/**
	 * @brief returns whether or not the color is currently in check
	 *
	 * @param color white = false; black = true
	 * @return true
	 * @return false
	 */
	bool inCheck(bool);

	/**
	 * @brief returns whether or not it's legal
	 *
	 * @param Move move to be played on this board
	 */
	bool isMoveLegal(Move *);

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