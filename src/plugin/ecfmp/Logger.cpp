#include "Logger.h"

namespace UKControllerPlugin::ECFMP {
    void Logger::Info(const std::string& message)
    {
        LogInfo(message);
    }

    void Logger::Debug(const std::string& message)
    {
        LogDebug(message);
    }

    void Logger::Warning(const std::string& message)
    {
        LogWarning(message);
    }

    void Logger::Error(const std::string& message)
    {
        LogError(message);
    }
} // namespace UKControllerPlugin::ECFMP
