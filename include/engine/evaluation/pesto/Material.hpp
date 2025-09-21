#pragma once

namespace engine::evaluation::pesto {

// 0: opening, 1: endgame
inline constexpr int MATERIAL_VALUES[2][6] = {
    { 82, 337, 365, 477, 1025, 12000 },
    { 94, 281, 297, 512, 936, 12000 },
};

} // namespace engine::evaluation::pesto
