#pragma once

#include <cstdint>

#include "utility/BitUtility.hpp"
#include "utility/BoardUtility.hpp"

namespace engine::piece::Bishop {

inline uint64_t RAY_NORTH_EAST[64];
inline uint64_t RAY_NORTH_WEST[64];
inline uint64_t RAY_SOUTH_EAST[64];
inline uint64_t RAY_SOUTH_WEST[64];

inline constexpr void initialiseRays();

[[nodiscard]] inline constexpr uint64_t getAttacks(int square, uint64_t occupancy);

inline constexpr void initialiseRays() {
    for (int rank = 0; rank < 8; ++rank) {
        for (int file = 0; file < 8; ++file) {
            int square = utility::BoardUtility::getSquare(rank, file);

            RAY_NORTH_EAST[square] = 0ULL;
            for (int dRank = rank + 1, dFile = file + 1; dRank < 8 && dFile < 8; ++dRank, ++dFile) {
                utility::BitUtility::setBit(RAY_NORTH_EAST[square], dRank, dFile);
            }

            RAY_NORTH_WEST[square] = 0ULL;
            for (int dRank = rank + 1, dFile = file - 1; dRank < 8 && dFile >= 0; ++dRank, --dFile) {
                utility::BitUtility::setBit(RAY_NORTH_WEST[square], dRank, dFile);
            }

            RAY_SOUTH_EAST[square] = 0ULL;
            for (int dRank = rank - 1, dFile = file + 1; dRank >= 0 && dFile < 8; --dRank, ++dFile) {
                utility::BitUtility::setBit(RAY_SOUTH_EAST[square], dRank, dFile);
            }

            RAY_SOUTH_WEST[square] = 0ULL;
            for (int dRank = rank - 1, dFile = file - 1; dRank >= 0 && dFile >= 0; --dRank, --dFile) {
                utility::BitUtility::setBit(RAY_SOUTH_WEST[square], dRank, dFile);
            }
        }
    }
}

[[nodiscard]] inline constexpr uint64_t getAttacks(int square, uint64_t occupancy) {
    uint64_t attacks = 0ULL;

    uint64_t rays = RAY_NORTH_EAST[square];
    uint64_t blockers = rays & occupancy;

    if (blockers) {
        rays &= ~RAY_NORTH_EAST[utility::BitUtility::getLSBIndex(blockers)];
    }

    attacks |= rays;

    rays = RAY_NORTH_WEST[square];
    blockers = rays & occupancy;

    if (blockers) {
        rays &= ~RAY_NORTH_WEST[utility::BitUtility::getLSBIndex(blockers)];
    }

    attacks |= rays;

    rays = RAY_SOUTH_EAST[square];
    blockers = rays & occupancy;

    if (blockers) {
        rays &= ~RAY_SOUTH_EAST[utility::BitUtility::getMSBIndex(blockers)];
    }

    attacks |= rays;

    rays = RAY_SOUTH_WEST[square];
    blockers = rays & occupancy;

    if (blockers) {
        rays &= ~RAY_SOUTH_WEST[utility::BitUtility::getMSBIndex(blockers)];
    }

    attacks |= rays;

    return attacks;
}

} // namespace engine::piece::Bishop