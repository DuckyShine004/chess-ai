#pragma once

#include <cstdint>

#include "engine/board/Colour.hpp"

namespace engine::piece::Pawn {

constexpr uint64_t NOT_A_FILE = 0xFEFEFEFEFEFEFEFEULL;
constexpr uint64_t NOT_H_FILE = 0x7F7F7F7F7F7F7F7FULL;

constexpr uint64_t SECOND_RANK[2] = {
    0x000000000000FF00ULL,
    0x00FF000000000000ULL,
};

[[nodiscard]] inline constexpr uint64_t north(uint64_t square);

[[nodiscard]] inline constexpr uint64_t south(uint64_t square);

[[nodiscard]] inline constexpr uint64_t northAll(uint64_t pawns);

[[nodiscard]] inline constexpr uint64_t southAll(uint64_t pawns);

[[nodiscard]] inline constexpr uint64_t northEast(uint64_t square);

[[nodiscard]] inline constexpr uint64_t northWest(uint64_t square);

[[nodiscard]] inline constexpr uint64_t southEast(uint64_t square);

[[nodiscard]] inline constexpr uint64_t southWest(uint64_t square);

[[nodiscard]] inline constexpr uint64_t getAttacks(int square, engine::board::ColourType side);

[[nodiscard]] inline constexpr bool canSinglePush(int square, engine::board::ColourType side, uint64_t empty);

[[nodiscard]] inline constexpr bool canDoublePush(int square, engine::board::ColourType side, uint64_t empty);

[[nodiscard]] inline constexpr int singlePush(int square, engine::board::ColourType side);

[[nodiscard]] inline constexpr int doublePush(int square, engine::board::ColourType side);

[[nodiscard]] inline constexpr uint64_t getSinglePushAll(uint64_t pawns, engine::board::ColourType side, uint64_t empty);

[[nodiscard]] inline constexpr uint64_t getDoublePushAll(uint64_t pawns, engine::board::ColourType side, uint64_t empty);

[[nodiscard]] inline constexpr uint64_t north(uint64_t square) {
    return square << 8;
}

[[nodiscard]] inline constexpr uint64_t south(uint64_t square) {
    return square >> 8;
}

[[nodiscard]] inline constexpr uint64_t northAll(uint64_t pawns) {
    return pawns << 8;
}

[[nodiscard]] inline constexpr uint64_t southAll(uint64_t pawns) {
    return pawns >> 8;
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

[[nodiscard]] inline constexpr uint64_t getAttacks(int square, engine::board::ColourType side) {
    uint64_t squareU64 = 1ULL << square;

    uint64_t attacks = 0ULL;

    if (side == engine::board::ColourType::WHITE) {
        attacks |= northEast(squareU64);
        attacks |= northWest(squareU64);
    } else {
        attacks |= southEast(squareU64);
        attacks |= southWest(squareU64);
    }

    return attacks;
}

[[nodiscard]] inline constexpr bool canSinglePush(int square, engine::board::ColourType side, uint64_t empty) {
    uint64_t squareU64 = 1ULL << square;

    return (side == engine::board::ColourType::WHITE) ? north(squareU64) & empty : south(squareU64) & empty;
}

[[nodiscard]] inline constexpr bool canDoublePush(int square, engine::board::ColourType side, uint64_t empty) {
    if (!canSinglePush(square, side, empty)) {
        return false;
    }

    uint64_t squareU64 = 1ULL << square;

    squareU64 &= SECOND_RANK[side];

    return (side == engine::board::ColourType::WHITE) ? north(north(squareU64)) & empty : south(south(squareU64)) & empty;
}

[[nodiscard]] inline constexpr int singlePush(int square, engine::board::ColourType side) {
    return (side == engine::board::ColourType::WHITE) ? square + 8 : square - 8;
}

[[nodiscard]] inline constexpr int doublePush(int square, engine::board::ColourType side) {
    return (side == engine::board::ColourType::WHITE) ? square + 16 : square - 16;
}

[[nodiscard]] inline constexpr uint64_t getSinglePushAll(uint64_t pawns, engine::board::ColourType side, uint64_t empty) {
    return (side == engine::board::ColourType::WHITE) ? northAll(pawns) & empty : southAll(pawns) & empty;
}

[[nodiscard]] inline constexpr uint64_t getDoublePushAll(uint64_t pawns, engine::board::ColourType side, uint64_t empty) {
    uint64_t secondRankPawns = pawns & SECOND_RANK[side];

    uint64_t thirdRankPawns = getSinglePushAll(secondRankPawns, side, empty);

    return getSinglePushAll(thirdRankPawns, side, empty);
}

} // namespace engine::piece::Pawn