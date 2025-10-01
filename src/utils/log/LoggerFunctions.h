#pragma once

namespace spdlog {
    class logger;
} // namespace spdlog

namespace UKControllerPluginUtils::Log {
    class ApiLoggerInterface;
}

[[nodiscard]] auto ApiLogger() -> const UKControllerPluginUtils::Log::ApiLoggerInterface&;
void LogFatalExceptionAndRethrow(const std::string& source, const std::exception& exception);
void LogFatalExceptionAndRethrow(
    const std::string& source, const std::string& subsource, const std::exception& exception);
void LogCritical(std::string message);
void LogDebug(std::string message);
void LogError(std::string message);
void LogInfo(std::string message);
void LogWarning(std::string message);
void SetLoggerInstance(std::shared_ptr<spdlog::logger> instance);
void SetApiLoggerInstance(std::shared_ptr<UKControllerPluginUtils::Log::ApiLoggerInterface> instance);
void ShutdownLogger(void);
