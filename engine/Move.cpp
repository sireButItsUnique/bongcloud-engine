#include "Move.hpp"

constexpr Move::Move(uint16_t data) {
	this->data = data;
}

constexpr Move::Move(int from, int to) {
	data = 0;
	data |= to;
	data |= (from << 6);
}

constexpr Move::Move(int from, int to, bool piece) {
	data = 0;
	data |= to;
	data |= (from << 6);
	data |= 0x4000 & piece;
	data |= 0x8000;
}

constexpr uint8_t Move::from() {
	return ((data >> 6) & 0x3f);
}

constexpr uint8_t Move::to() {
	return (data & 0x3f);
}

constexpr bool Move::isPromotion() {
	return (data & 0x8000);
}

constexpr bool Move::promotionPiece() {
	return (data & 0x4000);
}