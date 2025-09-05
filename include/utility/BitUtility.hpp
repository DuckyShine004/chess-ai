#pragma once

#include <cstdint>

#include "utility/BoardUtility.hpp"

namespace utility::BitUtility {

[[nodiscard]] inline constexpr bool isBitSet(uint64_t value, int position);

[[nodiscard]] inline constexpr int getLSBIndex(uint64_t value);

[[nodiscard]] inline constexpr int getMSBIndex(uint64_t value);

inline constexpr void setBit(uint64_t &value, int position);

inline constexpr void setBit(uint64_t &value, int rank, int file);

void printBitBoard(uint64_t bitboard);

[[nodiscard]] inline constexpr bool isBitSet(uint64_t value, int position) {
    return value & (1ULL << position);
}

[[nodiscard]] inline constexpr int getLSBIndex(uint64_t value) {
    return __builtin_ctzll(value);
}

[[nodiscard]] inline constexpr int getMSBIndex(uint64_t value) {
    return 63 - __builtin_clzll(value);
}

inline constexpr void setBit(uint64_t &value, int position) {
    value |= (1ULL << position);
}

inline constexpr void setBit(uint64_t &value, int rank, int file) {
    setBit(value, BoardUtility::getSquare(rank, file));
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