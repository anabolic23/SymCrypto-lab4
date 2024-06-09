#include <iostream>
#include "LFSR.h"
#include "Geffe.h"

const int C1 = 71;
const int C2 = 73;

const int N1 = 222;
const int N2 = 229;

void analyze_dummies() {
    std::string given_sequence = "10101011110010000011011010001000100001001100010101011000011101010010011100001110110111000010101000001001011011101110111101101110010111100011001100101100100011000000010000011110110100111110011011100011001111101000011111001110011110001100111111001010001000011010000000001100110111111111101011100000101111010000001000101110100010111101010111011111011101111001100001100101111011111010001011111101011010110101011110011011000101010101000111100001100111111110111101000011100100011111011100010001010011100011000011100110111010100000111011001010100101001000011100011111100110000001011110010101101101000111000100001111000100101100101000110111000011100000111111000100101011001101000111011011001101001011011110011110000111101011101101010011001111111110101100001010001000100011110010011110001100011110110010111111101110111000000110100101101100000010010100010110110110100001110101110010111001010010100101110000001111100011001001011100001000100101110101111011110111111111100011010001100011000010000010010110110100101100001111110101111011110011010100110000000001000000110110101000000010000111000100011110011001000101111111101100101001101011100110101110000111011111100111011100001000001111111101001000110110001111110010100101101110011111011101000001000101011000011011100100111000110001100111100100000000110111111011101011110001111011100111101010000111111110010101001110010111000010010001111011011000000001100100011110100101100100111001000010000100100000001010000100110010111101110100110111011000111101101010010111001011011010010111001111001101100000111111010011001100001101100010000111100010001111111011100100100100111111000001110100001101011101001111111101001011001000000001000000001000000100001100101001110111000101110101110110100001000111101001110110110100111101100011101110100010001000111101110010000101000110101001010111101110001110000011010101101011111010101101101101011010010010001001100100101000010110010010011101101001011111101011100001101111010011111011000111101011001000001010111101010010111010100010001110001001000000011101011111110100010101001011010100";
    std::bitset<2048> x;
    for (int i = 0; i < 2048; i++) {
        if (given_sequence.at(i) == '1') {
            x.set(i);
        }
    }
    uint64_t x_short = 0;

    for (int i = 0; i < 64; i++) {
        if (given_sequence.at(i) == '1') {
            x_short += 1ull << i;
        }
    }
    uint32_t polynomial1 = ((1u << 3) ^ 1);
    uint8_t size1 = 25;

    uint32_t polynomial2 = ((1u << 6) ^ (1u << 2) ^ (1u << 1) ^ 1u);
    uint8_t size2 = 26;

    uint32_t polynomial3 = ((1u << 5) ^ (1u << 2) ^ (1u << 1) ^ 1u);
    uint8_t size3 = 27;

    LFSR lfsr1(polynomial1, size1);
    LFSR lfsr2(polynomial2, size2);
    LFSR lfsr3(polynomial3, size3);

    auto *generatedL1 = new std::bitset<(1u << 25) + 2048>();
    auto *generatedL2 = new std::bitset<(1u << 26) + 2048>();
    auto *generatedL3 = new std::bitset<(1u << 27) + 2048>();

    for (uint32_t i = 0; i < (1u << 25) + 2048; i++) {
        int bit = lfsr1.step();
        if (bit == 1) {
            generatedL1->set(i);
        }
    }
    for (uint32_t i = 0; i < (1u << 26) + 2048; i++) {
        int bit = lfsr2.step();
        if (bit == 1) {
            generatedL2->set(i);
        }
    }
    for (uint32_t i = 0; i < (1u << 27) + 2048; i++) {
        int bit = lfsr3.step();
        if (bit == 1) {
            generatedL3->set(i);
        }
    }

    std::vector<uint32_t> candidates1;
    std::vector<uint32_t> candidates2;

    std::bitset<N1> stateL1;
    std::bitset<N2> stateL2;

    for (size_t i = 0; i < N1; i++) {
        stateL1.set(i, generatedL1->test(i));
    }
    for (size_t i = 0; i < N2; i++) {
        stateL2.set(i, generatedL2->test(i));
    }

    for (int i = 0; i < 512; i++) {
        std::cout << generatedL1->test(i);
    }
    std::cout << '\n';

    for (uint32_t i = 0; i < (1u << 25); i++) {
        int R = 0;
        for (int j = 0; j < N1; j++) {
            if (!stateL1.test(j) != !x.test(j)) {
                R++;
            }
        }
        if (R < C1) {
            uint32_t candidate = 0;
            for (int j = 0; j < size1; j++) {
                if (stateL1.test(j)) {
                    candidate += 1 << j;
                }
            }
            std::cout << candidate << '\n';
            candidates1.push_back(candidate);
        }
        stateL1 >>= 1;
        stateL1.set(N1 - 1, generatedL1->test(i + N1));
    }
    std::cout << "Finded " << candidates1.size() << " in L1\n";
    for (uint32_t i = 0; i < (1u << 26); i++) {
        int R = 0;
        for (int j = 0; j < N2; j++) {
            if (!stateL2.test(j) != !x.test(j)) {
                R++;
            }
        }
        if (R < C2) {
            uint32_t candidate = 0;
            for (int j = 0; j < size2; j++) {
                if (stateL2.test(j)) {
                    candidate += 1 << j;
                }
            }
            std::cout << candidate << '\n';
            candidates2.push_back(candidate);
        }
        stateL2 >>= 1;
        stateL2.set(N2 - 1, generatedL2->test(i + N2));
    }
    std::cout << "Finded " << candidates2.size() << " in L2\n";

    std::vector<std::bitset<2048>> candidates1sequence;
    std::vector<std::bitset<2048>> candidates2sequence;

    std::vector<uint64_t> candidates1short;
    std::vector<uint64_t> candidates2short;

    for (auto i : candidates1) {
        LFSR L(polynomial1, size1, i);
        uint64_t candidate = 0;
        std::bitset<2048> candidate_sequence;
        for (int j = 0; j < 2048; j++) {
            if (L.step()) {
                candidate_sequence.set(j);
            }
        }
        for (int j = 0; j < 64; j++) {
            if (candidate_sequence.test(j)) {
                candidate += 1ull << j;
            }
        }
        candidates1sequence.push_back(candidate_sequence);
        candidates1short.push_back(candidate);
    }

    for (auto i : candidates2) {
        LFSR L(polynomial2, size2, i);
        uint64_t candidate = 0;
        std::bitset<2048> candidate_sequence;
        for (int j = 0; j < 2048; j++) {
            if (L.step()) {
                candidate_sequence.set(j);
            }
        }
        for (int j = 0; j < 64; j++) {
            if (candidate_sequence.test(j)) {
                candidate += 1ull << j;
            }
        }
        candidates2sequence.push_back(candidate_sequence);
        candidates2short.push_back(candidate);
    }
    std::bitset<2048> stateL3;
    uint64_t stateL3_short = 0;
    for (int i = 0; i < 2048; i++) {
        stateL3.set(i, generatedL3->test(i));
    }
    for (int i = 0; i < 64; i++) {
        if (generatedL3->test(i)) {
            stateL3_short += 1ull << i;
        }
    }
    uint32_t l1 = 0;
    uint32_t l2 = 0;
    uint32_t l3 = 0;



    for (uint32_t i = 0; i < (1u << 27); i++) {
        for (int L1 = 0; L1 < candidates1.size(); L1++) {
            for (int L2 = 0; L2 < candidates2.size(); L2++) {
                uint64_t r = (stateL3_short & candidates1short[L1]) ^ (~stateL3_short & candidates2short[L2]);
                if (r != x_short) {
                    continue;
                }

                std::bitset<2048> R = (stateL3 & candidates1sequence[L1]) ^ (~stateL3 & candidates2sequence[L2]);
                if (R != x) {
                    continue;
                }

                for (int j = 0; j < size3; j++) {
                    if (stateL3.test(j)) {
                        l3 += 1 << j;
                    }
                }
                l1 = candidates1[L1];
                l2 = candidates2[L2];
                break;
            }
        }
        stateL3 >>= 1;
        stateL3.set(2047, generatedL3->test(i + 2048));
        stateL3_short >>= 1;
        if(generatedL3->test(i + 64))
            stateL3_short += 1ull << 63;

    }
    std::cout << "Finded candidates: \nL1 = " << l1 << "\nL2 = " << l2 << "\nL3 = " << l3;

    

}

int main() {
    analyze_dummies();
    /*uint32_t polynomial1 = ((1u << 3) ^ 1u);
    uint8_t size1 = 25;
   
    LFSR lfsr1(polynomial1, size1,2281337);

    for (int i = 0; i < 512; i++) {
        std::cout << (lfsr1.step() == 1) ? '1' : '0';
    }*/

    
    return 0;
}
