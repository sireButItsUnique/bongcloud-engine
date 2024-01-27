#pragma once
#include "Move.hpp"
#include "SlideAttacks.hpp"
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

	unsigned ll rookLookupOffsets[64];
	unsigned ll bishopLookupOffsets[64];

	unsigned ll rookLookup[102400];
	unsigned ll bishopLookup[5248];
	unsigned ll knightLookup[64];
	unsigned ll kingLookup[64];

	/**
	 * @brief precomputes all sliding piece moves
	 *
	 */
	MoveGen();

	/**
	 * @brief initializes ray attacks
	 *
	 */
	void initializeRays();

	/**
	 * @brief initializes rook lookup array
	 *
	 */
	void initializeRookLookup();

	/**
	 * @brief initializes rook lookup array
	 *
	 */
	void initializeBishopLookup();

	/**
	 * @brief initializes knight lookup array
	 *
	 */
	void initializeKnightLookup();

	/**
	 * @brief initializes king lookup array
	 *
	 */
	void initializeKingLookup();

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
	 * @brief generates pawn moves, ignoring en passsant for now
	 *
	 * @param pawn pieceboard representing the position of the pawn
	 * @param color color of pawn, white = false; black = true
	 * @param friendlyPieces pieceboard representing all friendly pieces
	 * @param enemyPieces pieceboard representing all enemy pieces
	 * @param moves resulting moves
	 */
	void genPawnMoves(unsigned ll, bool, unsigned ll, unsigned ll, vector<Move> &);

	/**
	 * @brief generates possible castles (note position of king and rook are not checked as if they moved canCastle will already be marked false)
	 *
	 * @param side side to castle, kingside = false; queenside = true
	 * @param color color for which castling is to be checked, white = false; black = true
	 * @param friendlyPieces pieceboard representing all friendly pieces
	 * @param enemyPieces pieceboard representing all enemy pieces
	 * @param enemyAttacks pieceboard representing all pieces the enemy currently attakcs
	 * @param moves resulting moves
	 */
	void genCastleMoves(bool, bool, unsigned ll, unsigned ll, unsigned ll, vector<Move> &);
};
