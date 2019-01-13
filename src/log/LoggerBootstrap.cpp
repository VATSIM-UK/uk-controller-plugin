#include "pch/stdafx.h"
#include "log/LoggerBootstrap.h"
#include "bootstrap/PersistenceContainer.h"
#include "windows/WinApiInterface.h"

using UKControllerPlugin::Bootstrap::PersistenceContainer;
namespace UKControllerPlugin {
    namespace Log {

        /*
            Bootstrap the logger.
        */
        void LoggerBootstrap::Bootstrap(PersistenceContainer & persistence, bool nullLogger)
        {
            if (nullLogger) {
                SetLoggerInstance(
                    std::make_shared<spdlog::logger>(
                        "null_logger",
                        std::make_shared<spdlog::sinks::null_sink_mt>()
                    )
                );
                return;
            }

            if (!persistence.windows->CreateFolder("ukcp")) {
                throw std::runtime_error("Couldn't create the plugin files directory.");
            }

            // Create us a logger, for now we log everything that happens.
            if (!persistence.windows->CreateFolder(persistence.windows->GetFullPathToLocalFile("logs"))) {
                throw std::runtime_error("Could not create the logs folder.");
            }

            std::shared_ptr<spdlog::sinks::rotating_file_sink_mt> rotatingSink =
                std::make_shared<spdlog::sinks::rotating_file_sink_mt>(
                persistence.windows->GetFullPathToLocalFile("logs/eventlog.txt"),
                1024 * 1024,
                3
            );
            rotatingSink->set_level(spdlog::level::info);

            std::shared_ptr<spdlog::logger> logger = std::make_shared<spdlog::logger>("rotating logger", rotatingSink);
            logger->set_pattern("%Y-%m-%d %T [%l] - %v");
            logger->flush_on(spdlog::level::info);
            SetLoggerInstance(logger);
            LogInfo("Log opened");
        }
    }  // namespace Log
}  // namespace UKControllerPlugin
