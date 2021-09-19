#pragma once

namespace UKControllerPlugin::Windows {
    class WinApiInterface;
} // namespace UKControllerPlugin::Windows

namespace UKControllerPlugin::Log {

    /*
        A class for bootstrapping the logger.
    */
    class LoggerBootstrap
    {
        public:
        static void Bootstrap(Windows::WinApiInterface& windows, const std::wstring& logfilePrefix);
        static auto CreateLogsFolder(Windows::WinApiInterface& windows) -> bool;
        static void PruneLogs(Windows::WinApiInterface& windows, const std::wstring& logFilePrefix);
        static auto GetExistingLogs(Windows::WinApiInterface& windows, const std::wstring& logFilePrefix)
            -> std::set<std::wstring>;
        static auto GetLogfileName(const std::wstring& logFilePrefix) -> std::wstring;

        private:
        static auto GetLogFilePath(const std::wstring& logname) -> std::wstring;
        static void CreateNullLogger();
        [[nodiscard]] static auto GetLogsFolder() -> std::wstring;
        [[nodiscard]] static auto GetLogFilesToKeep() -> int;

        inline static const int LOGFILES_TO_KEEP = 5;
    };
} // namespace UKControllerPlugin::Log
