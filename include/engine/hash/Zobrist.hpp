#pragma once

#include <cstdint>

#include "engine/board/Colour.hpp"

#include "utility/BitUtility.hpp"
#include "utility/RandomUtility.hpp"

#include "logger/LoggerMacros.hpp"

namespace engine::hash::Zobrist {

inline uint64_t pieceKeys[2][6][64];
inline uint64_t enPassantKeys[64];
inline uint64_t castleKeys[16];
inline uint64_t sideKey;

inline void initialise();

[[nodiscard]] inline uint64_t hash(const uint64_t bitboards[2][6], uint8_t castleRights, int enPassantSquare, engine::board::ColourType side);

inline void initialise() {
    for (int sideIndex = 0; sideIndex < 2; ++sideIndex) {
        for (int pieceIndex = 0; pieceIndex < 6; ++pieceIndex) {
            for (int square = 0; square < 64; ++square) {
                pieceKeys[sideIndex][pieceIndex][square] = utility::RandomUtility::getRandomU64();
            }
        }
    }

    for (int i = 0; i < 64; ++i) {
        enPassantKeys[i] = utility::RandomUtility::getRandomU64();
    }

    for (int i = 0; i < 16; ++i) {
        castleKeys[i] = utility::RandomUtility::getRandomU64();
    }

    sideKey = utility::RandomUtility::getRandomU64();
}

[[nodiscard]] inline uint64_t hash(const uint64_t bitboards[2][6], uint8_t castleRights, int enPassantSquare, engine::board::ColourType side) {
    uint64_t hash = 0ULL;

    for (int sideIndex = 0; sideIndex < 2; ++sideIndex) {
        for (int pieceIndex = 0; pieceIndex < 6; ++pieceIndex) {
            uint64_t bitboard = bitboards[sideIndex][pieceIndex];

            while (bitboard) {
                int square = utility::BitUtility::popLSB(bitboard);

                hash ^= pieceKeys[sideIndex][pieceIndex][square];
            }
        }
    }

    hash ^= castleKeys[castleRights];

    if (enPassantSquare != -1) {
        hash ^= enPassantKeys[enPassantSquare];
    }

    if (side == engine::board::ColourType::BLACK) {
        hash ^= sideKey;
    }

    return hash;
}

} // namespace engine::hash::Zobrist
