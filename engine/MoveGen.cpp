#include "MoveGen.hpp"

MoveGen::MoveGen() {

	// north
	unsigned ll north = 0x0101010101010100ULL;
	for (int square = 0; square < 64; square++) {
		rayAttacks[square][NORTH] = north;
		north <<= 1;
	}

	// northeast
	unsigned ll northeast = 0x102040810204000ULL;
	for (int col = 7; col >= 0; col--) {
		unsigned ll ne = northeast;
		for (int r8 = 0; r8 < 8 * 8; r8 += 8, ne <<= 8) {
			rayAttacks[r8 + col][NORTHEAST] = ne;
		}
		northeast = ((northeast >> 1) & 0x7f7f7f7f7f7f7f7f);
	}

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
	unsigned ll southeast = 0x40201008040201ULL;
	for (int col = 7; col >= 0; col--) {
		unsigned ll se = southeast;
		for (int row = 7; row >= 0; row--, se >>= 8) {
			rayAttacks[row * 8 + col][SOUTHEAST] = se;
		}
		southeast = ((southeast >> 1) & 0x7f7f7f7f7f7f7f7f);
	}

	// south
	unsigned ll south = 0x0080808080808080ULL;
	for (int square = 63; square >= 0; square--) {
		rayAttacks[square][SOUTH] = south;
		south >>= 1;
	}

	// south west
	unsigned ll southwest = 0x2040810204080ULL;
	for (int col = 0; col < 8; col++) {
		unsigned ll sw = southwest;
		for (int row = 7; row >= 0; row--, sw >>= 8) {
			rayAttacks[row * 8 + col][SOUTHWEST] = sw;
		}
		southwest = ((southwest << 1) & 0xfefefefefefefefe);
	}

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
	unsigned ll northwest = 0x8040201008040200ULL;
	for (int col = 0; col < 8; col++) {
		unsigned ll nw = northwest;
		for (int r8 = 0; r8 < 8 * 8; r8 += 8, nw <<= 8) {
			rayAttacks[r8 + col][NORTHWEST] = nw;
		}
		northwest = ((northwest << 1) & 0xfefefefefefefefe);
	}

	// rook attacks
	for (int i = 0; i < 64; i++) {
		rookRays[i] = rayAttacks[i][NORTH] | rayAttacks[i][SOUTH] | rayAttacks[i][WEST] | rayAttacks[i][EAST];
	}

	// bishop attacks
	for (int i = 0; i < 64; i++) {
		bishopRays[i] = rayAttacks[i][NORTHEAST] | rayAttacks[i][NORTHWEST] | rayAttacks[i][SOUTHEAST] | rayAttacks[i][SOUTHWEST];
	}
	cout << bishopRays[35];
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