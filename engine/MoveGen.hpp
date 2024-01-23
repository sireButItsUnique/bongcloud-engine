#pragma once
#include <vector>
using namespace std;

#define Move pair<uint8_t, uint8_t>
#define ll long long

/**
 * @brief helper namespace for generating moves of each piece
 *
 */
namespace MoveGen {

/**
 * @brief generates knight moves
 *
 * @param knight pieceboard representing the position of the knight
 * @param pieceBoards pieceboards
 * @param moves resulting moves
 */
void genKnightMoves(unsigned ll, unsigned ll[12], vector<Move> &);

/**
 * @brief generates king moves
 *
 * @param king pieceboard representing the position of the king
 * @param pieceBoards pieceboards
 * @param moves resulting moves
 */
void genKingMoves(unsigned ll, unsigned ll[12], vector<Move> &);

/**
 * @brief generates bishop moves
 *
 * @param bishop pieceboard representing the position of the bishop
 * @param pieceBoards pieceboards
 * @param moves resulting moves
 */
void genBishop(unsigned ll, unsigned ll[12], vector<Move> &);

/**
 * @brief generates rook moves
 *
 * @param rook pieceboard representing the position of the rook
 * @param pieceBoards pieceboards
 * @param moves resulting moves
 */
void genRookMoves(unsigned ll, unsigned ll[12], vector<Move> &);

/**
 * @brief generates queen moves
 *
 * @param queen pieceboard representing the position of the queen
 * @param pieceBoards pieceboards
 * @param moves resulting moves
 */
void genQueenMoves(unsigned ll, unsigned ll[12], vector<Move> &);

/**
 * @brief generates pawn moves
 *
 * @param pawn pieceboard representing the position of the pawn
 * @param pieceBoards pieceboards
 * @param moves resulting moves
 */
void genPawnMoves(unsigned ll, unsigned ll[12], vector<Move> &);

} // namespace MoveGen
