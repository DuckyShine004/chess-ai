#pragma once

#include <string>

#include <nlohmann/json.hpp>

#include "logger/Severity.hpp"

namespace logger {

class Entry {
  public:
    Entry(Severity severity, const char *file, const char *function, int line, std::string message);

    nlohmann::json getJson();

    std::string toString();

  private:
    Severity _severity;

    nlohmann::json _json;

    std::string _backgroundColour;

    std::string _textColour;

    std::string _file;

    std::string _filename;

    std::string _function;

    std::string _message;

    std::string _severityName;

    std::string _date;

    std::string _time;

    void toJson();

    int _line;

    int _severityLevel;
};

} // namespace logger
