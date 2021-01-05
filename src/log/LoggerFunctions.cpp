#include "pch/stdafx.h"
#include "spdlog/spdlog.h"
#include "log/LoggerFunctions.h"

std::shared_ptr<spdlog::logger> logger;

void LogCritical(std::string message)
{
    logger->critical(message);
}

void LogDebug(std::string message)
{
    logger->debug(message);
}

void LogError(std::string message)
{
    logger->error(message);
}

void LogInfo(std::string message)
{
    logger->info(message);
}

void LogWarning(std::string message)
{
    logger->warn(message);
}

void SetLoggerInstance(std::shared_ptr<spdlog::logger> instance)
{
    if (logger) {
        return;
    }

    logger = instance;
}

void ShutdownLogger(void)
{
    if (!logger) {
        return;
    }

    spdlog::drop_all();
    logger.reset();
}
