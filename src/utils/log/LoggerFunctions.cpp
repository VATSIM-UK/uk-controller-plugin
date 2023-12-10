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

    LogInfo("Logger shutdown");
    spdlog::drop_all();
    logger.reset();
}

void LogFatalExceptionAndRethrow(const std::string& source, const std::exception& exception)
{
    logger->critical(
        "Critical exception of type " + std::string(typeid(exception).name()) + " at " + source + ": " +
        exception.what());
    throw;
}

void LogFatalExceptionAndRethrow(
    const std::string& source, const std::string& subsource, const std::exception& exception)
{
    LogFatalExceptionAndRethrow(source + "::" + subsource, exception);
}
