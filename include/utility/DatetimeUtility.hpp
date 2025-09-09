#pragma once

#include <string>

namespace utility {

class DatetimeUtility {
  public:
    static std::string getDatetime(const std::string &pattern);
};

} // namespace utility
