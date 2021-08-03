#pragma once

void LogCritical(std::string message);
void LogDebug(std::string message);
void LogError(std::string message);
void LogInfo(std::string message);
void LogWarning(std::string message);
void SetLoggerInstance(std::shared_ptr<spdlog::logger> instance);
void ShutdownLogger(void);
