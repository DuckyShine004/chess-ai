#include <sstream>

#include "logger/Entry.hpp"

#include "utility/FileUtility.hpp"
#include "utility/ColourUtility.hpp"
#include "utility/DatetimeUtility.hpp"

using namespace utility;

namespace logger {

Entry::Entry(Severity severity, const char *file, const char *function, int line, std::string message) : _severity(severity), _file(file), _function(function), _line(line), _message(message) {
    int severityLevel = static_cast<int>(severity);

    this->_severityName = _NAMES[severityLevel];

    this->_backgroundColour = ColourUtility::getAnsiBackgroundFromHex(_BACKGROUND_COLOURS[severityLevel]);

    this->_textColour = ColourUtility::getAnsiForegroundFromHex(_TEXT_COLOURS[severityLevel]);

    this->_date = DatetimeUtility::getDatetime("%A %d %Y");

    this->_time = DatetimeUtility::getDatetime("%H:%M:%S");

    this->_filename = FileUtility::getFilenameFromPath(file);

    this->_severityLevel = severityLevel;

    this->toJson();
}

nlohmann::json Entry::getJson() {
    return this->_json;
}

std::string Entry::toString() {
    std::ostringstream oss;

    oss << "\033[1m" << this->_backgroundColour << this->_textColour << "[" << this->_severityName << "]" << "\033[0m";

    oss << " [" << this->_date << " ~ " << this->_time << "]";

    oss << " [" << this->_filename << " | " << this->_function << ":" << this->_line << "]\n";

    oss << "  " << this->_message;

    return oss.str();
}

void Entry::toJson() {
    this->_json["severity"] = this->_severityName;
    this->_json["level"] = this->_severityLevel;

    this->_json["date"] = this->_date;
    this->_json["time"] = this->_time;

    this->_json["file"] = this->_file;
    this->_json["function"] = this->_function;
    this->_json["line"] = this->_line;

    this->_json["message"] = this->_message;
}

} // namespace logger
