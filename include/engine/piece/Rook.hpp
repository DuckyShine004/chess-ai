#pragma once

#include <cstdint>

#include "utility/BitUtility.hpp"
#include "utility/BoardUtility.hpp"

namespace engine::piece::Rook {

// https://github.com/maksimKorzh/chess_programming/blob/master/src/magics/magics.txt
inline constexpr uint64_t MAGIC_MASK[64] = {
    0x8a80104000800020ULL, 0x140002000100040ULL,  0x2801880a0017001ULL,  0x100081001000420ULL,  0x200020010080420ULL,  0x3001c0002010008ULL,  0x8480008002000100ULL, 0x2080088004402900ULL,
    0x800098204000ULL,     0x2024401000200040ULL, 0x100802000801000ULL,  0x120800800801000ULL,  0x208808088000400ULL,  0x2802200800400ULL,    0x2200800100020080ULL, 0x801000060821100ULL,
    0x80044006422000ULL,   0x100808020004000ULL,  0x12108a0010204200ULL, 0x140848010000802ULL,  0x481828014002800ULL,  0x8094004002004100ULL, 0x4010040010010802ULL, 0x20008806104ULL,
    0x100400080208000ULL,  0x2040002120081000ULL, 0x21200680100081ULL,   0x20100080080080ULL,   0x2000a00200410ULL,    0x20080800400ULL,      0x80088400100102ULL,   0x80004600042881ULL,
    0x4040008040800020ULL, 0x440003000200801ULL,  0x4200011004500ULL,    0x188020010100100ULL,  0x14800401802800ULL,   0x2080040080800200ULL, 0x124080204001001ULL,  0x200046502000484ULL,
    0x480400080088020ULL,  0x1000422010034000ULL, 0x30200100110040ULL,   0x100021010009ULL,     0x2002080100110004ULL, 0x202008004008002ULL,  0x20020004010100ULL,   0x2048440040820001ULL,
    0x101002200408200ULL,  0x40802000401080ULL,   0x4008142004410100ULL, 0x2060820c0120200ULL,  0x1001004080100ULL,    0x20c020080040080ULL,  0x2935610830022400ULL, 0x44440041009200ULL,
    0x280001040802101ULL,  0x2100190040002085ULL, 0x80c0084100102001ULL, 0x4024081001000421ULL, 0x20030a0244872ULL,    0x12001008414402ULL,   0x2006104900a0804ULL,  0x1004081002402ULL,
};

// clang-format off
inline constexpr int SHIFT[64] = {
    12, 11, 11, 11, 11, 11, 11, 12,
    11, 10, 10, 10, 10, 10, 10, 11,
    11, 10, 10, 10, 10, 10, 10, 11,
    11, 10, 10, 10, 10, 10, 10, 11,
    11, 10, 10, 10, 10, 10, 10, 11,
    11, 10, 10, 10, 10, 10, 10, 11,
    11, 10, 10, 10, 10, 10, 10, 11,
    12, 11, 11, 11, 11, 11, 11, 12,
};
// clang-format on

inline uint64_t ATTACK_RAYS[64];

inline uint64_t ATTACKS[64][4096];

inline void initialise();

inline void initialiseRays();

inline void initialiseAttacks();

[[nodiscard]] inline uint64_t getRookAttacks(int square, uint64_t occupancy);

[[nodiscard]] inline uint64_t getAttacks(int square, uint64_t occupancy);

inline void initialise() {
    initialiseRays();

    initialiseAttacks();
}

inline void initialiseRays() {
    for (int rank = 0; rank < 8; ++rank) {
        for (int file = 0; file < 8; ++file) {
            uint64_t attacks = 0ULL;

            int square = utility::BoardUtility::getSquare(rank, file);

            for (int dFile = file + 1; dFile <= 6; ++dFile) {
                utility::BitUtility::setBit(attacks, rank, dFile);
            }

            for (int dFile = file - 1; dFile >= 1; --dFile) {
                utility::BitUtility::setBit(attacks, rank, dFile);
            }

            for (int dRank = rank + 1; dRank <= 6; ++dRank) {
                utility::BitUtility::setBit(attacks, dRank, file);
            }

            for (int dRank = rank - 1; dRank >= 1; --dRank) {
                utility::BitUtility::setBit(attacks, dRank, file);
            }

            ATTACK_RAYS[square] = attacks;
        }
    }
}

inline void initialiseAttacks() {
    for (int square = 0; square < 64; ++square) {
        uint64_t rays = ATTACK_RAYS[square];

        int bits = utility::BitUtility::popCount(rays);

        for (int indices = 0; indices < (1 << bits); ++indices) {
            uint64_t occupancy = utility::BitUtility::getOccupancy(indices, bits, rays);

            int magicIndex = (occupancy * MAGIC_MASK[square]) >> (64 - SHIFT[square]);

            ATTACKS[square][magicIndex] = getRookAttacks(square, occupancy);
        }
    }
}

[[nodiscard]] inline uint64_t getRookAttacks(int square, uint64_t occupancy) {
    uint64_t attacks = 0ULL;

    int rank = utility::BoardUtility::getRank(square);
    int file = utility::BoardUtility::getFile(square);

    for (int dFile = file + 1; dFile <= 7; ++dFile) {
        utility::BitUtility::setBit(attacks, rank, dFile);

        if (utility::BitUtility::isBitSet(occupancy, rank, dFile)) {
            break;
        }
    }

    for (int dFile = file - 1; dFile >= 0; --dFile) {
        utility::BitUtility::setBit(attacks, rank, dFile);

        if (utility::BitUtility::isBitSet(occupancy, rank, dFile)) {
            break;
        }
    }

    for (int dRank = rank + 1; dRank <= 7; ++dRank) {
        utility::BitUtility::setBit(attacks, dRank, file);

        if (utility::BitUtility::isBitSet(occupancy, dRank, file)) {
            break;
        }
    }

    for (int dRank = rank - 1; dRank >= 0; --dRank) {
        utility::BitUtility::setBit(attacks, dRank, file);

        if (utility::BitUtility::isBitSet(occupancy, dRank, file)) {
            break;
        }
    }

    return attacks;
}

[[nodiscard]] inline uint64_t getAttacks(int square, uint64_t occupancy) {
    occupancy &= ATTACK_RAYS[square];
    occupancy *= MAGIC_MASK[square];
    occupancy >>= (64 - SHIFT[square]);

    return ATTACKS[square][occupancy];
}

} // namespace engine::piece::Rook
