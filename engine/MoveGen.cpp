#include "MoveGen.hpp"

void MoveGen::genKnightMoves(unsigned ll knight, unsigned ll pieceBoards[12], vector<Move> &moves) {
	uint8_t from = _tzcnt_u64(knight);
	uint64_t ew1 = ((knight << 1) & 0xfefefefefefefefe) | ((knight >> 1) & 0x7f7f7f7f7f7f7f7f);
	uint64_t ew2 = ((knight << 2) & 0xfcfcfcfcfcfcfcfc) | ((knight >> 2) & 0x3f3f3f3f3f3f3f3f);
	knight = (ew2 << 8) | (ew2 >> 8) | (ew1 << 16) | (ew2 >> 16);
}