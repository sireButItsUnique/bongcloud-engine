#pragma once
#include <intrin.h>
#include <iomanip>
#include <iostream>
#include <stdint.h>
#include <string>
#include <time.h>
#include <utility>
#include <vector>
using namespace std;

#define pawnWhite 0
#define pawnBlack 1
#define knightWhite 2
#define knightBlack 3
#define bishopWhite 4
#define bishopBlack 5
#define rookWhite 6
#define rookBlack 7
#define queenWhite 8
#define queenBlack 9
#define kingWhite 10
#define kingBlack 11
#define NORTH 0
#define NORTHEAST 1
#define EAST 2
#define SOUTHEAST 3
#define SOUTH 4
#define SOUTHWEST 5
#define WEST 6
#define NORTHWEST 7
#define ll long long

#define TO_SQUARE(col, row) 8 * (row - '1') + 7 - (col - 'a')
#define TO_ALGEBRA(sq) string(1, 'a' + (7 - (sq % 8))) + string(1, '1' + (sq / 8))