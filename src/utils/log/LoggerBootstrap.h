#pragma once

// Forward declarations
namespace UKControllerPlugin {
    namespace Windows
    {
        class WinApiInterface;
    } // namespace Windows
}  // namespace UKControllerPlugin
// END

/*

*/
namespace UKControllerPlugin {
    namespace Log {

        /*
            A class for bootstrapping the logger.
        */
        class LoggerBootstrap
        {
            public:
                static void Bootstrap(
                    Windows::WinApiInterface& windows,
                    std::wstring logfilePrefix
                );
                static bool CreateLogsFolder(Windows::WinApiInterface& windows);
                static void PruneLogs(Windows::WinApiInterface& windows, std::wstring logFilePrefix);
                static std::set<std::wstring> GetExistingLogs(
                    Windows::WinApiInterface& windows,
                    std::wstring logFilePrefix
                );
                static std::wstring GetLogfileName(std::wstring logFilePrefix);

            private:

                static std::wstring GetLogFilePath(std::wstring logname);
                static void CreateNullLogger();
        };
    }  // namespace Log
}  // namespace UKControllerPlugin
