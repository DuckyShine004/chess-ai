#pragma once

#include <cstdint>

#include "utility/RandomUtility.hpp"

#include "logger/LoggerMacros.hpp"

namespace engine::hash::Zobrist {

inline uint64_t piece[12][64];
inline uint64_t enPassant[64];
inline uint64_t castle[16];
inline uint64_t side;

inline void initialise();

inline void initialise() {
    for (int i = 0; i < 12; ++i) {
        for (int j = 0; j < 64; ++j) {
            piece[i][j] = utility::RandomUtility::getRandomU64();

            // LOG_DEBUG("{:#x}", piece[i][j]);
        }
    }

    for (int i = 0; i < 64; ++i) {
        enPassant[i] = utility::RandomUtility::getRandomU64();
    }

    for (int i = 0; i < 16; ++i) {
        castle[i] = utility::RandomUtility::getRandomU64();
    }

    side = utility::RandomUtility::getRandomU64();
}

// class Zobrist {
//   public:
//     Zobrist(uint64_t seed);
//
//     uint64_t hash(engine::board::Board &board);
//
//     Key &getKey();
//
//   private:
//     Key _key;
// };

} // namespace engine::hash::Zobrist
