#pragma once

#include <string>
#include <fstream>

#include "engine/move/Move.hpp"

#include "engine/opening/Trie.hpp"

#include "utility/RandomUtility.hpp"
#include "utility/StringUtility.hpp"

#include "logger/LoggerMacros.hpp"

namespace engine::opening::Opening {

inline constexpr int MAX_OPENING_DEPTH = 16;

inline constexpr const char *OPENING_FILE_PATH = "resources/openings/book.txt";

inline engine::opening::Trie OPENING_TREE = engine::opening::Trie();

inline void initialise();

[[nodiscard]] inline std::string getRandomMove(std::vector<uint16_t> &moveHistory);

inline void initialise() {
    std::ifstream file(OPENING_FILE_PATH);

    std::string line;

    while (std::getline(file, line)) {
        std::string trimmedLine = utility::StringUtility::getTrimmedString(line);

        std::vector<std::string> openings = utility::StringUtility::splitStringByWhiteSpace(trimmedLine);

        OPENING_TREE.add(openings);
    }

    file.close();
}

[[nodiscard]] inline std::string getRandomMove(std::vector<uint16_t> &moveHistory) {
    std::vector<std::string> moveHistoryString;

    for (uint16_t move : moveHistory) {
        moveHistoryString.push_back(engine::move::Move::getFromToString(move));
    }

    std::vector<std::string> children = OPENING_TREE.getChildren(moveHistoryString);

    if (children.empty()) {
        return "";
    }

    for (std::string &string : children) {
        LOG_INFO(string);
    }

    return utility::RandomUtility::getRandomElement(children);
}

}; // namespace engine::opening::Opening
