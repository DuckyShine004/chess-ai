#pragma once

#include <string>
#include <vector>

#include "logger/Entry.hpp"

namespace logger {

class Logger {
  public:
    Logger(const Logger &) = delete;

    Logger &operator=(const Logger &) = delete;

    static Logger &getInstance();

    void log(Severity severity, const char *file, const char *function, int line, std::string message);

    void addEntry(Entry entry);

    void save();

  private:
    Logger();

    ~Logger();

    void initialise();

    void run();

    nlohmann::json _json;

    std::vector<Entry> _entries;

    std::string _logPath;
};

} // namespace logger
