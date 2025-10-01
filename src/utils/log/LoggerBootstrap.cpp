#include "LoggerBootstrap.h"
#include "helper/HelperFunctions.h"
#include "windows/WinApiInterface.h"

using UKControllerPlugin::Windows::WinApiInterface;
namespace UKControllerPlugin::Log {

    /*
        Bootstrap the logger.
    */
    void LoggerBootstrap::Bootstrap(WinApiInterface& windows, const std::wstring& logfilePrefix)
    {
        // Create us a logger, for now we log everything that happens.
        if (!CreateLogsFolder(windows)) {
            CreateNullLogger();
            return;
        }
        PruneLogs(windows, logfilePrefix);

        std::shared_ptr<spdlog::logger> logger = spdlog::basic_logger_mt(
            HelperFunctions::ConvertToRegularString(logfilePrefix) + "-logger",
            windows.GetFullPathToLocalFile(GetLogFilePath(GetLogfileName(logfilePrefix))));
        logger->set_pattern("%Y-%m-%d %T [%l] - %v");

#ifdef _DEBUG
        logger->set_level(spdlog::level::trace);
        logger->flush_on(spdlog::level::trace);
#else
        logger->set_level(spdlog::level::info);
        logger->flush_on(spdlog::level::info);
#endif // DEBUG

        SetLoggerInstance(logger);
        LogInfo("Logfile opened");
    }

    /*
        Sets the logger instance to be a null logger.
    */
    void LoggerBootstrap::CreateNullLogger()
    {
        SetLoggerInstance(
            std::make_shared<spdlog::logger>("null_logger", std::make_shared<spdlog::sinks::null_sink_mt>()));
    }

    /*
     * Create the logs folder
     */
    auto LoggerBootstrap::CreateLogsFolder(WinApiInterface& windows) -> bool
    {
        // Create us a logger, for now we log everything that happens.
        if (!windows.CreateLocalFolderRecursive(GetLogsFolder())) {
            std::wstring msg = L"Unable to create logs folder, please contact the VATSIM UK Web Department.\n\n";
            msg += L"Plugin events will not be logged.";
            windows.OpenMessageBox(msg.c_str(), L"UKCP Error", MB_OK | MB_ICONSTOP);
            return false;
        }

        return true;
    }

    /**
     * Prune all the logs matching the prefix, so we only keep a certain amount.
     */
    void LoggerBootstrap::PruneLogs(WinApiInterface& windows, const std::wstring& logfilePrefix)
    {
        std::set<std::wstring> allFilesInLogFolder = GetExistingLogs(windows, logfilePrefix);
        const int logFilesToDelete = static_cast<int>(allFilesInLogFolder.size()) - GetLogFilesToKeep();

        int logFilesDeleted = 0;
        for (auto log = allFilesInLogFolder.cbegin(); log != allFilesInLogFolder.cend(); ++log) {
            if (logFilesDeleted >= logFilesToDelete) {
                break;
            }

            windows.DeleteGivenFile(GetLogFilePath(*log));
            logFilesDeleted++;
        }
    }

    /*
     * Get all the existing logs that match the log prefix
     */
    auto LoggerBootstrap::GetExistingLogs(WinApiInterface& windows, const std::wstring& logFilePrefix)
        -> std::set<std::wstring>
    {
        std::set<std::wstring> allFilesInLogFolder = windows.ListAllFilenamesInDirectory(GetLogsFolder());
        std::wregex logsRegex(L"^" + logFilePrefix + L"-[0-9]+\\.txt");
        for (auto log = allFilesInLogFolder.begin(); log != allFilesInLogFolder.end();) {
            if (!std::regex_match(*log, logsRegex)) {
                log = allFilesInLogFolder.erase(log);
            } else {
                ++log;
            }
        }

        return allFilesInLogFolder;
    }

    /*
     * Get the name of a given logfile
     */
    auto LoggerBootstrap::GetLogfileName(const std::wstring& logFilePrefix) -> std::wstring
    {
        return logFilePrefix + L"-" +
               HelperFunctions::ConvertToWideString(
                   fmt::format(
                       "{:%Y%m%d%H%M%S}", std::chrono::floor<std::chrono::seconds>(std::chrono::system_clock::now()))) +
               L".txt";
    }

    /*
     * Get the path for a given logfile
     */
    auto LoggerBootstrap::GetLogFilePath(const std::wstring& name) -> std::wstring
    {
        return GetLogsFolder() + L"/" + name;
    }

    auto LoggerBootstrap::GetLogsFolder() -> std::wstring
    {
        return L"logs";
    }

    auto LoggerBootstrap::GetLogFilesToKeep() -> int
    {
        return LOGFILES_TO_KEEP;
    }
} // namespace UKControllerPlugin::Log
