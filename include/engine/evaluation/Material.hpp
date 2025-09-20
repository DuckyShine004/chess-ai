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
// clang-format on

} // namespace engine::evaluation
