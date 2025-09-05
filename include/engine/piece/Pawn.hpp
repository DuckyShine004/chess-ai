#pragma once

#include <cstdint>

#include "engine/board/Colour.hpp"

namespace engine::piece::Pawn {

constexpr uint64_t NOT_A_FILE = 0xFEFEFEFEFEFEFEFEULL;
constexpr uint64_t NOT_H_FILE = 0x7F7F7F7F7F7F7F7FULL;

[[nodiscard]] inline constexpr uint64_t northEast(uint64_t square);

[[nodiscard]] inline constexpr uint64_t northWest(uint64_t square);

[[nodiscard]] inline constexpr uint64_t southEast(uint64_t square);

[[nodiscard]] inline constexpr uint64_t southWest(uint64_t square);

[[nodiscard]] inline constexpr uint64_t getAttacks(int square, engine::board::Colour colour);

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

[[nodiscard]] inline constexpr uint64_t getAttacks(int square, engine::board::Colour colour) {
    uint64_t squareU64 = 1ULL << square;

    uint64_t attacks = 0ULL;

    if (colour == engine::board::Colour::WHITE) {
        attacks |= northEast(squareU64);
        attacks |= northWest(squareU64);
    } else {
        attacks |= southEast(squareU64);
        attacks |= southWest(squareU64);
    }

    return attacks;
}

} // namespace engine::piece::Pawn