#include "log/ApiLoggerInterface.h"
#include "log/LoggerFunctions.h"

std::shared_ptr<spdlog::logger> logger;
std::shared_ptr<UKControllerPluginUtils::Log::ApiLoggerInterface> apiLogger;

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
    apiLogger.reset();
}

void LogFatalExceptionAndRethrow(const std::string& source, const std::exception& exception)
{
    const auto exceptionMessage = "Critical exception of type " + std::string(typeid(exception).name()) + " at " +
                                  source + ": " + exception.what();
    logger->critical(exceptionMessage);

    try {
        ApiLogger().Log("FATAL_EXCEPTION", exceptionMessage);
    } catch (const std::exception& e) {
        LogCritical("Exception caught in LogFatalExceptionAndRethrow: " + std::string(e.what()));
    }

    throw;
}

void LogFatalExceptionAndRethrow(
    const std::string& source, const std::string& subsource, const std::exception& exception)
{
    LogFatalExceptionAndRethrow(source + "::" + subsource, exception);
}

void SetApiLoggerInstance(std::shared_ptr<UKControllerPluginUtils::Log::ApiLoggerInterface> instance)
{
    if (apiLogger) {
        return;
    }

    apiLogger = instance;
}

auto ApiLogger() -> const UKControllerPluginUtils::Log::ApiLoggerInterface&
{
    if (!apiLogger) {
        LogError("ApiLogger not set");
        throw std::runtime_error("ApiLogger not set");
    }

    return *apiLogger;
}
