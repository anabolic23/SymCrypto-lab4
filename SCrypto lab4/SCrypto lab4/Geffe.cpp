#include "Geffe.h"
#include <iostream>

Geffe::Geffe(LFSR l1, LFSR l2, LFSR l3) : L1(l1), L2(l2), L3(l3) {}

uint8_t Geffe::step() {
    uint8_t x1 = L1.step();
    uint8_t x2 = L2.step();
    uint8_t x3 = L3.step();
    return (x1 & x3) ^ ((1 ^ x3) & x2);
}

uint32_t Geffe::generate(uint32_t bits) {
    uint32_t result = 0;
    for (uint32_t i = 0; i < bits; ++i) {
        result = (result << 1) | step();
    }
    return result;
}