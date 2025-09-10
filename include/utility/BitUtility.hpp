#pragma once

#include <cstdint>

#include "engine/board/Square.hpp"

#include "utility/BoardUtility.hpp"

namespace utility::BitUtility {

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

extern void printBitBoard(uint64_t bitboard);

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
