#pragma once

#include<memory>
#include<mutex>

namespace Utilities {

/**
 * @brief Enum class for log levels.
 */
enum class LogLevel {
    Debug,      ///< Debug level logging
    Info,       ///< Informational level logging
    Warning,    ///< Warning level logging
    Error       ///< Error level logging
};

/**
 * @class Logger
 * @brief A thread-safe logger class that provides logging functionality with various log levels.
 *
 * This class allows logging messages to the console or to a file (TODO: need implementation).
 * It supports multiple log levels such as Debug, Info, Warning, and Error.
 * The logger uses a mutex to ensure thread safety.
 */
class Logger {

public:
    explicit Logger(LogLevel level);
    ~Logger() = default;

    Logger(const Logger&) = delete;
    Logger(Logger&&) = delete;
    Logger& operator=(const Logger&) = delete;
    Logger& operator=(Logger&&) = delete;

    /**
     * @brief Logs a message with the given log level.
     *
     * @param level The log level (Debug, Info, Warning, Error).
     * @param message The message to log.
     *
     * Logs the message only if the provided log level is greater than or equal to the logger's current log level.
     * The message is timestamped and written to the console (or file if implemented).
     * This method is thread-safe.
     */
    void Log(LogLevel level, const std::string& message);

    /**
     * @brief Sets the log level of the logger.
     *
     * @param level The log level to set (Debug, Info, Warning, Error).
     *
     * Only messages with a log level greater than or equal to the set log level will be logged.
     */
    void ChangeLogLevel(LogLevel level);

private:

    /**
     * @brief Helper function to get the current time as a formatted string.
     *
     * @return A string representing the current time in the format "YYYY-MM-DD HH:MM:SS".
     */
    std::string GetCurrentTime();

    // Log level threshold
    LogLevel m_logLevel;
    // For thread-safe logging
    std::mutex m_logMutex;
};

} // Utilities
