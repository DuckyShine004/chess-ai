#pragma once

#include <fmt/format.h>

// clang-format off
#ifdef NDEBUG
    #define LOG_DEBUG(...) ((void) 0)
#else
    #define LOG_DEBUG(...) Logger::getInstance().log(Severity::DEBUG, __FILE__, __FUNCTION__, __LINE__, fmt::format(__VA_ARGS__))
#endif
// clang-format on

#define LOG_INFO(...) Logger::getInstance().log(Severity::INFO, __FILE__, __FUNCTION__, __LINE__, fmt::format(__VA_ARGS__))

#define LOG_WARN(...) Logger::getInstance().log(Severity::WARN, __FILE__, __FUNCTION__, __LINE__, fmt::format(__VA_ARGS__))

#define LOG_ERROR(...) Logger::getInstance().log(Severity::ERROR, __FILE__, __FUNCTION__, __LINE__, fmt::format(__VA_ARGS__))

#define LOG_FATAL(...) Logger::getInstance().log(Severity::FATAL, __FILE__, __FUNCTION__, __LINE__, fmt::format(__VA_ARGS__))
