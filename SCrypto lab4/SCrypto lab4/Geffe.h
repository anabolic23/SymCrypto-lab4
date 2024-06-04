#pragma once
#include <iostream>
#include <vector>
#include <bitset>
#include "LFSR.h"

class Geffe {
private:
	LFSR L1;
	LFSR L2;
	LFSR L3;
public:
	Geffe(LFSR l1, LFSR l2, LFSR l3);
	uint8_t step();
	uint32_t generate(uint32_t bits);
};