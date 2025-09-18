#pragma once

#include <cstdint>

namespace engine::hash::Transposition {

// 8 MB transposition table
inline constexpr uint32_t TRANSPOSITION_TABLE_SIZE = 0x1E8480000U;

enum NodeType : uint8_t {
    ALPHA = 0,
    BETA = 1,
    EXACT = 2,
    UNKNOWN = 3,
};

struct Entry {
    uint64_t zobrist;

    int score;
    int depth;

    NodeType nodeType;

    Entry() : zobrist(0ULL), score(0), depth(0), nodeType(NodeType::UNKNOWN) {
    }
};

} // namespace engine::hash::Transposition
