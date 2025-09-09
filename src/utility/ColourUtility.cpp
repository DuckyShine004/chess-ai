#include "utility/ColourUtility.hpp"

namespace utility {

std::string ColourUtility::getAnsiForegroundFromHex(const std::string &hex) {
    std::array<int, 3> rgb = ColourUtility::getRgbFromHex(hex);

    char buffer[32];

    std::snprintf(buffer, sizeof(buffer), "\033[38;2;%d;%d;%dm", rgb[0], rgb[1], rgb[2]);

    return std::string(buffer);
}

std::string ColourUtility::getAnsiBackgroundFromHex(const std::string &hex) {
    std::array<int, 3> rgb = ColourUtility::getRgbFromHex(hex);

    char buffer[32];

    std::snprintf(buffer, sizeof(buffer), "\033[48;2;%d;%d;%dm", rgb[0], rgb[1], rgb[2]);

    return std::string(buffer);
}

std::array<int, 3> ColourUtility::getRgbFromHex(const std::string &hex) {
    std::string digits = hex.substr(1);

    std::array<int, 3> rgb;

    rgb[0] = std::stoi(digits.substr(0, 2), nullptr, 16);
    rgb[1] = std::stoi(digits.substr(2, 2), nullptr, 16);
    rgb[2] = std::stoi(digits.substr(4, 2), nullptr, 16);

    return rgb;
}

} // namespace utility
