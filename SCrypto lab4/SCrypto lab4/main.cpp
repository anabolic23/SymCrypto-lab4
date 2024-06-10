#include <iostream>
#include "LFSR.h"
#include "Geffe.h"

const int C1 = 71;
const int C2 = 73;

const int N1 = 222;
const int N2 = 229;

void analyze_dummies() {
    std::string given_sequence = "00001100111000011100011101011000111100111111100001000100100000000000011000011001110000000101010001001000010000111100000100000110011111110011010000001000010010100111001101010011110011010100101100000111110110000101111000110111100101111111000101101110110000100111010001000111100100101011011011000010111000111001010010010010001001011000101101101101000001100010010011010000001000000011001000010000101110000101110100101011110100000000010100101111101110000000001001101011010111101101001110001001110010110010011000000011010110011010110100101110011001100110000111100110011011001100101000001101001111010010100101101000000101000111010101000000010111111111111011011110011101000001010000101011100110101100111011001001001111100110011100001110000000010011101000111001111010011110011111111000000101011100111110010000101011111010001111100110010111110000011010011101011000010110111101001110110111010111110000011110000010100011111101111100001011111110111011001111110000100001111011111000110100011101110001001011110101011001101110001000000011011010100111010110100011001011011101000000010100000101100001110000001010001111000110011101001000110011101111001100000100111001000110100100000110110001010001000011111001011111001111101110001011001010001110111100101111001101000001111101000100110001111101000111001111000111000101001111011100010101110011011110011111110100000000111111111001111011001011001101011101011010100111110111110010001010100000000111110100111101010111100101110011101010110101011001100011010100101000001111110000100101110101001111001010011001100010110001010101111000101100011110101100100110110001111001110110000100001100010101100110111010110101101100000101110010011101110111010100010001010111011000010100010110100010100000000101001110101000010011101010111110000000100100011110001001000110111110010101101011001001100010010100010101011100011110010111101101001101111000000100000100010101100100001110101000110100100011111100010100111101010110001111110101011101100110110111010001110000110000110001010100101000000001001001000110101111000111101100011100101101100111";
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

    
    LFSR L1(polynomial1, size1,l1);
    LFSR L2(polynomial2, size2,l2);
    LFSR L3(polynomial3, size3,l3);

    Geffe geffe(L1, L2, L3);

    std::cout << "\nSequence from candidates:\n";
    for (int i = 0; i < 2048; i++) {
        std::cout << (int)(geffe.step());
    }
    std::cout << "\nGiven sequence:\n" << given_sequence;
    

    

}

int main() {
    analyze_dummies();
    return 0;
}
