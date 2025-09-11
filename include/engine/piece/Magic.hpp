#pragma once

#include <cstdint>

namespace engine::piece::Magic {

// https://github.com/maksimKorzh/chess_programming/blob/master/src/magics/magics.txt

inline uint64_t BISHOP_ATTACKS[64][4096];

inline uint64_t ROOK_ATTACKS[64][512];

// Relevant bits
inline int BISHOP_BITS[64];
inline int ROOK_BITS[64];

inline void initialiseAttacks();

[[nodiscard]] inline uint64_t getBishopAttacks(int square, uint64_t occupancy);

[[nodiscard]] inline uint64_t getRookAttacks(int square, uint64_t occupancy);

} // namespace engine::piece::Magic
