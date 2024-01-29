#include "MoveGen.hpp"

void MoveGen::genKnightMovesA(unsigned ll knight, unsigned ll friendlyPieces, unsigned ll &attack) {

	while (knight) {
		// get lookup
		uint8_t from = _tzcnt_u64(knight);
		attack |= knightLookup[from];

		// remove knight
		knight &= ~(1ULL << from);
	}
}

void MoveGen::genKingMovesA(unsigned ll king, unsigned ll friendlyPieces, unsigned ll &attack) {

	// get lookup
	uint8_t from = _tzcnt_u64(king);
	king = kingLookup[from];
	attack |= king;
}

void MoveGen::genBishopMovesA(unsigned ll bishop, unsigned ll friendlyPieces, unsigned ll enemyPieces, unsigned ll &attack) {

	while (bishop) {
		// gen rays
		uint8_t from = _tzcnt_u64(bishop);
		unsigned ll bishopBlockers = bishopRays[from];

		// gen relevant bits + lookup
		bishopBlockers = _pext_u64(friendlyPieces | enemyPieces, bishop);
		attack |= bishopLookup[bishopLookupOffsets[from] + bishopBlockers];

		// remove bishop
		bishop &= ~(1ULL << from);
	}
}

void MoveGen::genRookMovesA(unsigned ll rook, unsigned ll friendlyPieces, unsigned ll enemyPieces, unsigned ll &attack) {

	while (rook) {
		// gen rays
		uint8_t from = _tzcnt_u64(rook);
		unsigned ll rookBlockers = rookRays[from];

		// gen relevant bits + lookup
		rookBlockers = _pext_u64(friendlyPieces | enemyPieces, rook);
		attack |= rookLookup[rookLookupOffsets[from] + rookBlockers];

		// remove rook
		rook &= ~(1ULL << from);
	}
}

void MoveGen::genQueenMovesA(unsigned ll queen, unsigned ll friendlyPieces, unsigned ll enemyPieces, unsigned ll &attack) {

	while (queen) {
		// gen rays
		uint8_t from = _tzcnt_u64(queen);
		unsigned ll rookBlockers = rookRays[from];
		unsigned ll bishopBlockers = bishopRays[from];

		// gen relevant bits + lookup
		rookBlockers = _pext_u64(friendlyPieces | enemyPieces, rookBlockers);
		bishopBlockers = _pext_u64(friendlyPieces | enemyPieces, bishopBlockers);
		attack |= rookLookup[rookLookupOffsets[from] + rookBlockers];
		attack |= bishopLookup[bishopLookupOffsets[from] + bishopBlockers];

		// remove queen
		queen &= ~(1ULL << from);
	}
}

void MoveGen::genPawnMovesA(unsigned ll pawn, bool color, unsigned ll friendlyPieces, unsigned ll enemyPieces, unsigned ll &attack) {

	// pawn captures
	if (color) {
		attack |= ((pawn >> 9) & 0x7f7f7f7f7f7f7f7f);
		attack |= ((pawn >> 7) & 0xfefefefefefefefe);
	} else {
		attack |= ((pawn << 9) & 0xfefefefefefefefe);
		attack |= ((pawn << 7) & 0x7f7f7f7f7f7f7f7f);
	}
}