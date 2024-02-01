#pragma once
#include "includes.hpp"

class Move {
public:
	/**
	 * @brief [0, 1] = promotion {is, piece}
	 *        [2, 3] = castling {color, side}
	 *        [4, 9] = from
	 *        [10, 15] = to
	 *
	 */
	uint16_t data;

	/**
	 * @brief constructor for promotion move
	 *
	 * @param from from square
	 * @param to to square
	 * @param piece piece to promote to, queen = false; knight = true
	 */
	Move(int, int, bool);

	/**
	 * @brief constructor for castle move
	 *
	 * @param side which side castling occurs, kingside = false; queenside = true
	 */
	Move(bool);

	/**
	 * @brief constructor for basic move
	 *
	 * @param from from square
	 * @param to to square
	 */
	Move(int, int);

	/**
	 * @brief constructor given algebraic notation
	 *
	 * @param move in algebraic notation
	 */
	Move(string);

	/**
	 * @brief constructor with raw data
	 *
	 * @param data raw 16 bit data
	 */
	Move(uint16_t);

	/**
	 * @brief returns the square the piece came from
	 *
	 * @return uint8_t
	 */
	uint8_t from();

	/**
	 * @brief returns the square the piece moved to
	 *
	 * @return uint8_t
	 */
	uint8_t to();

	/**
	 * @brief returns whether or not the move has a promotion flag
	 *
	 * @return true
	 * @return false
	 */
	bool isPromotion();

	/**
	 * @brief returns which piece the pawn promoted to during the move
	 *
	 * @return true = knight
	 * @return false = queen
	 */
	bool promotionPiece();

	/**
	 * @brief returns whether or not the move has a castle flag
	 *
	 * @return true
	 * @return false
	 */
	bool isCastle();

	/**
	 * @brief returns which side the guy is castling
	 *
	 * @return true = queenside
	 * @return false = kingside
	 */
	bool castleSide();

	/**
	 * @brief returns algebraic move
	 *
	 */
	string toAlgebra();
};