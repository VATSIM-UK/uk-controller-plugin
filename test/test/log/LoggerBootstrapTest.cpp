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
                    std::wregex(L"^plugin-[0-9]{4}[0-9]{2}[0-9]{2}[0-9]{2}[0-9]{2}[0-9]{2}\\.txt$")
                )
            );
        }

        TEST_F(LoggerBootstrapTest, ItReturnsExistingLogsMatchingAPrefix)
        {
            const std::set<std::wstring> windowsReturnedFiles = {
                L"plugin.txt", // Should not be included, no timestamp
                L"plugin-2.3.txt", // Should not be included, has dots
                L"plugin-3.txt",
                L"plugin-20210314162200.txt",
                L"plugin-3.1.2.txt", // Should not be included, too many dots
                L"someextra/plugin-25.txt", // Should not be included, has a folder
                L"plugin-2.txts", // Should not be included, different extension
                L"plugin-3", // Should not be included, no extension
                L"plugin-2.txt.example", // Should not be included, characters after log
                L"foo/plugin-3.txt", // Should not be included, intermediate folder
                L"updater-4.txt", // Should not be included, wrong prefix
                L"updater-abc.txt", // Should not be included, not timestamped
                L"updater5.txt", // Should not be included, no dash
            };

            std::wstring logsFolder = L"logs";
            EXPECT_CALL(this->windows, ListAllFilenamesInDirectory(logsFolder))
                .Times(1)
                .WillOnce(Return(windowsReturnedFiles));

            std::set<std::wstring> matchingLogs = LoggerBootstrap::GetExistingLogs(this->windows, L"plugin");
            EXPECT_EQ(2, matchingLogs.size());

            EXPECT_NE(matchingLogs.cend(), matchingLogs.find(L"plugin-3.txt"));
            EXPECT_NE(matchingLogs.cend(), matchingLogs.find(L"plugin-20210314162200.txt"));
        }

        TEST_F(LoggerBootstrapTest, ItPrunesLogfiles)
        {
            const std::set<std::wstring> windowsReturnedFiles = {
                L"plugin-1.txt",
                L"plugin-2.txt",
                L"plugin-3.txt",
                L"plugin-4.txt",
                L"plugin-5.txt",
                L"plugin-6.txt",
                L"plugin-7.txt",
                L"plugin-8.txt",
                L"loader-9.txt",
                L"loader-10.txt",
                L"loader-11.txt",
                L"loader-12.txt",
                L"updater-13.txt",
            };

            std::wstring logsFolder = L"logs";
            EXPECT_CALL(this->windows, ListAllFilenamesInDirectory(logsFolder))
                .Times(1)
                .WillOnce(Return(windowsReturnedFiles));

            EXPECT_CALL(this->windows, DeleteGivenFile(std::wstring(L"logs/plugin-1.txt")))
                .Times(1);

            EXPECT_CALL(this->windows, DeleteGivenFile(std::wstring(L"logs/plugin-2.txt")))
                .Times(1);

            EXPECT_CALL(this->windows, DeleteGivenFile(std::wstring(L"logs/plugin-3.txt")))
                .Times(1);

            LoggerBootstrap::PruneLogs(this->windows, L"plugin");
        }

        TEST_F(LoggerBootstrapTest, ItDoesntPruneLogsIfNotRequired)
        {
            const std::set<std::wstring> windowsReturnedFiles = {
                L"plugin-1.txt",
                L"plugin-2.txt",
                L"plugin-3.txt",
                L"plugin-4.txt",
                L"plugin-5.txt",
                L"loader-1.txt",
                L"loader-2.txt",
                L"loader-3.txt",
                L"loader-4.txt",
                L"updater-1.txt",
            };

            std::wstring logsFolder = L"logs";
            EXPECT_CALL(this->windows, ListAllFilenamesInDirectory(logsFolder))
                .Times(1)
                .WillOnce(Return(windowsReturnedFiles));

            EXPECT_CALL(this->windows, DeleteGivenFile(testing::_))
                .Times(0);

            LoggerBootstrap::PruneLogs(this->windows, L"plugin");
        }

        TEST_F(LoggerBootstrapTest, ItDoesntPruneLogFilesIfNotEnoughToPrune)
        {
            const std::set<std::wstring> windowsReturnedFiles = {
                L"plugin-1.txt",
                L"plugin-2.txt",
                L"plugin-3.txt"
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
