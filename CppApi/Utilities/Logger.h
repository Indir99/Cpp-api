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
 * @brief Logger class for logging messages with different severity levels.
 *
 * This class implements a thread-safe singleton logger that can be used
 * to log messages at different log levels (Debug, Info, Warning, Error).
 */
class Logger {

public:

    /**
     * @brief Get the singleton instance of the Logger.
     *
     * This is a thread-safe method to retrieve the global instance of the Logger.
     *
     * @return Logger& Reference to the singleton Logger instance.
     */
    static Logger& GetInstance();

    /**
     * @brief Log a message with a specific log level.
     *
     * Logs a message to the console (or file) if the message's log level is greater than
     * or equal to the current log level. Thread-safe using a mutex.
     *
     * @param level The log level (Debug, Info, Warning, Error).
     * @param message The message to log.
     */
    void Log(LogLevel level, const std::string& message);

    /**
     * @brief Set the log level threshold.
     *
     * Only messages with a log level greater than or equal to this threshold will be logged.
     *
     * @param level The log level threshold.
     */
    void SetLogLevel(LogLevel level);

private:

    Logger() = default;
    ~Logger() = default;
    Logger(const Logger&) = delete;
    Logger(Logger&&) = delete;
    Logger& operator=(const Logger&) = delete;
    Logger& operator=(Logger&&) = delete;

    /**
     * @brief Helper function to get the current time as a string.
     *
     * This method returns the current system time formatted as a string.
     *
     * @return std::string The current time in the format "YYYY-MM-DD HH:MM:SS".
     */
    std::string GetCurrentTime();

    // Singleton instance
    static std::unique_ptr<Logger> m_instance; ///< Pointer to the singleton Logger instance.
    static std::once_flag m_initInstanceFlag; ///< Flag for initializing the Logger instance.
    // Log level threshold
    LogLevel m_logLevel = LogLevel::Info;
    // For thread-safe logging
    std::mutex m_logMutex;
};

} // Utilities
