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
	constexpr Move(uint16_t data) {
		this->data = data;
	}

	/**
	 * @brief constructor for basic move
	 *
	 * @param from from square
	 * @param to to square
	 */
	constexpr Move(int from, int to) {
		this->data = 0;
		this->data |= to;
		this->data |= (from << 6);
	}

	/**
	 * @brief constructor for promotion move
	 *
	 * @param from from square
	 * @param to to square
	 * @param piece piece to promote to, queen = false; knight = true
	 */
	constexpr Move(int from, int to, bool piece) {
		this->data = 0;
		this->data |= to;
		this->data |= (from << 6);
		this->data |= 0x4000 & piece;
		this->data |= 0x8000;
	};

	constexpr uint8_t from() {
		return ((data >> 6) & 0x3f);
	}
	constexpr uint8_t to() {
		return (data & 0x3f);
	}
	constexpr bool isPromotion() {
		return (data & 0x8000);
	}
	constexpr bool promotionPiece() {
		return (data & 0x4000);
	}
};