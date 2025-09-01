#include <sstream>

#include "utility/StringUtility.hpp"

namespace utility {

std::vector<std::string> StringUtility::splitStringByWhiteSpace(const std::string &input) {
    std::vector<std::string> strings;

    std::istringstream iss(input);

    std::string word;

    while (iss >> word) {
        strings.push_back(word);
    }

    return strings;
}

} // namespace utility