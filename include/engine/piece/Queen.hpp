#pragma once

#include "engine/piece/Bishop.hpp"
#include "engine/piece/Rook.hpp"

namespace engine::piece::Queen {

[[nodiscard]] inline uint64_t getAttacks(int square, uint64_t occupancy);

[[nodiscard]] inline uint64_t getAttacks(int square, uint64_t occupancy) {
    return Bishop::getAttacks(square, occupancy) | Rook::getAttacks(square, occupancy);
}

} // namespace engine::piece::Queen