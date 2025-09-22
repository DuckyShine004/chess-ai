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

// TODO: Tune NMP endgame material score, at 50% material remaining can't do NMP
// 1 queen and 2 rooks
// Or, two rooks, two bishops and knight
inline constexpr int ENDGAME_MATERIAL_SCORE = 2050;

// clang-format on

} // namespace engine::evaluation
