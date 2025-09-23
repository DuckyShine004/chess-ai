#pragma once

namespace engine::evaluation {

// clang-format off
// inline constexpr int MATERIAL_TABLE[6] = {
//     100,
//     320,
//     330,
//     500,
//     900,
//     20000
// };

// TODO: Tune material value - sometimes queen sacrifices herself
inline constexpr int MATERIAL_TABLE[6] = {
    100,
    300,
    350,
    500,
    1000,
    10000,
};

// TODO: Tune NMP endgame material score, at ~40% material remaining can't do NMP total: 4100
// 1 queen and 1 rook
inline constexpr int ENDGAME_MATERIAL_SCORE = 1500;

// clang-format on

} // namespace engine::evaluation
