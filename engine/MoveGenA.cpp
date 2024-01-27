#include "MoveGen.hpp"

void MoveGen::genKnightMovesA(unsigned ll knight, unsigned ll friendlyPieces, unsigned ll &attack) {

	// get lookup
	uint8_t from = _tzcnt_u64(knight);
	knight = knightLookup[from];
	attack |= knight;
}

void MoveGen::genKingMovesA(unsigned ll king, unsigned ll friendlyPieces, unsigned ll &attack) {

	// get lookup
	uint8_t from = _tzcnt_u64(king);
	king = kingLookup[from];
	attack |= king;
}

void MoveGen::genBishopMovesA(unsigned ll bishop, unsigned ll friendlyPieces, unsigned ll enemyPieces, unsigned ll &attack) {

	// gen rays
	uint8_t from = _tzcnt_u64(bishop);
	bishop = bishopRays[from];

	// gen relevant bits + lookup
	unsigned ll blockers = _pext_u64(friendlyPieces | enemyPieces, bishop);
	bishop = bishopLookup[bishopLookupOffsets[from] + blockers];
	attack |= bishop;
}

void MoveGen::genRookMovesA(unsigned ll rook, unsigned ll friendlyPieces, unsigned ll enemyPieces, unsigned ll &attack) {

	// gen rays
	uint8_t from = _tzcnt_u64(rook);
	rook = rookRays[from];

	// gen relevant bits + lookup
	unsigned ll blockers = _pext_u64(friendlyPieces | enemyPieces, rook);
	rook = rookLookup[rookLookupOffsets[from] + blockers];
	attack |= rook;
}

void MoveGen::genQueenMovesA(unsigned ll queen, unsigned ll friendlyPieces, unsigned ll enemyPieces, unsigned ll &attack) {

	// gen rays
	uint8_t from = _tzcnt_u64(queen);
	unsigned ll rookBlockers = rookRays[from];
	unsigned ll bishopBlockers = bishopRays[from];

	// gen relevant bits + lookup
	rookBlockers = _pext_u64(friendlyPieces | enemyPieces, rookBlockers);
	bishopBlockers = _pext_u64(friendlyPieces | enemyPieces, bishopBlockers);
	queen = rookLookup[rookLookupOffsets[from] + rookBlockers];
	queen |= bishopLookup[bishopLookupOffsets[from] + bishopBlockers];
	attack |= queen;
}

void MoveGen::genPawnMovesA(unsigned ll pawn, bool color, unsigned ll friendlyPieces, unsigned ll enemyPieces, unsigned ll &attack) {

	// mask pawn
	uint8_t from = _tzcnt_u64(pawn);

	// captures
	if (color) {
		pawn |= (((1ULL << from) >> 9) & 0x7f7f7f7f7f7f7f7f);
		pawn |= (((1ULL << from) >> 7) & 0xfefefefefefefefe);
	} else {
		pawn |= (((1ULL << from) << 9) & 0xfefefefefefefefe);
		pawn |= (((1ULL << from) << 7) & 0x7f7f7f7f7f7f7f7f);
	}
	attack |= pawn;
}