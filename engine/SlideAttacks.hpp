#pragma once
#include "includes.hpp"

/**
 * @brief helper class for generating all sliding attacks
 *
 */
namespace SlideAttacks {
	uint64_t southAttacks(uint64_t, uint64_t);

	uint64_t northAttacks(uint64_t, uint64_t);

	uint64_t eastAttacks(uint64_t, uint64_t);

	uint64_t noEaAttacks(uint64_t, uint64_t);

	uint64_t soEaAttacks(uint64_t, uint64_t);

	uint64_t westAttacks(uint64_t, uint64_t);

	uint64_t soWeAttacks(uint64_t, uint64_t);

	uint64_t noWeAttacks(uint64_t, uint64_t);
}