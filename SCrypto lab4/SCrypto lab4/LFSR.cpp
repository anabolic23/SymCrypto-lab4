#include "LFSR.h"
#include <iostream>

LFSR::LFSR(uint32_t polynomial, uint8_t n) : poly(polynomial), size(n - 1) {
	state = 1;
};
LFSR::LFSR(uint32_t polynomial, uint8_t n, uint32_t initial_state) : poly(polynomial), size(n - 1), state(initial_state) {}

uint8_t LFSR::step() {
    uint8_t result = state & 1;
    uint8_t new_bit = 0;
    for (uint8_t i = 0; i < size; ++i) {
        if (state & (1 << i)) {
            new_bit ^= (poly >> i) & 1;
        }
    }
    state = (state >> 1) | (new_bit << (size - 1));
    return result;
}

uint32_t LFSR::generate(uint32_t bits) {
    uint32_t result = 0;
    for (uint32_t i = 0; i < bits; ++i) {
        result = (result << 1) | step();
    }
    return result;
}
