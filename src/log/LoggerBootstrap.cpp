#include "utils/pch.h"
#include "log/LoggerBootstrap.h"
#include "windows/WinApiInterface.h"

using UKControllerPlugin::Windows::WinApiInterface;
namespace UKControllerPlugin {
    namespace Log {
        const std::wstring logsFolder = L"logs";
        const int logfilesToKeep = 7;

        /*
            Bootstrap the logger.
        */
        void LoggerBootstrap::Bootstrap(WinApiInterface& windows, std::wstring logfilePrefix)
        {
            // Create us a logger, for now we log everything that happens.
            if (!CreateLogsFolder(windows)) {
                CreateNullLogger();
                return;
            }
            PruneLogs(windows, logfilePrefix);

            std::shared_ptr<spdlog::sinks::rotating_file_sink_mt> rotatingSink =
                std::make_shared<spdlog::sinks::rotating_file_sink_mt>(
                    windows.GetFullPathToLocalFile(GetLogfileName(logfilePrefix)),
                1024 * 1024,
                3
            );

            std::shared_ptr<spdlog::logger> logger = std::make_shared<spdlog::logger>("rotating logger", rotatingSink);
            logger->set_pattern("%Y-%m-%d %T [%l] - %v");

#ifdef _DEBUG
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

        /*
         * Shuts everything down
         */
        void LoggerBootstrap::Shutdown(void)
        {
            ShutdownLogger();
        }

        /*
         * Create the logs folder
         */
        bool LoggerBootstrap::CreateLogsFolder(WinApiInterface& windows)
        {
            // Create us a logger, for now we log everything that happens.
            if (!windows.CreateLocalFolderRecursive(L"logs")) {
                std::wstring msg = L"Unable to create logs folder, please contact the VATSIM UK Web Department.\n\n";
                msg += L"Plugin events will not be logged.";
                windows.OpenMessageBox(
                    msg.c_str(),
                    L"UKCP Error",
                    MB_OK | MB_ICONSTOP
                );
                return false;
            }

            return true;
        }

        /**
         * Prune all the logs matching the prefix, so we only keep a certain amount
         */
        void LoggerBootstrap::PruneLogs(WinApiInterface& windows, std::wstring logfilePrefix)
        { }

        /*
         *
         */
        std::wstring LoggerBootstrap::GetLogfileName(std::wstring logFilePrefix)
        {
            return L"";
        }
    }  // namespace Log
}  // namespace UKControllerPlugin
