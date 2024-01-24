#pragma once
#include "includes.hpp"

/**
 * @brief helper class for generating moves of each piece
 *
 */
class MoveGen {
public:
	/**
	 * @brief stores precomputed ray attacks for empty board
	 *
	 * @param square what square the piece is on
	 * @param direction north starts at 0, numbers increase w/ clockwise rotation
	 */
	unsigned ll rayAttacks[64][8];
	unsigned ll rookRays[64];
	unsigned ll bishopRays[64];

	/**
	 * @brief initializes ray attacks
	 *
	 */
	MoveGen();

	/**
	 * @brief generates knight moves
	 *
	 * @param knight pieceboard representing the position of the knight
	 * @param friendlyPieces pieceboard representing all friendly pieces
	 * @param moves resulting moves
	 */
	void genKnightMoves(unsigned ll, unsigned ll, vector<Move> &);

	/**
	 * @brief generates king moves
	 *
	 * @param king pieceboard representing the position of the king
	 * @param friendlyPieces pieceboard representing all friendly pieces
	 * @param moves resulting moves
	 */
	void genKingMoves(unsigned ll, unsigned ll, vector<Move> &);

	/**
	 * @brief generates bishop moves
	 *
	 * @param bishop pieceboard representing the position of the bishop
	 * @param friendlyPieces pieceboard representing all friendly pieces
	 * @param enemyPieces pieceboard representing all enemy pieces
	 * @param moves resulting moves
	 */
	void genBishopMoves(unsigned ll, unsigned ll, unsigned ll, vector<Move> &);

	/**
	 * @brief generates rook moves
	 *
	 * @param rook pieceboard representing the position of the rook
	 * @param friendlyPieces pieceboard representing all friendly pieces
	 * @param enemyPieces pieceboard representing all enemy pieces
	 * @param moves resulting moves
	 */
	void genRookMoves(unsigned ll, unsigned ll, unsigned ll, vector<Move> &);

	/**
	 * @brief generates queen moves
	 *
	 * @param queen pieceboard representing the position of the queen
	 * @param friendlyPieces pieceboard representing all friendly pieces
	 * @param enemyPieces pieceboard representing all enemy pieces
	 * @param moves resulting moves
	 */
	void genQueenMoves(unsigned ll, unsigned ll, unsigned ll, vector<Move> &);

	/**
	 * @brief generates pawn moves
	 *
	 * @param pawn pieceboard representing the position of the pawn
	 * @param pieceBoards pieceboards
	 * @param moves resulting moves
	 */
	void genPawnMoves(unsigned ll, unsigned ll[12], vector<Move> &);
};
