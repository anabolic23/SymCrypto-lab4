#include <iostream>
#include "LFSR.h"
#include "Geffe.h"


int main() {
	uint32_t polynomial1 = ((1u << 3) ^ 1);
    uint8_t size1 = 25;

    uint32_t polynomial2 = ((1u << 6) ^ (1u << 2) ^ (1u << 1) ^ 1u);
    uint8_t size2 = 26;

    uint32_t polynomial3 = ((1u << 5) ^ (1u << 2) ^ (1u << 1) ^ 1u);
    uint8_t size3 = 27;
    
    LFSR lfsr1(polynomial1, size1);
    LFSR lfsr2(polynomial2, size2);
    LFSR lfsr3(polynomial3, size3);

    Geffe geffe(lfsr1, lfsr2, lfsr3);
    
    for (int i = 0; i < 100; i++) {
        std::cout << ((geffe.step() == 1) ? '1': '0');
    }

    return 0;
}
