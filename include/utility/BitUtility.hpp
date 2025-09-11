#pragma once

#include <cstdint>

#include "engine/board/Square.hpp"

#include "utility/BoardUtility.hpp"

namespace utility::BitUtility {

[[nodiscard]] inline constexpr bool isBitSet(uint64_t value, int rank, int file);

[[nodiscard]] inline constexpr bool isBitSet(uint64_t value, int position);

[[nodiscard]] inline constexpr int getLSBIndex(uint64_t value);

[[nodiscard]] inline constexpr int getMSBIndex(uint64_t value);

[[nodiscard]] inline constexpr int popCount(uint64_t value);

inline constexpr void setBit(uint64_t &value, int position);

inline constexpr void setBit(uint64_t &value, int rank, int file);

inline constexpr void clearBit(uint64_t &value, int position);

inline constexpr void clearBit(uint64_t &value, int rank, int file);

inline constexpr void clearBit(uint8_t &value, int position);

[[nodiscard]] inline constexpr int popLSB(uint64_t &value);

[[nodiscard]] inline constexpr uint64_t getOccupancy(int indices, int bits, uint64_t rays);

extern void printBitBoard(uint64_t bitboard);

[[nodiscard]] inline constexpr bool isBitSet(uint64_t value, int rank, int file) {
    return isBitSet(value, BoardUtility::getSquare(rank, file));
}
[[nodiscard]] inline constexpr bool isBitSet(uint64_t value, int position) {
    return value & engine::board::BITBOARD_SQUARES[position];
}

[[nodiscard]] inline constexpr int getLSBIndex(uint64_t value) {
    return __builtin_ctzll(value);
}

[[nodiscard]] inline constexpr int getMSBIndex(uint64_t value) {
    return 63 - __builtin_clzll(value);
}

[[nodiscard]] inline constexpr int popCount(uint64_t value) {
    return __builtin_popcountll(value);
}

[[nodiscard]] inline constexpr uint64_t getOccupancy(int indices, int bits, uint64_t rays) {
    uint64_t occupancy = 0ULL;

    for (int index = 0; index < bits; ++index) {
        int square = popLSB(rays);

        if (indices & (1 << index)) {
            setBit(occupancy, square);
        }
    }

    return occupancy;
}

inline constexpr void setBit(uint64_t &value, int position) {
    value |= engine::board::BITBOARD_SQUARES[position];
}

inline constexpr void setBit(uint64_t &value, int rank, int file) {
    setBit(value, BoardUtility::getSquare(rank, file));
}

inline constexpr void clearBit(uint64_t &value, int position) {
    value &= engine::board::INVERTED_BITBOARD_SQUARES[position];
}

inline constexpr void clearBit(uint64_t &value, int rank, int file) {
    clearBit(value, BoardUtility::getSquare(rank, file));
}

inline constexpr void clearBit(uint8_t &value, int position) {
    value &= engine::board::INVERTED_BITBOARD_SQUARES[position];
}

[[nodiscard]] inline constexpr int popLSB(uint64_t &value) {
    int lsbIndex = getLSBIndex(value);

    value &= (value - 1);

    return lsbIndex;
}

} // namespace utility::BitUtility
