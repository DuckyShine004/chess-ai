#pragma once

#include <cstdint>

namespace utility::RandomUtility {

inline uint32_t state = 1804289383U;

[[nodiscard]] inline uint32_t getRandomU32();

[[nodiscard]] inline uint64_t getRandomU64();

[[nodiscard]] inline uint32_t getRandomU32() {
    uint32_t x = state;

    x ^= x << 13;
    x ^= x >> 17;
    x ^= x << 5;

    return state = x;
}

[[nodiscard]] inline uint64_t getRandomU64() {
    uint64_t h1 = (uint64_t)(getRandomU32()) & 0xFFFF;
    uint64_t h2 = (uint64_t)(getRandomU32()) & 0xFFFF;
    uint64_t h3 = (uint64_t)(getRandomU32()) & 0xFFFF;
    uint64_t h4 = (uint64_t)(getRandomU32()) & 0xFFFF;

    return h1 | (h2 << 16) | (h3 << 32) | (h4 << 48);
}

} // namespace utility::RandomUtility
