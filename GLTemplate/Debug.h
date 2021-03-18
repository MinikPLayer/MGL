#pragma once
#include <sstream>
#include <iostream>

enum LogTypes
{
    Log,
    Warning,
    Error,
    FatalError
};

enum LogLevels
{
    Engine,
    User
};

// Engine logs
#define LOG_E(...) LogWrapper(__FILE__, __LINE__, LogTypes::Log, LogLevels::Engine, __VA_ARGS__)
#define LOGW_E(...) LogWrapper(__FILE__, __LINE__, LogTypes::Warning, LogLevels::Engine, __VA_ARGS__)
#define LOGE_E(...) LogWrapper(__FILE__, __LINE__, LogTypes::Error, LogLevels::Engine, __VA_ARGS__)
#define LOGF_E(...) LogWrapper(__FILE__, __LINE__, LogTypes::FatalError, LogLevels::Engine, __VA_ARGS__);

// User logs
#define LOG(...) LogWrapper(__FILE__, __LINE__, LogTypes::Log, LogLevels::User, __VA_ARGS__)
#define LOGW(...) LogWrapper(__FILE__, __LINE__, LogTypes::Warning, LogLevels::User, __VA_ARGS__)
#define LOGE(...) LogWrapper(__FILE__, __LINE__, LogTypes::Error, LogLevels::User, __VA_ARGS__)
#define LOGF(...) LogWrapper(__FILE__, __LINE__, LogTypes::FatalError, LogLevels::User, __VA_ARGS__);


// Log_Recursive wrapper that creates the ostringstream
template<typename... Args>
void LogWrapper(const char* file, int line, LogTypes type, LogLevels level, const Args&... args)
{
    std::ostringstream msg;
    Log_Recursive(file, line, type, level, msg, args...);
}

// "Recursive" variadic function
template<typename T, typename... Args>
void Log_Recursive(const char* file, int line, LogTypes type, LogLevels level, std::ostringstream& msg,
    T value, const Args&... args)
{
    msg << value;
    Log_Recursive(file, line, type, level, msg, args...);
}

// Terminator
void Log_Recursive(const char* file, int line, LogTypes type, LogLevels level, std::ostringstream& msg);