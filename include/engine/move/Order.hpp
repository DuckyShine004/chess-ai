#pragma once

// http://paulwebster.net/mvv-lva-move-ordering/
namespace engine::move {

inline constexpr int TT_VALUE = 30000;

inline constexpr int PV_VALUE = 20000;

// inline constexpr int MVV_LVA_OFFSET = INT_MAX - 4096;
inline constexpr int MVV_LVA_OFFSET = 10000;

// clang-format off
inline constexpr int MVV_LVA[6][6] = {
    {105, 205, 305, 405, 505, 605},
    {104, 204, 304, 404, 504, 604},
    {103, 203, 303, 403, 503, 603},
    {102, 202, 302, 402, 502, 602},
    {101, 201, 301, 401, 501, 601},
    {100, 200, 300, 400, 500, 600},
};
// clang-format on 

inline constexpr int MAX_PLY = 64;
inline constexpr int MAX_KILLER_MOVES = 2;
// inline constexpr int KILLER_VALUE = 10;
inline constexpr int KILLER_VALUE = 1000;

} // namespace engine::evaluation
