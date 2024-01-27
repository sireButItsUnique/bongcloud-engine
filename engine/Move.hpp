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
	 * @brief constructor with raw data
	 *
	 * @param data raw 16 bit data
	 */
	constexpr Move(uint16_t);

	/**
	 * @brief constructor for basic move
	 *
	 * @param from from square
	 * @param to to square
	 */
	constexpr Move(int from, int to);

	/**
	 * @brief constructor for promotion move
	 *
	 * @param from from square
	 * @param to to square
	 * @param piece piece to promote to, queen = false; knight = true
	 */
	constexpr Move(int from, int to, bool piece);

	/**
	 * @brief returns the square the piece came from
	 *
	 * @return constexpr uint8_t
	 */
	constexpr uint8_t from();

	/**
	 * @brief returns the square the piece moved to
	 *
	 * @return constexpr uint8_t
	 */
	constexpr uint8_t to();

	/**
	 * @brief returns whether or not the move has a promotion flag
	 *
	 * @return true
	 * @return false
	 */
	constexpr bool isPromotion();

	/**
	 * @brief returns which piece the pawn promoted to during the move
	 *
	 * @return true = knight
	 * @return false = queen
	 */
	constexpr bool promotionPiece();
};