#include "Logger.h"
#include <iostream>
#include <chrono>

namespace Utilities {

// Initialize singleton instance
std::unique_ptr<Logger> Logger::m_instance;
std::once_flag Logger::m_initInstanceFlag;

// TODO: Need implementation
Logger& Logger::GetInstance() {
    std::call_once(m_initInstanceFlag, []() {
        m_instance.reset(new Logger);
    });
    return *m_instance;
}

// TODO: Check what should we do here
void Logger::SetLogLevel(LogLevel level) {
    m_logLevel = level;
}

void Logger::Log(LogLevel level, const std::string& message) {

    // Only log if the message's level is >= the set log level
    if (level < m_logLevel) return;

    // Thread-safe logging using mutex
    std::lock_guard<std::mutex> lock(m_logMutex);

    std::string levelStr;
    switch (level) {
    case LogLevel::Debug:   levelStr = "DEBUG"; break;
    case LogLevel::Info:    levelStr = "INFO"; break;
    case LogLevel::Warning: levelStr = "WARNING"; break;
    case LogLevel::Error:   levelStr = "ERROR"; break;
    }
    // Log to console or file
    std::cout << "[" << GetCurrentTime() << "] " << levelStr << ": " << message << std::endl;
}

std::string Logger::GetCurrentTime() {
    // Get current time
    auto currentTime = std::chrono::system_clock::now();
    // Convert the time point to a time_t (seconds since epoch)
    std::time_t currentTimeT = std::chrono::system_clock::to_time_t(currentTime);
    // Format the time as a string
    char timeBuffer[80];
    std::strftime(timeBuffer, sizeof(timeBuffer), "%Y-%m-%d %H:%M:%S", std::localtime(&currentTimeT));
    // Store the formatted time in a std::string
    std::string formattedTime(timeBuffer);
    return formattedTime;
}

} // Utilities
