#include "Move.hpp"

Move::Move(bool color, bool side) {
	data = 0;
	if (color)
		data |= 0x2000;
	if (side)
		data |= 0x1000;
}

Move::Move(int from, int to, bool piece) {
	data = 0;
	data |= to;
	data |= (from << 6);
	data |= 0x4000 & piece;
	data |= 0x8000;
}

Move::Move(int from, int to) {
	data = 0;
	data |= to;
	data |= (from << 6);
}

Move::Move(uint16_t data) {
	this->data = data;
}

uint8_t Move::from() {
	return ((data >> 6) & 0x3f);
}

uint8_t Move::to() {
	return (data & 0x3f);
}

bool Move::isPromotion() {
	return (data & 0x8000);
}

bool Move::promotionPiece() {
	return (data & 0x4000);
}

bool Move::isCastle() {
	return (this->from() == this->to());
}