#include "utils/pch.h"
#include "log/LoggerBootstrap.h"
#include "helper/HelperFunctions.h"
#include "windows/WinApiInterface.h"

using UKControllerPlugin::Windows::WinApiInterface;
namespace UKControllerPlugin {
    namespace Log {
        const int logfilesToKeep = 5;
        const std::wstring logsFolder = L"logs";

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

            std::shared_ptr<spdlog::logger> logger = spdlog::basic_logger_mt(
                "logger",
                windows.GetFullPathToLocalFile(GetLogFilePath(GetLogfileName(logfilePrefix)))
            );
            logger->set_pattern("%Y-%m-%d %T [%l] - %v");

#ifdef _DEBUG
            logger->set_level(spdlog::level::trace);
            logger->flush_on(spdlog::level::trace);
#else
            logger->set_level(spdlog::level::info);
            logger->flush_on(spdlog::level::info);
#endif  // DEBUG

            SetLoggerInstance(logger);
            LogInfo("Logfile opened");
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
            LogInfo("Logfile closed");
            ShutdownLogger();
        }

        /*
         * Create the logs folder
         */
        bool LoggerBootstrap::CreateLogsFolder(WinApiInterface& windows)
        {
            // Create us a logger, for now we log everything that happens.
            if (!windows.CreateLocalFolderRecursive(logsFolder)) {
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
         * Prune all the logs matching the prefix, so we only keep a certain amount.
         */
        void LoggerBootstrap::PruneLogs(WinApiInterface& windows, std::wstring logfilePrefix)
        {
            std::set<std::wstring> allFilesInLogFolder = GetExistingLogs(windows, logfilePrefix);
            const int logFilesToDelete = allFilesInLogFolder.size() - logfilesToKeep;

            int logFilesDeleted = 0;
            for (
                auto log = allFilesInLogFolder.cbegin();
                log != allFilesInLogFolder.cend();
                ++log
            ) {
                if (logFilesDeleted == logFilesToDelete) {
                    break;
                }

                windows.DeleteGivenFile(GetLogFilePath(*log));
                logFilesDeleted++;
            }

            LogInfo("Pruned " + std::to_string(logFilesDeleted) + " log files");
        }

        /*
         * Get all the existing logs that match the log prefix
         */
        std::set<std::wstring> LoggerBootstrap::GetExistingLogs(
            WinApiInterface& windows,
            std::wstring logFilePrefix
        )
        {
            std::set<std::wstring> allFilesInLogFolder = windows.ListAllFilenamesInDirectory(logsFolder);
            std::wregex logsRegex(L"^" + logFilePrefix + L"-[0-9]+\\.log$");
            for (
                auto log = allFilesInLogFolder.begin();
                log != allFilesInLogFolder.end();
            ) {
                if (!std::regex_match(*log, logsRegex)) {
                    log = allFilesInLogFolder.erase(log);
                } else {
                    ++log;
                }
            }

            return std::move(allFilesInLogFolder);
        }

        /*
         * Get the name of a given logfile
         */
        std::wstring LoggerBootstrap::GetLogfileName(std::wstring logFilePrefix)
        {
            return logFilePrefix + L"-" + HelperFunctions::ConvertToWideString(
                date::format("%Y%m%d%H%M%S", date::floor<std::chrono::seconds>(std::chrono::system_clock::now()))
            ) + L".log";
        }

        /*
         * Get the path for a given logfile
         */
        std::wstring LoggerBootstrap::GetLogFilePath(std::wstring name)
        {
            return logsFolder + L"/" + name;
        }
    }  // namespace Log
}  // namespace UKControllerPlugin
