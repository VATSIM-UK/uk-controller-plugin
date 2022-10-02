#include "helper/TestLogger.h"

std::shared_ptr<spdlog::logger> testLogger;

/*
    Returns a logger to test with that doesnt write log files.
*/
std::shared_ptr<spdlog::logger> GetTestLogger()
{
    if (!testLogger) {
        testLogger = std::make_shared<spdlog::logger>("null_logger", std::make_shared<spdlog::sinks::null_sink_mt>());
    }

    return testLogger;
}
