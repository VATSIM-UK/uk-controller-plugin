#include "pch/updatertestpch.h"
#include "api/ApiException.h"
#include "updater/PerformUpdates.h"
#include "mock/MockWinApi.h"
#include "mock/MockApiInterface.h"
#include "mock/MockCurlApi.h"
#include "curl/CurlRequest.h"
#include "curl/CurlResponse.h"
#include "update/LoadChangelog.h"

using testing::Test;
using testing::NiceMock;
using UKControllerPluginTest::Windows::MockWinApi;
using UKControllerPluginTest::Api::MockApiInterface;
using UKControllerPluginTest::Curl::MockCurlApi;
using UKControllerPlugin::Curl::CurlRequest;
using UKControllerPlugin::Curl::CurlResponse;

namespace UKControllerPluginUpdaterTest {
    namespace Updater {
        class PerformUpdatesTest : public Test
        {
            public:
                NiceMock<MockWinApi> mockWindows;
                NiceMock<MockApiInterface> mockApi;
                NiceMock<MockCurlApi> mockCurl;
        };

        TEST_F(PerformUpdatesTest, ItHasAnOldUpdaterLocation)
        {
            EXPECT_EQ(L"bin/UKControllerPluginUpdater.dll.old", GetOldUpdaterLocation());
        }

        TEST_F(PerformUpdatesTest, ItGetsVersionFromJson)
        {
            nlohmann::json version{
                {"version", "3.0.1"},
                {"not_version", "3.0.2"}
            };
            EXPECT_EQ("3.0.1", GetVersionFromJson(version));
        }

        TEST_F(PerformUpdatesTest, UpdateIsRequiredIfNoLockfile)
        {
            nlohmann::json version{
                {"version", "3.0.1"},
                {"not_version", "3.0.2"}
            };

            ON_CALL(this->mockWindows, FileExists(std::wstring(L"version.lock")))
                .WillByDefault(testing::Return(false));

            ON_CALL(this->mockWindows, FileExists(std::wstring(L"bin/UKControllerPluginCore.dll")))
                .WillByDefault(testing::Return(true));

            EXPECT_TRUE(UpdateRequired(mockWindows, version));
        }

        TEST_F(PerformUpdatesTest, UpdateIsRequiredIfLockfileDoesntMatchVersion)
        {
            nlohmann::json version{
                {"version", "3.0.1"},
                {"not_version", "3.0.2"}
            };

            ON_CALL(this->mockWindows, FileExists(std::wstring(L"version.lock")))
                .WillByDefault(testing::Return(true));

            ON_CALL(this->mockWindows, ReadFromFileMock(std::wstring(L"version.lock"), true))
                .WillByDefault(testing::Return("3.0.0"));

            ON_CALL(this->mockWindows, FileExists(std::wstring(L"bin/UKControllerPluginCore.dll")))
                .WillByDefault(testing::Return(true));

            EXPECT_TRUE(UpdateRequired(mockWindows, version));
        }

        TEST_F(PerformUpdatesTest, UpdateIsRequiredIfCoreBinaryMissing)
        {
            nlohmann::json version{
                {"version", "3.0.1"},
                {"not_version", "3.0.2"}
            };

            ON_CALL(this->mockWindows, FileExists(std::wstring(L"version.lock")))
                .WillByDefault(testing::Return(true));

            ON_CALL(this->mockWindows, ReadFromFileMock(std::wstring(L"version.lock"), true))
                .WillByDefault(testing::Return("3.0.1"));

            ON_CALL(this->mockWindows, FileExists(std::wstring(L"bin/UKControllerPluginCore.dll")))
                .WillByDefault(testing::Return(false));

            EXPECT_TRUE(UpdateRequired(mockWindows, version));
        }

        TEST_F(PerformUpdatesTest, UpdateIsNotRequiredIfLockfileMatchesVersion)
        {
            nlohmann::json version{
                {"version", "3.0.1"},
                {"not_version", "3.0.2"}
            };

            ON_CALL(this->mockWindows, FileExists(std::wstring(L"version.lock")))
                .WillByDefault(testing::Return(true));

            ON_CALL(this->mockWindows, ReadFromFileMock(std::wstring(L"version.lock"), true))
                .WillByDefault(testing::Return("3.0.1"));

            ON_CALL(this->mockWindows, FileExists(std::wstring(L"bin/UKControllerPluginCore.dll")))
                .WillByDefault(testing::Return(true));

            EXPECT_FALSE(UpdateRequired(mockWindows, version));
        }

        TEST_F(PerformUpdatesTest, ItDoesntDoUpdateIfPluginIsDuplicate)
        {
            EXPECT_CALL(this->mockApi, GetUpdateDetails)
                .Times(0);

            CheckForUpdates(mockApi, mockWindows, mockCurl, true);
        }

        TEST_F(PerformUpdatesTest, ItHandlesApiExceptionsWhenGettingUpdateDetails)
        {
            ON_CALL(this->mockApi, GetUpdateDetails)
                .WillByDefault(testing::Throw(UKControllerPlugin::Api::ApiException("foo")));

            EXPECT_CALL(
                this->mockWindows,
                OpenMessageBox(testing::_, testing::_, MB_OK | MB_ICONSTOP)
            )
                .Times(1);

            CheckForUpdates(mockApi, mockWindows, mockCurl, false);
        }

        TEST_F(PerformUpdatesTest, ItHandlesInvalidApiDataWhenGettingUpdateDetails)
        {
            nlohmann::json apiData{
                {"foo", "bar"}
            };

            ON_CALL(this->mockApi, GetUpdateDetails)
                .WillByDefault(testing::Return(apiData));

            EXPECT_CALL(
                this->mockWindows,
                OpenMessageBox(testing::_, testing::_, MB_OK | MB_ICONSTOP)
            )
                .Times(1);

            CheckForUpdates(mockApi, mockWindows, mockCurl, false);
        }

        TEST_F(PerformUpdatesTest, ItDoesntDoUpdateIfNoUpdateRequired)
        {
            nlohmann::json apiData{
                {"version", "3.0.1"},
                {"updater_download_url", "foo"},
                {"core_download_url", "bar"},
                {"loader_download_url", "baz"},
            };

            ON_CALL(this->mockApi, GetUpdateDetails)
                .WillByDefault(testing::Return(apiData));

            ON_CALL(this->mockWindows, FileExists(std::wstring(L"version.lock")))
                .WillByDefault(testing::Return(true));

            ON_CALL(this->mockWindows, FileExists(std::wstring(L"bin/UKControllerPluginCore.dll")))
                .WillByDefault(testing::Return(true));

            ON_CALL(this->mockWindows, ReadFromFileMock(std::wstring(L"version.lock"), true))
                .WillByDefault(testing::Return("3.0.1"));

            EXPECT_CALL(this->mockWindows, MoveFileToNewLocation(testing::_, testing::_))
                .Times(0);

            CheckForUpdates(mockApi, mockWindows, mockCurl, false);
        }

        TEST_F(PerformUpdatesTest, ItPerformsUpdatesWithNoChangelog)
        {
            nlohmann::json apiData{
                {"version", "3.0.1"},
                {"updater_download_url", "foo"},
                {"core_download_url", "bar"},
                {"loader_download_url", "baz"},
            };

            ON_CALL(this->mockApi, GetUpdateDetails)
                .WillByDefault(testing::Return(apiData));

            ON_CALL(this->mockWindows, FileExists(std::wstring(L"version.lock")))
                .WillByDefault(testing::Return(true));

            ON_CALL(this->mockWindows, ReadFromFileMock(std::wstring(L"version.lock"), true))
                .WillByDefault(testing::Return("3.0.0"));

            EXPECT_CALL(
                this->mockWindows,
                MoveFileToNewLocation(
                    std::wstring(L"bin/UKControllerPluginUpdater.dll"),
                    std::wstring(L"bin/UKControllerPluginUpdater.dll.old")
                )
            )
                .Times(1);

            // Core things
            CurlRequest expectedCoreRequest(
                "bar",
                CurlRequest::METHOD_GET
            );
            expectedCoreRequest.SetMaxRequestTime(0);

            CurlResponse coreResponse("3.0.1.core", false, 200);

            EXPECT_CALL(this->mockCurl, MakeCurlRequest(expectedCoreRequest))
                .Times(1)
                .WillOnce(testing::Return(coreResponse));

            EXPECT_CALL(
                this->mockWindows,
                WriteToFile(std::wstring(L"bin/UKControllerPluginCore.dll"), "3.0.1.core", true, true)
            )
                .Times(1);

            // Updater things
            CurlRequest expectedUpdaterRequest(
                "foo",
                CurlRequest::METHOD_GET
            );
            expectedUpdaterRequest.SetMaxRequestTime(0);

            CurlResponse updaterResponse("3.0.1.updater", false, 200);

            EXPECT_CALL(this->mockCurl, MakeCurlRequest(expectedUpdaterRequest))
                .Times(1)
                .WillOnce(testing::Return(updaterResponse));

            EXPECT_CALL(
                this->mockWindows,
                WriteToFile(std::wstring(L"bin/UKControllerPluginUpdater.dll"), "3.0.1.updater", true, true)
            )
                .Times(1);

            // Lockfile update
            EXPECT_CALL(
                this->mockWindows,
                WriteToFile(std::wstring(L"version.lock"), "3.0.1", true, false)
            )
                .Times(1);

            // Update message
            EXPECT_CALL(
                this->mockWindows,
                OpenMessageBox(testing::_, testing::_, MB_YESNO | MB_ICONINFORMATION)
            )
                .Times(1)
                .WillOnce(testing::Return(IDNO));

            EXPECT_CALL(
                this->mockWindows,
                OpenWebBrowser(testing::_)
            )
                .Times(0);

            CheckForUpdates(mockApi, mockWindows, mockCurl, false);
        }

        TEST_F(PerformUpdatesTest, ItPerformsUpdatesWithChangelog)
        {
            nlohmann::json apiData{
                {"version", "3.0.1"},
                {"updater_download_url", "foo"},
                {"core_download_url", "bar"},
                {"loader_download_url", "baz"},
            };

            ON_CALL(this->mockApi, GetUpdateDetails)
                .WillByDefault(testing::Return(apiData));

            ON_CALL(this->mockWindows, FileExists(std::wstring(L"version.lock")))
                .WillByDefault(testing::Return(true));

            ON_CALL(this->mockWindows, ReadFromFileMock(std::wstring(L"version.lock"), true))
                .WillByDefault(testing::Return("3.0.0"));

            EXPECT_CALL(
                this->mockWindows,
                MoveFileToNewLocation(
                    std::wstring(L"bin/UKControllerPluginUpdater.dll"),
                    std::wstring(L"bin/UKControllerPluginUpdater.dll.old")
                )
            )
                .Times(1);

            // Core things
            CurlRequest expectedCoreRequest(
                "bar",
                CurlRequest::METHOD_GET
            );
            expectedCoreRequest.SetMaxRequestTime(0);

            CurlResponse coreResponse("3.0.1.core", false, 200);

            EXPECT_CALL(this->mockCurl, MakeCurlRequest(expectedCoreRequest))
                .Times(1)
                .WillOnce(testing::Return(coreResponse));

            EXPECT_CALL(
                this->mockWindows,
                WriteToFile(std::wstring(L"bin/UKControllerPluginCore.dll"), "3.0.1.core", true, true)
            )
                .Times(1);

            // Updater things
            CurlRequest expectedUpdaterRequest(
                "foo",
                CurlRequest::METHOD_GET
            );
            expectedUpdaterRequest.SetMaxRequestTime(0);

            CurlResponse updaterResponse("3.0.1.updater", false, 200);

            EXPECT_CALL(this->mockCurl, MakeCurlRequest(expectedUpdaterRequest))
                .Times(1)
                .WillOnce(testing::Return(updaterResponse));

            EXPECT_CALL(
                this->mockWindows,
                WriteToFile(std::wstring(L"bin/UKControllerPluginUpdater.dll"), "3.0.1.updater", true, true)
            )
                .Times(1);

            // Lockfile update
            EXPECT_CALL(
                this->mockWindows,
                WriteToFile(std::wstring(L"version.lock"), "3.0.1", true, false)
            )
                .Times(1);

            // Update message
            EXPECT_CALL(
                this->mockWindows,
                OpenMessageBox(testing::_, testing::_, MB_YESNO | MB_ICONINFORMATION)
            )
                .Times(1)
                .WillOnce(testing::Return(IDYES));

            EXPECT_CALL(
                this->mockWindows,
                OpenWebBrowser(UKControllerPlugin::Update::changelogUrl)
            )
                .Times(1);

            CheckForUpdates(mockApi, mockWindows, mockCurl, false);
        }

        TEST_F(PerformUpdatesTest, ItHandlesFailedUpdaterDownloads)
        {
            nlohmann::json apiData{
                {"version", "3.0.1"},
                {"updater_download_url", "foo"},
                {"core_download_url", "bar"},
                {"loader_download_url", "baz"},
            };

            ON_CALL(this->mockApi, GetUpdateDetails)
                .WillByDefault(testing::Return(apiData));

            ON_CALL(this->mockWindows, FileExists(std::wstring(L"version.lock")))
                .WillByDefault(testing::Return(true));

            ON_CALL(this->mockWindows, ReadFromFileMock(std::wstring(L"version.lock"), true))
                .WillByDefault(testing::Return("3.0.0"));

            EXPECT_CALL(
                this->mockWindows,
                MoveFileToNewLocation(
                    std::wstring(L"bin/UKControllerPluginUpdater.dll"),
                    std::wstring(L"bin/UKControllerPluginUpdater.dll.old")
                )
            )
                .Times(1);

            // Core things
            CurlRequest expectedCoreRequest(
                "bar",
                CurlRequest::METHOD_GET
            );
            expectedCoreRequest.SetMaxRequestTime(0);

            CurlResponse coreResponse("3.0.1.core", false, 200);

            EXPECT_CALL(this->mockCurl, MakeCurlRequest(expectedCoreRequest))
                .Times(1)
                .WillOnce(testing::Return(coreResponse));

            EXPECT_CALL(
                this->mockWindows,
                WriteToFile(std::wstring(L"bin/UKControllerPluginCore.dll"), "3.0.1.core", true, true)
            )
                .Times(1);

            // Updater things
            CurlRequest expectedUpdaterRequest(
                "foo",
                CurlRequest::METHOD_GET
            );
            expectedUpdaterRequest.SetMaxRequestTime(0);

            CurlResponse updaterResponse("3.0.1.updater", true, 200);

            EXPECT_CALL(this->mockCurl, MakeCurlRequest(expectedUpdaterRequest))
                .Times(1)
                .WillOnce(testing::Return(updaterResponse));

            EXPECT_CALL(
                this->mockWindows,
                WriteToFile(std::wstring(L"bin/UKControllerPluginUpdater.dll"), "3.0.1.updater", true, true)
            )
                .Times(0);

            // Lockfile update
            EXPECT_CALL(
                this->mockWindows,
                WriteToFile(std::wstring(L"version.lock"), "3.0.1", true, false)
            )
                .Times(0);

            // Messagebox
            EXPECT_CALL(
                this->mockWindows,
                OpenMessageBox(testing::_, testing::_, MB_OK | MB_ICONSTOP)
            )
                .Times(1);

            CheckForUpdates(mockApi, mockWindows, mockCurl, false);
        }

        TEST_F(PerformUpdatesTest, ItHandlesFailedCoreDownloads)
        {
            nlohmann::json apiData{
                {"version", "3.0.1"},
                {"updater_download_url", "foo"},
                {"core_download_url", "bar"},
                {"loader_download_url", "baz"},
            };

            ON_CALL(this->mockApi, GetUpdateDetails)
                .WillByDefault(testing::Return(apiData));

            ON_CALL(this->mockWindows, FileExists(std::wstring(L"version.lock")))
                .WillByDefault(testing::Return(true));

            ON_CALL(this->mockWindows, ReadFromFileMock(std::wstring(L"version.lock"), true))
                .WillByDefault(testing::Return("3.0.0"));

            EXPECT_CALL(
                this->mockWindows,
                MoveFileToNewLocation(
                    std::wstring(L"bin/UKControllerPluginUpdater.dll"),
                    std::wstring(L"bin/UKControllerPluginUpdater.dll.old")
                )
            )
                .Times(1);

            // Core things
            CurlRequest expectedCoreRequest(
                "bar",
                CurlRequest::METHOD_GET
            );
            expectedCoreRequest.SetMaxRequestTime(0);

            CurlResponse coreResponse("3.0.1.core", true, 200);

            EXPECT_CALL(this->mockCurl, MakeCurlRequest(expectedCoreRequest))
                .Times(1)
                .WillOnce(testing::Return(coreResponse));

            EXPECT_CALL(
                this->mockWindows,
                WriteToFile(std::wstring(L"bin/UKControllerPluginCore.dll"), "3.0.1.core", true, true)
            )
                .Times(0);

            // Updater things
            CurlRequest expectedUpdaterRequest(
                "foo",
                CurlRequest::METHOD_GET
            );
            expectedUpdaterRequest.SetMaxRequestTime(0);

            CurlResponse updaterResponse("3.0.1.updater", true, 200);

            EXPECT_CALL(this->mockCurl, MakeCurlRequest(expectedUpdaterRequest))
            .Times(0)
                .WillOnce(testing::Return(updaterResponse));

            EXPECT_CALL(
                this->mockWindows,
                WriteToFile(std::wstring(L"bin/UKControllerPluginUpdater.dll"), "3.0.1.updater", true, true)
            )
                .Times(0);

            // Lockfile update
            EXPECT_CALL(
                this->mockWindows,
                WriteToFile(std::wstring(L"version.lock"), "3.0.1", true, false)
            )
                .Times(0);

            // Messagebox
            EXPECT_CALL(
                this->mockWindows,
                OpenMessageBox(testing::_, testing::_, MB_OK | MB_ICONSTOP)
            )
                .Times(1);

            CheckForUpdates(mockApi, mockWindows, mockCurl, false);
        }
    } // namespace Updater
} // namespace UKControllerPluginUpdaterTest
