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
                LoggerBootstrap::CreateNullLogger();
                return;
            }

            // Create us a logger, for now we log everything that happens.
            if (!persistence.windows->CreateLocalFolderRecursive("logs")) {
                std::wstring msg = L"Unable to create logs folder, please contact the VATSIM UK Web Department.\n\n";
                msg += L"Plugin events will not be logged.";
                persistence.windows->OpenMessageBox(
                    msg.c_str(),
                    L"UKCP Error",
                    MB_OK | MB_ICONSTOP
                );
                LoggerBootstrap::CreateNullLogger();
                return;
            }

            std::shared_ptr<spdlog::sinks::rotating_file_sink_mt> rotatingSink =
                std::make_shared<spdlog::sinks::rotating_file_sink_mt>(
                persistence.windows->GetFullPathToLocalFile("logs/eventlog.txt"),
                1024 * 1024,
                3
            );

            std::shared_ptr<spdlog::logger> logger = std::make_shared<spdlog::logger>("rotating logger", rotatingSink);
            logger->set_pattern("%Y-%m-%d %T [%l] - %v");

#ifdef DEBUG
            logger->set_level(spdlog::level::trace);
            logger->flush_on(spdlog::level::trace);
#else
            logger->set_level(spdlog::level::info);
            logger->flush_on(spdlog::level::info);
#endif  // DEBUG

            SetLoggerInstance(logger);
            LogInfo("Log opened");
        }

        /*
            Sets the logger instance to be a null logger.
        */
        void LoggerBootstrap::CreateNullLogger(void)
        {
            SetLoggerInstance(
                std::make_shared<spdlog::logger>(
                "null_logger",
                std::make_shared<spdlog::sinks::null_sink_mt>()
            )
            );
        }
    }  // namespace Log
}  // namespace UKControllerPlugin
