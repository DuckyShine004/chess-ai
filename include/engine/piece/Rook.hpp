#pragma once

#include <cstdint>

#include "utility/BitUtility.hpp"
#include "utility/BoardUtility.hpp"

namespace engine::piece::Rook {

inline uint64_t RAY_EAST[64];
inline uint64_t RAY_WEST[64];
inline uint64_t RAY_NORTH[64];
inline uint64_t RAY_SOUTH[64];

inline void initialiseRays();

[[nodiscard]] inline uint64_t getAttacks(int square, uint64_t occupancy);

inline void initialiseRays() {
    for (int rank = 0; rank < 8; ++rank) {
        for (int file = 0; file < 8; ++file) {
            int square = utility::BoardUtility::getSquare(rank, file);

            RAY_EAST[square] = 0ULL;
            for (int dFile = file + 1; dFile < 8; ++dFile) {
                utility::BitUtility::setBit(RAY_EAST[square], rank, dFile);
            }

            RAY_WEST[square] = 0ULL;
            for (int dFile = file - 1; dFile >= 0; --dFile) {
                utility::BitUtility::setBit(RAY_WEST[square], rank, dFile);
            }

            RAY_NORTH[square] = 0ULL;
            for (int dRank = rank + 1; dRank < 8; ++dRank) {
                utility::BitUtility::setBit(RAY_NORTH[square], dRank, file);
            }

            RAY_SOUTH[square] = 0ULL;
            for (int dRank = rank - 1; dRank >= 0; --dRank) {
                utility::BitUtility::setBit(RAY_SOUTH[square], dRank, file);
            }
        }
    }
}

[[nodiscard]] inline uint64_t getAttacks(int square, uint64_t occupancy) {
    uint64_t attacks = 0ULL;

    uint64_t rays = RAY_EAST[square];
    uint64_t blockers = rays & occupancy;

    if (blockers) {
        rays &= ~RAY_EAST[utility::BitUtility::getLSBIndex(blockers)];
    }

    attacks |= rays;

    rays = RAY_WEST[square];
    blockers = rays & occupancy;

    if (blockers) {
        rays &= ~RAY_WEST[utility::BitUtility::getMSBIndex(blockers)];
    }

    attacks |= rays;

    rays = RAY_NORTH[square];
    blockers = rays & occupancy;

    if (blockers) {
        rays &= ~RAY_NORTH[utility::BitUtility::getLSBIndex(blockers)];
    }

    attacks |= rays;

    rays = RAY_SOUTH[square];
    blockers = rays & occupancy;

    if (blockers) {
        rays &= ~RAY_SOUTH[utility::BitUtility::getMSBIndex(blockers)];
    }

    attacks |= rays;

    return attacks;
}

} // namespace engine::piece::Rook
