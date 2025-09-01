#pragma once

#include <string>
#include <vector>

namespace utility {

class StringUtility {
  public:
    static std::vector<std::string> splitStringByWhiteSpace(const std::string &input);

    static bool isLetter(const char letter) {
        return ('a' <= letter && letter <= 'z') || ('A' <= letter && letter <= 'Z');
    }
};

} // namespace utility