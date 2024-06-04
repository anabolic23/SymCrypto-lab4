#pragma once
#include <iostream>
#include <vector>
#include <bitset>

class LFSR {
private:
	uint32_t poly;
	uint32_t state;
	uint8_t size;

public:
	LFSR(uint32_t polynomial, uint8_t n);
	LFSR(uint32_t polynomial, uint8_t n, uint32_t initial_state);
    uint8_t step();
	uint32_t generate(uint32_t bits);
};

