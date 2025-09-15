#pragma once

#include <cstdint>

namespace engine::piece::King {

inline constexpr uint64_t NOT_A_FILE = 0xFEFEFEFEFEFEFEFEULL;
inline constexpr uint64_t NOT_H_FILE = 0x7F7F7F7F7F7F7F7FULL;

inline uint64_t ATTACKS[64];

inline void initialise();

[[nodiscard]] inline constexpr uint64_t getAttacks(int square);

[[nodiscard]] inline constexpr uint64_t north(uint64_t square);

[[nodiscard]] inline constexpr uint64_t east(uint64_t square);

[[nodiscard]] inline constexpr uint64_t south(uint64_t square);

[[nodiscard]] inline constexpr uint64_t west(uint64_t square);

[[nodiscard]] inline constexpr uint64_t northEast(uint64_t square);

[[nodiscard]] inline constexpr uint64_t northWest(uint64_t square);

[[nodiscard]] inline constexpr uint64_t southEast(uint64_t square);

[[nodiscard]] inline constexpr uint64_t southWest(uint64_t square);

inline void initialise() {
    for (int square = 0; square < 64; ++square) {
        ATTACKS[square] = getAttacks(square);
    }
}

[[nodiscard]] inline constexpr uint64_t north(uint64_t square) {
    return square << 8;
}

[[nodiscard]] inline constexpr uint64_t east(uint64_t square) {
    return (square << 1) & NOT_A_FILE;
}

[[nodiscard]] inline constexpr uint64_t south(uint64_t square) {
    return square >> 8;
}

[[nodiscard]] inline constexpr uint64_t west(uint64_t square) {
    return (square >> 1) & NOT_H_FILE;
}

[[nodiscard]] inline constexpr uint64_t northEast(uint64_t square) {
    return (square << 9) & NOT_A_FILE;
}

[[nodiscard]] inline constexpr uint64_t northWest(uint64_t square) {
    return (square << 7) & NOT_H_FILE;
}

[[nodiscard]] inline constexpr uint64_t southEast(uint64_t square) {
    return (square >> 7) & NOT_A_FILE;
}

[[nodiscard]] inline constexpr uint64_t southWest(uint64_t square) {
    return (square >> 9) & NOT_H_FILE;
}

[[nodiscard]] inline constexpr uint64_t getAttacks(int square) {
    uint64_t squareU64 = 1ULL << square;

    uint64_t attacks = 0ULL;

    attacks |= north(squareU64);
    attacks |= east(squareU64);
    attacks |= south(squareU64);
    attacks |= west(squareU64);
    attacks |= northEast(squareU64);
    attacks |= northWest(squareU64);
    attacks |= southEast(squareU64);
    attacks |= southWest(squareU64);

    return attacks;
}

} // namespace engine::piece::King
