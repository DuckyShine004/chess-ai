#include <cstdint>
#include <iostream>

#include "utility/BitUtility.hpp"
#include "utility/BoardUtility.hpp"

namespace utility {

inline void printBitBoard(uint64_t bitboard) {
    for (int rank = 7; rank >= 0; --rank) {
        for (int file = 0; file < 8; ++file) {
            int square = BoardUtility::getSquare(rank, file);

            if (BitUtility::isBitSet(bitboard, square)) {
                std::cout << "1 ";
            } else {
                std::cout << "0 ";
            }
        }

        std::cout << '\n';
    }

    std::cout << '\n';
}

} // namespace utility
