#include "MoveGen.hpp"

MoveGen::MoveGen() {
	time_t start, end;
	time(&start);

	initializeRays();
	initializeRookLookup();
	initializeBishopLookup();

	time(&end);
	double time_taken = double(end - start);
	cout << "Initiated move generator in " << fixed << time_taken << setprecision(5) << " secs\n";
}

void MoveGen::initializeRays() {
	// north
	unsigned ll north = 0x101010101010100ULL;
	for (int square = 0; square < 64; square++) {
		north &= 0xffffffffffffff;
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
		if (ogEast) { // prevents int overflow
			ogEast -= (1ULL << col);
		}
		ogEast &= 0xfefefefefefefefe;
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
		south &= 0xffffffffffffff00;
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
		if (ogWest) { // prevents int overflow
			ogWest -= (1ULL << col);
		}
		ogWest &= 0x7f7f7f7f7f7f7f7f;
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

	rookLookupOffsets[0] = 0;
	for (int i = 1; i < 64; i++) {
		rookLookupOffsets[i] = rookLookupOffsets[i - 1] + (1ULL << __popcnt64(rookRays[i - 1]));
		// cout << i << ": " << rookLookupOffsets[i] << " (+2^" << __popcnt64(rookRays[i]) << ")";
		// cout << " (" << rookRays[i] << ")\n";
	}

	// bishop attacks
	for (int i = 0; i < 64; i++) {
		bishopRays[i] = rayAttacks[i][NORTHEAST] | rayAttacks[i][NORTHWEST] | rayAttacks[i][SOUTHEAST] | rayAttacks[i][SOUTHWEST];
		bishopRays[i] &= 0x7e7e7e7e7e7e00;
	}
	bishopLookupOffsets[0] = 0;
	for (int i = 1; i < 64; i++) {
		bishopLookupOffsets[i] = bishopLookupOffsets[i - 1] + (1ULL << __popcnt64(bishopRays[i - 1]));
		// cout << i << ": " << bishopLookupOffsets[i] << " (+2^" << __popcnt64(bishopRays[i]) << ")";
		// cout << " (" << bishopRays[i] << ")\n";
	}
}

void MoveGen::initializeRookLookup() {

	for (int square = 0; square < 64; square++) {
		unsigned ll maxPos = 1ULL << __popcnt64(rookRays[square]);
		unsigned ll rook = 1ULL << square;
		for (unsigned ll i = 0; i < maxPos; i++) {
			unsigned ll blockers = _pdep_u64(i, rookRays[square]);

			rookLookup[rookLookupOffsets[square] + i] = SlideAttacks::northAttacks(rook, blockers);
			rookLookup[rookLookupOffsets[square] + i] |= SlideAttacks::southAttacks(rook, blockers);
			rookLookup[rookLookupOffsets[square] + i] |= SlideAttacks::eastAttacks(rook, blockers);
			rookLookup[rookLookupOffsets[square] + i] |= SlideAttacks::westAttacks(rook, blockers);
		}
	}
}

void MoveGen::initializeBishopLookup() {

	for (int square = 0; square < 64; square++) {
		unsigned ll maxPos = 1ULL << __popcnt64(bishopRays[square]);
		unsigned ll bishop = 1ULL << square;
		for (unsigned ll i = 0; i < maxPos; i++) {
			unsigned ll blockers = _pdep_u64(i, bishopRays[square]);

			bishopLookup[bishopLookupOffsets[square] + i] = SlideAttacks::noEaAttacks(bishop, blockers);
			bishopLookup[bishopLookupOffsets[square] + i] |= SlideAttacks::noWeAttacks(bishop, blockers);
			bishopLookup[bishopLookupOffsets[square] + i] |= SlideAttacks::soEaAttacks(bishop, blockers);
			bishopLookup[bishopLookupOffsets[square] + i] |= SlideAttacks::soWeAttacks(bishop, blockers);
		}
	}
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
	uint8_t from = _tzcnt_u64(bishop);
	bishop = bishopRays[from];

	// gen relevant bits + lookup
	unsigned ll blockers = _pext_u64(friendlyPieces | enemyPieces, bishop);
	// cout << "BISHOP: lookupIdx (" << (bishopLookupOffsets[from] + blockers) << ") = offset (" << bishopLookupOffsets[from];
	// cout << ") + relevantBits (" << blockers << ")\n";

	bishop = bishopLookup[bishopLookupOffsets[from] + blockers];
	bishop &= ~friendlyPieces;
	// cout << "BISHOP: moves = " << bishop << endl;

	// iterate over end positions
	while (bishop) {
		uint8_t to = _tzcnt_u64(bishop);
		moves.push_back({from, to});
		bishop &= ~(1ULL << to);
	}
}

void MoveGen::genRookMoves(unsigned ll rook, unsigned ll friendlyPieces, unsigned ll enemyPieces, vector<Move> &moves) {

	// gen rays
	uint8_t from = _tzcnt_u64(rook);
	rook = rookRays[from];

	// gen relevant bits + lookup
	unsigned ll blockers = _pext_u64(friendlyPieces | enemyPieces, rook);

	// cout << "ROOK: lookupIdx (" << (rookLookupOffsets[from] + blockers) << ") = offset (" << rookLookupOffsets[from];
	// cout << ") + relevantBits (" << blockers << ")\n";

	rook = rookLookup[rookLookupOffsets[from] + blockers];
	rook &= ~friendlyPieces;
	// cout << "ROOK: moves = " << rook << endl;

	// iterate over end positions
	while (rook) {
		uint8_t to = _tzcnt_u64(rook);
		moves.push_back({from, to});
		rook &= ~(1ULL << to);
	}
}

void MoveGen::genQueenMoves(unsigned ll queen, unsigned ll friendlyPieces, unsigned ll enemyPieces, vector<Move> &moves) {

	// gen rays
	uint8_t from = _tzcnt_u64(queen);
	unsigned ll rookBlockers = rookRays[from];
	unsigned ll bishopBlockers = bishopRays[from];

	// gen relevant bits + lookup
	rookBlockers = _pext_u64(friendlyPieces | enemyPieces, rookBlockers);
	bishopBlockers = _pext_u64(friendlyPieces | enemyPieces, bishopBlockers);
	queen = rookLookup[rookLookupOffsets[from] + rookBlockers];
	queen |= bishopLookup[bishopLookupOffsets[from] + bishopBlockers];
	queen &= ~friendlyPieces;

	// iterate over end positions
	while (queen) {
		uint8_t to = _tzcnt_u64(queen);
		moves.push_back({from, to});
		queen &= ~(1ULL << to);
	}
}