#include "MoveGen.hpp"

MoveGen::MoveGen() {
	auto start = chrono::high_resolution_clock::now();

	initializeRays();
	initializeRookLookup();
	initializeBishopLookup();
	initializeKnightLookup();
	initializeKingLookup();

	auto end = chrono::high_resolution_clock::now();
	double time_taken = chrono::duration_cast<chrono::nanoseconds>(end - start).count();
	time_taken *= 1e-9;
	cout << GREEN << "Initiated move generator in " << fixed << time_taken << setprecision(9) << " secs\n" << UNCOLOR;
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
	}

	// bishop attacks
	for (int i = 0; i < 64; i++) {
		bishopRays[i] = rayAttacks[i][NORTHEAST] | rayAttacks[i][NORTHWEST] | rayAttacks[i][SOUTHEAST] | rayAttacks[i][SOUTHWEST];
		bishopRays[i] &= 0x7e7e7e7e7e7e00;
	}
	bishopLookupOffsets[0] = 0;
	for (int i = 1; i < 64; i++) {
		bishopLookupOffsets[i] = bishopLookupOffsets[i - 1] + (1ULL << __popcnt64(bishopRays[i - 1]));
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

void MoveGen::initializeKnightLookup() {

	for (int square = 0; square < 64; square++) {
		unsigned ll knightPos = (1ULL << square);
		unsigned ll horizontal1 = ((knightPos << 1) & 0xfefefefefefefefe) | ((knightPos >> 1) & 0x7f7f7f7f7f7f7f7f);
		unsigned ll horizontal2 = ((knightPos << 2) & 0xfcfcfcfcfcfcfcfc) | ((knightPos >> 2) & 0x3f3f3f3f3f3f3f3f);
		knightLookup[square] = (horizontal2 << 8) | (horizontal2 >> 8) | (horizontal1 << 16) | (horizontal1 >> 16);
	}
}

void MoveGen::initializeKingLookup() {

	for (int square = 0; square < 64; square++) {
		unsigned ll kingPos = (1ULL << square);
		kingPos |= ((kingPos << 1) & 0xfefefefefefefefe) | ((kingPos >> 1) & 0x7f7f7f7f7f7f7f7f);
		kingPos |= (kingPos << 8) | (kingPos >> 8);
		kingLookup[square] = kingPos & ~(1ULL << square);
	}
}

void MoveGen::genKnightMoves(unsigned ll knightBoard, unsigned ll friendlyPieces, vector<Move> &moves) {

	while (knightBoard) {
		// get lookup
		uint8_t from = _tzcnt_u64(knightBoard);
		unsigned ll knight = knightLookup[from];
		knight &= ~friendlyPieces;

		// iterate over end positions
		while (knight) {
			uint8_t to = _tzcnt_u64(knight);
			moves.push_back(Move(from, to));
			knight &= ~(1ULL << to);
		}
		knightBoard &= ~(1ULL << from);
	}
}

void MoveGen::genKingMoves(unsigned ll king, unsigned ll friendlyPieces, vector<Move> &moves) {

	// get lookup
	uint8_t from = _tzcnt_u64(king);
	king = kingLookup[from];
	king &= ~friendlyPieces;

	// iterate over end positions
	while (king) {
		uint8_t to = _tzcnt_u64(king);
		moves.push_back(Move(from, to));
		king &= ~(1ULL << to);
	}
}

void MoveGen::genBishopMoves(unsigned ll bishopBoard, unsigned ll friendlyPieces, unsigned ll enemyPieces, vector<Move> &moves) {

	while (bishopBoard) {
		// gen rays
		uint8_t from = _tzcnt_u64(bishopBoard);
		unsigned ll bishop = bishopRays[from];

		// gen relevant bits + lookup
		unsigned ll blockers = _pext_u64(friendlyPieces | enemyPieces, bishop);
		bishop = bishopLookup[bishopLookupOffsets[from] + blockers];
		bishop &= ~friendlyPieces;

		// iterate over end positions
		while (bishop) {
			uint8_t to = _tzcnt_u64(bishop);
			moves.push_back(Move(from, to));
			bishop &= ~(1ULL << to);
		}
		bishopBoard &= ~(1ULL << from);
	}
}

void MoveGen::genRookMoves(unsigned ll rookBoard, unsigned ll friendlyPieces, unsigned ll enemyPieces, vector<Move> &moves) {

	while (rookBoard) {
		// gen rays
		uint8_t from = _tzcnt_u64(rookBoard);
		unsigned ll rook = rookRays[from];

		// gen relevant bits + lookup
		unsigned ll blockers = _pext_u64(friendlyPieces | enemyPieces, rook);
		rook = rookLookup[rookLookupOffsets[from] + blockers];
		rook &= ~friendlyPieces;

		// iterate over end positions
		while (rook) {
			uint8_t to = _tzcnt_u64(rook);
			moves.push_back(Move(from, to));
			rook &= ~(1ULL << to);
		}
		rookBoard &= ~(1ULL << from);
	}
}

void MoveGen::genQueenMoves(unsigned ll queenBoard, unsigned ll friendlyPieces, unsigned ll enemyPieces, vector<Move> &moves) {

	while (queenBoard) {
		// gen rays
		uint8_t from = _tzcnt_u64(queenBoard);
		unsigned ll rookBlockers = rookRays[from];
		unsigned ll bishopBlockers = bishopRays[from];

		// gen relevant bits + lookup
		rookBlockers = _pext_u64(friendlyPieces | enemyPieces, rookBlockers);
		bishopBlockers = _pext_u64(friendlyPieces | enemyPieces, bishopBlockers);
		unsigned ll queen = rookLookup[rookLookupOffsets[from] + rookBlockers];
		queen |= bishopLookup[bishopLookupOffsets[from] + bishopBlockers];
		queen &= ~friendlyPieces;

		// iterate over end positions
		while (queen) {
			uint8_t to = _tzcnt_u64(queen);
			moves.push_back(Move(from, to));
			queen &= ~(1ULL << to);
		}
		queenBoard &= ~(1ULL << from);
	}
}

void MoveGen::genPawnMoves(unsigned ll pawnBoard, bool color, unsigned ll friendlyPieces, unsigned ll enemyPieces, vector<Move> &moves) {

	while (pawnBoard) {
		// mask pawn
		uint8_t from = _tzcnt_u64(pawnBoard);
		unsigned ll pawn = (1ULL << from);

		// pushes
		if (color) {
			pawn >>= 8;
			pawn &= ~(friendlyPieces | enemyPieces);
			if (pawn & 0xff0000000000) {
				pawn |= (pawn >> 8);
				pawn &= ~(friendlyPieces | enemyPieces);
			}
		} else {
			pawn <<= 8;
			pawn &= ~(friendlyPieces | enemyPieces);
			if (pawn & 0xff0000) {
				pawn |= (pawn << 8);
				pawn &= ~(friendlyPieces | enemyPieces);
			}
		}

		// captures
		if (color) {
			pawn |= (((1ULL << from) >> 9) & 0x7f7f7f7f7f7f7f7f & enemyPieces);
			pawn |= (((1ULL << from) >> 7) & 0xfefefefefefefefe & enemyPieces);
		} else {
			pawn |= (((1ULL << from) << 9) & 0xfefefefefefefefe & enemyPieces);
			pawn |= (((1ULL << from) << 7) & 0x7f7f7f7f7f7f7f7f & enemyPieces);
		}

		// iterate over end positions
		while (pawn) {
			uint8_t to = _tzcnt_u64(pawn);
			if (to >= 56 || to <= 7) {
				moves.push_back(Move(from, to, true));
				moves.push_back(Move(from, to, false));
			} else {
				moves.push_back(Move(from, to));
			}

			pawn &= ~(1ULL << to);
		}
		pawnBoard &= ~(1ULL << from);
	}
}

void MoveGen::genCastleMoves(
	bool color, bool canCastleKing, bool canCastleQueen, unsigned ll friendlyPieces, unsigned ll enemyPieces, unsigned ll enemyAttacks, vector<Move> &moves
) {

	// queenside
	if (canCastleQueen) {
		if (color) { // black
			if ((friendlyPieces | enemyPieces) & 0x7000000000000000) {
				// pieces blocking
			} else if (enemyAttacks & 0x7800000000000000) {
				// in check/squares checked
			} else {
				moves.push_back(Move(color, true));
			}

		} else { // white
			if ((friendlyPieces | enemyPieces) & 0x70) {
				// pieces blocking
			} else if (enemyAttacks & 0x78) {
				// in check/squares checked
			} else {
				moves.push_back(Move(color, true));
			}
		}
	}

	// kingside
	if (canCastleKing) {
		if (color) { // black
			if ((friendlyPieces | enemyPieces) & 0x600000000000000) {
				// pieces blocking
			} else if (enemyAttacks & 0xe00000000000000) {
				// in check/squares checked
			} else {
				moves.push_back(Move(color, false));
			}
		} else { // white
			if ((friendlyPieces | enemyPieces) & 0x6) {
				// pieces blocking
			} else if (enemyAttacks & 0xe) {
				// in check/squares checked
			} else {
				moves.push_back(Move(color, false));
			}
		}
	}
}