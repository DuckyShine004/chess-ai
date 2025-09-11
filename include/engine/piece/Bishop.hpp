#pragma once

#include <cstdint>

#include "utility/BitUtility.hpp"
#include "utility/BoardUtility.hpp"

// https://github.com/maksimKorzh/chess_programming/blob/master/src/magics/magics.txt
namespace engine::piece::Bishop {

inline constexpr uint64_t MAGIC_MASK[64] = {
    0x40040844404084ULL,   0x2004208a004208ULL,   0x10190041080202ULL,   0x108060845042010ULL,  0x581104180800210ULL,  0x2112080446200010ULL, 0x1080820820060210ULL, 0x3c0808410220200ULL,
    0x4050404440404ULL,    0x21001420088ULL,      0x24d0080801082102ULL, 0x1020a0a020400ULL,    0x40308200402ULL,      0x4011002100800ULL,    0x401484104104005ULL,  0x801010402020200ULL,
    0x400210c3880100ULL,   0x404022024108200ULL,  0x810018200204102ULL,  0x4002801a02003ULL,    0x85040820080400ULL,   0x810102c808880400ULL, 0xe900410884800ULL,    0x8002020480840102ULL,
    0x220200865090201ULL,  0x2010100a02021202ULL, 0x152048408022401ULL,  0x20080002081110ULL,   0x4001001021004000ULL, 0x800040400a011002ULL, 0xe4004081011002ULL,   0x1c004001012080ULL,
    0x8004200962a00220ULL, 0x8422100208500202ULL, 0x2000402200300c08ULL, 0x8646020080080080ULL, 0x80020a0200100808ULL, 0x2010004880111000ULL, 0x623000a080011400ULL, 0x42008c0340209202ULL,
    0x209188240001000ULL,  0x400408a884001800ULL, 0x110400a6080400ULL,   0x1840060a44020800ULL, 0x90080104000041ULL,   0x201011000808101ULL,  0x1a2208080504f080ULL, 0x8012020600211212ULL,
    0x500861011240000ULL,  0x180806108200800ULL,  0x4000020e01040044ULL, 0x300000261044000aULL, 0x802241102020002ULL,  0x20906061210001ULL,   0x5a84841004010310ULL, 0x4010801011c04ULL,
    0xa010109502200ULL,    0x4a02012000ULL,       0x500201010098b028ULL, 0x8040002811040900ULL, 0x28000010020204ULL,   0x6000020202d0240ULL,  0x8918844842082200ULL, 0x4010011029020020ULL,
};

// clang-format off
inline constexpr int SHIFT[64] = {
    6, 5, 5, 5, 5, 5, 5, 6,
    5, 5, 5, 5, 5, 5, 5, 5,
    5, 5, 7, 7, 7, 7, 5, 5,
    5, 5, 7, 9, 9, 7, 5, 5,
    5, 5, 7, 9, 9, 7, 5, 5,
    5, 5, 7, 7, 7, 7, 5, 5,
    5, 5, 5, 5, 5, 5, 5, 5,
    6, 5, 5, 5, 5, 5, 5, 6
};
// clang-format on

inline uint64_t ATTACK_RAYS[64];

inline uint64_t ATTACKS[64][512];

inline void initialise();

inline void initialiseRays();

inline void initialiseAttacks();

[[nodiscard]] inline uint64_t getBishopAttacks(int square, uint64_t occupancy);

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

            for (int dRank = rank + 1, dFile = file + 1; dRank <= 6 && dFile <= 6; ++dRank, ++dFile) {
                utility::BitUtility::setBit(attacks, dRank, dFile);
            }

            for (int dRank = rank + 1, dFile = file - 1; dRank <= 6 && dFile >= 1; ++dRank, --dFile) {
                utility::BitUtility::setBit(attacks, dRank, dFile);
            }

            for (int dRank = rank - 1, dFile = file + 1; dRank >= 1 && dFile <= 6; --dRank, ++dFile) {
                utility::BitUtility::setBit(attacks, dRank, dFile);
            }

            for (int dRank = rank - 1, dFile = file - 1; dRank >= 1 && dFile >= 1; --dRank, --dFile) {
                utility::BitUtility::setBit(attacks, dRank, dFile);
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

            ATTACKS[square][magicIndex] = getBishopAttacks(square, occupancy);
        }
    }
}

[[nodiscard]] inline uint64_t getBishopAttacks(int square, uint64_t occupancy) {
    uint64_t attacks = 0ULL;

    int rank = utility::BoardUtility::getRank(square);
    int file = utility::BoardUtility::getFile(square);

    for (int dRank = rank + 1, dFile = file + 1; dRank <= 7 && dFile <= 7; ++dRank, ++dFile) {
        utility::BitUtility::setBit(attacks, dRank, dFile);

        if (utility::BitUtility::isBitSet(occupancy, dRank, dFile)) {
            break;
        }
    }

    for (int dRank = rank + 1, dFile = file - 1; dRank <= 7 && dFile >= 0; ++dRank, --dFile) {
        utility::BitUtility::setBit(attacks, dRank, dFile);

        if (utility::BitUtility::isBitSet(occupancy, dRank, dFile)) {
            break;
        }
    }

    for (int dRank = rank - 1, dFile = file + 1; dRank >= 0 && dFile <= 7; --dRank, ++dFile) {
        utility::BitUtility::setBit(attacks, dRank, dFile);

        if (utility::BitUtility::isBitSet(occupancy, dRank, dFile)) {
            break;
        }
    }

    for (int dRank = rank - 1, dFile = file - 1; dRank >= 0 && dFile >= 0; --dRank, --dFile) {
        utility::BitUtility::setBit(attacks, dRank, dFile);

        if (utility::BitUtility::isBitSet(occupancy, dRank, dFile)) {
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

} // namespace engine::piece::Bishop
