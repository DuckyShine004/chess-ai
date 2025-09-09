#include <iostream>

#include "logger/Logger.hpp"

#include "utility/FileUtility.hpp"
#include "utility/DatetimeUtility.hpp"

using namespace utility;

namespace logger {

Logger::Logger() {
    this->initialise();
}

Logger::~Logger() = default;

Logger &Logger::getInstance() {
    static Logger instance;

    return instance;
}

void Logger::initialise() {
    std::string filename = DatetimeUtility::getDatetime("%d-%m-%Y_%H-%M-%S") + ".log";

    this->_logPath = ".cache/logs/" + filename;

    this->_json = nlohmann::json::array();

    FileUtility::createFile(this->_logPath);
}

void Logger::log(Severity severity, const char *file, const char *function, int line, std::string message) {
    Entry entry(severity, file, function, line, message);

    this->addEntry(entry);

    std::cout << entry.toString() << std::endl;
}

void Logger::addEntry(Entry entry) {
    this->_entries.push_back(entry);

    this->_json.push_back(entry.getJson());

    this->save();
}

void Logger::save() {
    FileUtility::saveJson(this->_json, this->_logPath);
}

} // namespace logger
