#include "Move.hpp"

Move::Move(bool side) {
	data = 0;
	if (side)
		data |= 0x1000;
}

// need to make e8g8 into O-O if its castle (...)
Move::Move(string move) {
	data = 0;
	if (move == "O-O") {
		return;
	} else if (move == "O-O-O") {
		data |= 0x1000;
		return;
	}

	data |= (TO_SQUARE(move[0], move[1])) << 6;
	data |= (TO_SQUARE(move[2], move[3]));
	if (move.size() > 4) {
		char piece = move[4];
		data |= 0x8000;
		data |= 0x4000 & ((piece == 'N') << 14);
	}
}

Move::Move(int from, int to, bool piece) {
	data = 0;
	data |= to;
	data |= (from << 6);
	data |= 0x4000 & (piece << 14);
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

bool Move::castleSide() {
	return (data & 0x1000);
}

string Move::toAlgebra(bool debug, bool color) {
	if (this->isCastle()) {
		if (debug) {
			return (this->castleSide() ? "O-O-O" : "O-O");
		} else {
			if (color == black) {
				return (this->castleSide() ? "e8c8" : "e8g8");
			} else {
				return (this->castleSide() ? "e1c1" : "e1c1");
			}
		}
	}
	if (this->isPromotion()) {
		return (TO_ALGEBRA(this->from()) + TO_ALGEBRA(this->to()) + (this->promotionPiece() ? "N" : "Q"));
	}
	return (TO_ALGEBRA(this->from()) + TO_ALGEBRA(this->to()));
}