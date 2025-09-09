#pragma once

#include <array>
#include <string>

namespace utility {

class ColourUtility {
  public:
    static std::string getAnsiForegroundFromHex(const std::string &hex);

    static std::string getAnsiBackgroundFromHex(const std::string &hex);

  private:
    static std::array<int, 3> getRgbFromHex(const std::string &hex);
};

} // namespace utility
