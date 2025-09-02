#pragma once

#include <cstdint>

namespace engine::board {

enum Castle : uint8_t {
    WHITE_KING = 1,
    WHITE_QUEEN = 2,
    BLACK_KING = 4,
    BLACK_QUEEN = 8,
};

} // namespace engine::board