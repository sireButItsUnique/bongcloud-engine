#include "SlideAttacks.hpp"

uint64_t SlideAttacks::southAttacks(uint64_t rooks, uint64_t blockers) {
	uint64_t flood = rooks;
	uint64_t empty = ~blockers;
	flood |= rooks = (rooks >> 8) & empty;
	flood |= rooks = (rooks >> 8) & empty;
	flood |= rooks = (rooks >> 8) & empty;
	flood |= rooks = (rooks >> 8) & empty;
	flood |= rooks = (rooks >> 8) & empty;
	flood |= (rooks >> 8) & empty;
	return flood >> 8;
}

uint64_t SlideAttacks::northAttacks(uint64_t rooks, uint64_t blockers) {
	uint64_t flood = rooks;
	uint64_t empty = ~blockers;
	flood |= rooks = (rooks << 8) & empty;
	flood |= rooks = (rooks << 8) & empty;
	flood |= rooks = (rooks << 8) & empty;
	flood |= rooks = (rooks << 8) & empty;
	flood |= rooks = (rooks << 8) & empty;
	flood |= (rooks << 8) & empty;
	return flood << 8;
}

uint64_t SlideAttacks::eastAttacks(uint64_t rooks, uint64_t blockers) {
	const uint64_t notA = 0xfefefefefefefefeULL;
	uint64_t flood = rooks;
	uint64_t empty = ~blockers;
	empty &= notA;
	flood |= rooks = (rooks << 1) & empty;
	flood |= rooks = (rooks << 1) & empty;
	flood |= rooks = (rooks << 1) & empty;
	flood |= rooks = (rooks << 1) & empty;
	flood |= rooks = (rooks << 1) & empty;
	flood |= (rooks << 1) & empty;
	return (flood << 1) & notA;
}

uint64_t SlideAttacks::noEaAttacks(uint64_t bishops, uint64_t blockers) {
	const uint64_t notA = 0xfefefefefefefefeULL;
	uint64_t flood = bishops;
	uint64_t empty = ~blockers;
	empty &= notA;
	flood |= bishops = (bishops << 9) & empty;
	flood |= bishops = (bishops << 9) & empty;
	flood |= bishops = (bishops << 9) & empty;
	flood |= bishops = (bishops << 9) & empty;
	flood |= bishops = (bishops << 9) & empty;
	flood |= (bishops << 9) & empty;
	return (flood << 9) & notA;
}

uint64_t SlideAttacks::soEaAttacks(uint64_t bishops, uint64_t blockers) {
	const uint64_t notA = 0xfefefefefefefefeULL;
	uint64_t flood = bishops;
	uint64_t empty = ~blockers;
	empty &= notA;
	flood |= bishops = (bishops >> 7) & empty;
	flood |= bishops = (bishops >> 7) & empty;
	flood |= bishops = (bishops >> 7) & empty;
	flood |= bishops = (bishops >> 7) & empty;
	flood |= bishops = (bishops >> 7) & empty;
	flood |= (bishops >> 7) & empty;
	return (flood >> 7) & notA;
}

uint64_t SlideAttacks::westAttacks(uint64_t rooks, uint64_t blockers) {
	const uint64_t notH = 0x7f7f7f7f7f7f7f7fULL;
	uint64_t flood = rooks;
	uint64_t empty = ~blockers;
	empty &= notH;
	flood |= rooks = (rooks >> 1) & empty;
	flood |= rooks = (rooks >> 1) & empty;
	flood |= rooks = (rooks >> 1) & empty;
	flood |= rooks = (rooks >> 1) & empty;
	flood |= rooks = (rooks >> 1) & empty;
	flood |= (rooks >> 1) & empty;
	return (flood >> 1) & notH;
}

uint64_t SlideAttacks::soWeAttacks(uint64_t bishops, uint64_t blockers) {
	const uint64_t notH = 0x7f7f7f7f7f7f7f7fULL;
	uint64_t flood = bishops;
	uint64_t empty = ~blockers;
	empty &= notH;
	flood |= bishops = (bishops >> 9) & empty;
	flood |= bishops = (bishops >> 9) & empty;
	flood |= bishops = (bishops >> 9) & empty;
	flood |= bishops = (bishops >> 9) & empty;
	flood |= bishops = (bishops >> 9) & empty;
	flood |= (bishops >> 9) & empty;
	return (flood >> 9) & notH;
}

uint64_t SlideAttacks::noWeAttacks(uint64_t bishops, uint64_t blockers) {
	const uint64_t notH = 0x7f7f7f7f7f7f7f7fULL;
	uint64_t flood = bishops;
	uint64_t empty = ~blockers;
	empty &= notH;
	flood |= bishops = (bishops << 7) & empty;
	flood |= bishops = (bishops << 7) & empty;
	flood |= bishops = (bishops << 7) & empty;
	flood |= bishops = (bishops << 7) & empty;
	flood |= bishops = (bishops << 7) & empty;
	flood |= (bishops << 7) & empty;
	return (flood << 7) & notH;
}