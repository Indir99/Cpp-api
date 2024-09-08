#include "Logger.h"
#include <iostream>
#include <chrono>

namespace Utilities {

Logger::Logger(LogLevel level)
    : m_logLevel{level}
{
}

void Logger::ChangeLogLevel(LogLevel level) {
    m_logLevel = level;
}

void Logger::Log(LogLevel level, const std::string& message) {
    if (level < m_logLevel) return;

    std::lock_guard<std::mutex> lock(m_logMutex);

    std::string levelStr;
    switch (level) {
    case LogLevel::Debug:   levelStr = "DEBUG"; break;
    case LogLevel::Info:    levelStr = "INFO"; break;
    case LogLevel::Warning: levelStr = "WARNING"; break;
    case LogLevel::Error:   levelStr = "ERROR"; break;
    }
    std::cout << "[" << GetCurrentTime() << "] " << levelStr << ": " << message << std::endl;
}

std::string Logger::GetCurrentTime() {
    auto currentTime = std::chrono::system_clock::now();
    std::time_t currentTimeT = std::chrono::system_clock::to_time_t(currentTime);
    char timeBuffer[80];
    std::strftime(timeBuffer, sizeof(timeBuffer), "%Y-%m-%d %H:%M:%S", std::localtime(&currentTimeT));
    return std::string(timeBuffer);
}

} // Utilities
