#include <cstddef>
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

std::string StringUtility::getTrimmedString(const std::string &input) {
    const char *whiteSpace = " \t\v\r\n";

    size_t start = input.find_first_not_of(whiteSpace);
    size_t end = input.find_last_not_of(whiteSpace);

    return start == end ? std::string() : input.substr(start, end - start + 1);
}

} // namespace utility
