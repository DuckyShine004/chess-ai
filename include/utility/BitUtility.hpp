#pragma once

#include <cstdint>

#include "utility/BoardUtility.hpp"

namespace utility::BitUtility {

[[nodiscard]] inline constexpr bool isBitSet(uint64_t x, int position);

void printBitBoard(uint64_t bitboard);

[[nodiscard]] inline constexpr bool isBitSet(uint64_t x, int position) {
    return x & (1ULL << position);
}

void printBitBoard(uint64_t bitboard) {
    for (int rank = 7; rank >= 0; --rank) {
        for (int file = 0; file < 8; ++file) {
            int square = BoardUtility::getSquare(rank, file);

            if (isBitSet(bitboard, square)) {
                std::cout << "1 ";
            } else {
                std::cout << "0 ";
            }
        }

        std::cout << '\n';
    }

    std::cout << '\n';
}

} // namespace utility::BitUtility