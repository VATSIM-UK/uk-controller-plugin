#include "pch/utilstestpch.h"
#include "log/LoggerBootstrap.h"
#include "bootstrap/PersistenceContainer.h"
#include "mock/MockWinApi.h"

using UKControllerPluginTest::Windows::MockWinApi;
using UKControllerPlugin::Bootstrap::PersistenceContainer;
using UKControllerPlugin::Log::LoggerBootstrap;
using testing::Test;
using testing::NiceMock;
using testing::StrEq;
using testing::Return;
using testing::_;

namespace UKControllerPluginUtilsTest {
    namespace Log {

        class LoggerBootstrapTest : public Test
        {
            public:
                NiceMock<MockWinApi> windows;
        };

        TEST_F(LoggerBootstrapTest, ItNotifiesTheUserIfLogsFolderCannotBeCreated)
        {
            std::wstring expected = L"Unable to create logs folder, please contact the VATSIM UK Web Department.\n\n";
            expected += L"Plugin events will not be logged.";

            ON_CALL(windows, CreateLocalFolderRecursive)
                .WillByDefault(Return(false));

            EXPECT_CALL(windows, OpenMessageBox(StrEq(expected.c_str()), _, _))
                .Times(1);

            LoggerBootstrap::Bootstrap(windows, L"foo");
        }

        TEST_F(LoggerBootstrapTest, ItReturnsAnAppropriateLogFileName)
        {
            EXPECT_TRUE(
                std::regex_match(
                    LoggerBootstrap::GetLogfileName(L"plugin"),
                    std::wregex(L"^plugin-[0-9]{4}[0-9]{2}[0-9]{2}[0-9]{2}[0-9]{2}[0-9]{2}\\.[0-9]+\\.log$")
                )
            );
        }

        TEST_F(LoggerBootstrapTest, ItReturnsExistingLogsMatchingAPrefix)
        {
            const std::set<std::wstring> windowsReturnedFiles = {
                L"plugin-1.log",
                L"plugin-2.log",
                L"someextra/plugin-25.log", // Should not be included, has a folder
                L"plugin-2.logs", // Should not be included, different extension
                L"plugin-3", // Should not be included, no extension
                L"plugin-2.log.example", // Should not be included, characters after log
                L"foo/plugin-3.log", // Should not be included, intermediate folder
                L"updater-4.log", // Should not be included, wrong prefix
                L"updater-abc.log", // Should not be included, not timestamped
                L"updater5.log", // Should not be included, no dash
            };

            std::wstring logsFolder = L"logs";
            EXPECT_CALL(this->windows, ListAllFilenamesInDirectory(logsFolder))
                .Times(1)
                .WillOnce(Return(windowsReturnedFiles));

            std::set<std::wstring> matchingLogs = LoggerBootstrap::GetExistingLogs(this->windows, L"plugin");
            EXPECT_EQ(2, matchingLogs.size());

            EXPECT_NE(matchingLogs.cend(), matchingLogs.find(L"plugin-1.log"));
            EXPECT_NE(matchingLogs.cend(), matchingLogs.find(L"plugin-2.log"));
        }

        TEST_F(LoggerBootstrapTest, ItPrunesLogfiles)
        {
            const std::set<std::wstring> windowsReturnedFiles = {
                L"plugin-1.log",
                L"plugin-2.log",
                L"plugin-3.log",
                L"plugin-4.log",
                L"plugin-5.log",
                L"plugin-6.log",
                L"plugin-7.log",
                L"loader-1.log",
                L"loader-2.log",
                L"loader-3.log",
                L"loader-4.log",
                L"updater-1.log",
            };

            std::wstring logsFolder = L"logs";
            EXPECT_CALL(this->windows, ListAllFilenamesInDirectory(logsFolder))
                .Times(1)
                .WillOnce(Return(windowsReturnedFiles));

            EXPECT_CALL(this->windows, DeleteGivenFile(std::wstring(L"logs/plugin-1.log")))
                .Times(1);

            EXPECT_CALL(this->windows, DeleteGivenFile(std::wstring(L"logs/plugin-2.log")))
                .Times(1);

            LoggerBootstrap::PruneLogs(this->windows, L"plugin");
        }

        TEST_F(LoggerBootstrapTest, ItDoesntPruneLogsIfNotRequired)
        {
            const std::set<std::wstring> windowsReturnedFiles = {
                L"plugin-1.log",
                L"plugin-2.log",
                L"plugin-3.log",
                L"plugin-4.log",
                L"plugin-5.log",
                L"loader-1.log",
                L"loader-2.log",
                L"loader-3.log",
                L"loader-4.log",
                L"updater-1.log",
            };

            std::wstring logsFolder = L"logs";
            EXPECT_CALL(this->windows, ListAllFilenamesInDirectory(logsFolder))
                .Times(1)
                .WillOnce(Return(windowsReturnedFiles));

            EXPECT_CALL(this->windows, DeleteGivenFile(testing::_))
                .Times(0);

            LoggerBootstrap::PruneLogs(this->windows, L"plugin");
        }
    }  // namespace Log
}  // namespace UKControllerPluginUtilsTest
