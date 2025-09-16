#pragma once

#include <cstdint>

#include "engine/board/Square.hpp"

namespace engine::piece::Knight {

inline constexpr uint64_t NOT_A_FILE = 0xFEFEFEFEFEFEFEFEULL;
inline constexpr uint64_t NOT_AB_FILE = 0xFCFCFCFCFCFCFCFCULL;
inline constexpr uint64_t NOT_H_FILE = 0x7F7F7F7F7F7F7F7FULL;
inline constexpr uint64_t NOT_GH_FILE = 0x3F3F3F3F3F3F3F3FULL;

inline uint64_t ATTACKS[64];

inline void initialise();

[[nodiscard]] inline constexpr uint64_t getAttacks(int square);

[[nodiscard]] inline constexpr uint64_t northNorthEast(uint64_t square);

[[nodiscard]] inline constexpr uint64_t northEastEast(uint64_t square);

[[nodiscard]] inline constexpr uint64_t southEastEast(uint64_t square);

[[nodiscard]] inline constexpr uint64_t southSouthEast(uint64_t square);

[[nodiscard]] inline constexpr uint64_t northNorthWest(uint64_t square);

[[nodiscard]] inline constexpr uint64_t northWestWest(uint64_t square);

[[nodiscard]] inline constexpr uint64_t southWestWest(uint64_t square);

[[nodiscard]] inline constexpr uint64_t southSouthWest(uint64_t square);

inline void initialise() {
    for (int square = 0; square < 64; ++square) {
        ATTACKS[square] = getAttacks(square);
    }
}

[[nodiscard]] inline constexpr uint64_t getAttacks(int square) {
    uint64_t squareU64 = engine::board::BITBOARD_SQUARES[square];

    uint64_t attacks = 0ULL;

    attacks |= northNorthEast(squareU64);
    attacks |= northEastEast(squareU64);
    attacks |= southEastEast(squareU64);
    attacks |= southSouthEast(squareU64);
    attacks |= northNorthWest(squareU64);
    attacks |= northWestWest(squareU64);
    attacks |= southWestWest(squareU64);
    attacks |= southSouthWest(squareU64);

    return attacks;
}

[[nodiscard]] inline constexpr uint64_t northNorthEast(uint64_t square) {
    return (square << 17) & NOT_A_FILE;
}

[[nodiscard]] inline constexpr uint64_t northEastEast(uint64_t square) {
    return (square << 10) & NOT_AB_FILE;
}

[[nodiscard]] inline constexpr uint64_t southEastEast(uint64_t square) {
    return (square >> 6) & NOT_AB_FILE;
}

[[nodiscard]] inline constexpr uint64_t southSouthEast(uint64_t square) {
    return (square >> 15) & NOT_A_FILE;
}

[[nodiscard]] inline constexpr uint64_t northNorthWest(uint64_t square) {
    return (square << 15) & NOT_H_FILE;
}

[[nodiscard]] inline constexpr uint64_t northWestWest(uint64_t square) {
    return (square << 6) & NOT_GH_FILE;
}

[[nodiscard]] inline constexpr uint64_t southWestWest(uint64_t square) {
    return (square >> 10) & NOT_GH_FILE;
}

[[nodiscard]] inline constexpr uint64_t southSouthWest(uint64_t square) {
    return (square >> 17) & NOT_H_FILE;
}

} // namespace engine::piece::Knight
