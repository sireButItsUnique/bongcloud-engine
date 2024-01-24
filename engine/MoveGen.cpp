#include "MoveGen.hpp"

MoveGen::MoveGen() {

	// north
	unsigned ll north = 0x0101010101010100ULL;
	for (int square = 0; square < 64; square++) {
		rayAttacks[square][NORTH] = north;
		north <<= 1;
	}

	// northeast

	// east
	unsigned ll ogEast = 0xffULL;
	for (int col = 7; col >= 0; col--) {
		ogEast -= (1ULL << col);
		unsigned ll east = ogEast;

		for (int row = 0; row < 8; row++) {
			rayAttacks[row * 8 + col][EAST] = east;
			east <<= 8;
		}
	}

	// south east

	// south
	unsigned ll south = 0x0080808080808080ULL;
	for (int square = 63; square >= 0; square--) {
		rayAttacks[square][SOUTH] = south;
		south >>= 1;
	}

	// south west

	// west
	unsigned ll ogWest = 0xffULL;
	for (int col = 0; col < 8; col++) {
		ogWest -= (1ULL << col);
		unsigned ll west = ogWest;

		for (int row = 0; row < 8; row++) {
			rayAttacks[row * 8 + col][WEST] = west;
			west <<= 8;
		}
	}
	// north west

	// rook attacks
	for (int i = 0; i < 64; i++) {
		rookRays[i] = rayAttacks[i][NORTH] | rayAttacks[i][SOUTH] | rayAttacks[i][WEST] | rayAttacks[i][EAST];
	}
	cout << rookRays[0];
}

void MoveGen::genKnightMoves(unsigned ll knight, unsigned ll friendlyPieces, vector<Move> &moves) {

	// mask knight
	uint8_t from = _tzcnt_u64(knight);
	unsigned ll horizontal1 = ((knight << 1) & 0xfefefefefefefefe) | ((knight >> 1) & 0x7f7f7f7f7f7f7f7f);
	unsigned ll horizontal2 = ((knight << 2) & 0xfcfcfcfcfcfcfcfc) | ((knight >> 2) & 0x3f3f3f3f3f3f3f3f);
	knight = (horizontal2 << 8) | (horizontal2 >> 8) | (horizontal1 << 16) | (horizontal1 >> 16);
	knight &= ~friendlyPieces;

	// iterate over end positions
	while (knight) {
		uint8_t to = _tzcnt_u64(knight);
		moves.push_back({from, to});
		knight &= ~(1ULL << to);
	}
}

void MoveGen::genKingMoves(unsigned ll king, unsigned ll friendlyPieces, vector<Move> &moves) {

	// mask king
	uint8_t from = _tzcnt_u64(king);
	king |= ((king << 1) & 0xfefefefefefefefe) | ((king >> 1) & 0x7f7f7f7f7f7f7f7f);
	king |= (king << 8) | (king >> 8);
	king &= ~friendlyPieces;

	// iterate over end positions
	while (king) {
		uint8_t to = _tzcnt_u64(king);
		moves.push_back({from, to});
		king &= ~(1ULL << to);
	}
}

void MoveGen::genBishopMoves(unsigned ll bishop, unsigned ll friendlyPieces, unsigned ll enemyPieces, vector<Move> &moves) {

	// gen rays
}