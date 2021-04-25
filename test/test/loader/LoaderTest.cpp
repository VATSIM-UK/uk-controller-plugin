#include "pch/loadertestpch.h"
#include "loader/loader.h"
#include "api/ApiException.h"
#include "updater/PerformUpdates.h"
#include "mock/MockWinApi.h"
#include "mock/MockApiInterface.h"
#include "mock/MockCurlApi.h"
#include "curl/CurlRequest.h"
#include "curl/CurlResponse.h"

using testing::Test;
using testing::NiceMock;
using UKControllerPluginTest::Windows::MockWinApi;
using UKControllerPluginTest::Api::MockApiInterface;
using UKControllerPluginTest::Curl::MockCurlApi;
using UKControllerPlugin::Curl::CurlRequest;
using UKControllerPlugin::Curl::CurlResponse;

namespace UKControllerPluginLoaderTest {
    namespace Loader {
        class LoaderTest : public Test
        {
            public:
                NiceMock<MockWinApi> mockWindows;
                NiceMock<MockApiInterface> mockApi;
                NiceMock<MockCurlApi> mockCurl;
        };

        TEST_F(LoaderTest, LoadPluginLibraryReturnsHandle)
        {
            HINSTANCE returnHandle = reinterpret_cast<HINSTANCE>(123);
            EXPECT_CALL(this->mockWindows, LoadLibraryRelative(std::wstring(L"bin/UKControllerPluginCore.dll")))
                .Times(1)
                .WillOnce(testing::Return(returnHandle));

            EXPECT_EQ(returnHandle, LoadPluginLibrary(this->mockWindows));
        }

        TEST_F(LoaderTest, UnloadPluginLibraryCallsUnload)
        {
            HINSTANCE handle = reinterpret_cast<HINSTANCE>(123);
            EXPECT_CALL(this->mockWindows, UnloadLibrary(handle))
                .Times(1);

            UnloadPluginLibrary(handle, this->mockWindows);
        }

        TEST_F(LoaderTest, UnloadPluginLibraryDoesntCallUnloadIfNullptr)
        {
            EXPECT_CALL(this->mockWindows, UnloadLibrary(testing::_))
                .Times(0);

            UnloadPluginLibrary(nullptr, this->mockWindows);
        }

        TEST_F(LoaderTest, RunUpdaterPreventsUpdatesIfNoLibraryLoader)
        {
            EXPECT_CALL(this->mockWindows, LoadLibraryRelative(std::wstring(L"bin/UKControllerPluginUpdater.dll")))
                .Times(1)
                .WillOnce(testing::Return(nullptr));

            EXPECT_CALL(
                this->mockWindows,
                OpenMessageBox(testing::_, testing::_, MB_OK | MB_ICONSTOP)
            )
                .Times(1);

            EXPECT_THROW(RunUpdater(this->mockWindows), std::exception);
        }

        TEST_F(LoaderTest, FirstTimeDownloadDoesNothingIfUpdaterExists)
        {
            EXPECT_CALL(this->mockWindows, FileExists(std::wstring(L"bin/UKControllerPluginUpdater.dll")))
                .Times(1)
            .WillOnce(testing::Return(true));

            EXPECT_TRUE(FirstTimeDownload(this->mockApi, this->mockWindows, this->mockCurl));
        }

        TEST_F(LoaderTest, FirstTimeDownloadDownloadsTheUpdater)
        {
            EXPECT_CALL(this->mockWindows, FileExists(std::wstring(L"bin/UKControllerPluginUpdater.dll")))
                .Times(1)
            .WillOnce(testing::Return(false));


            nlohmann::json apiData{
                {"version", "3.0.1"},
                {"updater_download_url", "foo"},
                {"core_download_url", "bar"},
                {"loader_download_url", "baz"},
            };

            EXPECT_CALL(this->mockApi, GetUpdateDetails())
                .Times(1)
                .WillOnce(testing::Return(apiData));

            // Updater request
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
                WriteToFile(std::wstring(L"bin/UKControllerPluginUpdater.dll"), "3.0.1.updater", true)
            )
                .Times(1);


            EXPECT_TRUE(FirstTimeDownload(this->mockApi, this->mockWindows, this->mockCurl));
        }

        TEST_F(LoaderTest, FirstTimeDownloadHandlesFailureToDownloadUpdater)
        {
            EXPECT_CALL(this->mockWindows, FileExists(std::wstring(L"bin/UKControllerPluginUpdater.dll")))
                .Times(1)
                .WillOnce(testing::Return(false));


            nlohmann::json apiData{
                {"version", "3.0.1"},
                {"updater_download_url", "foo"},
                {"core_download_url", "bar"},
                {"loader_download_url", "baz"},
            };

            EXPECT_CALL(this->mockApi, GetUpdateDetails())
                .Times(1)
                .WillOnce(testing::Return(apiData));

            // Updater request
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
                WriteToFile(std::wstring(L"bin/UKControllerPluginUpdater.dll"), "3.0.1.updater", true)
            )
                .Times(0);

            EXPECT_CALL(
                this->mockWindows,
                OpenMessageBox(testing::_, testing::_, MB_OK | MB_ICONSTOP)
            )
                .Times(1);

            EXPECT_FALSE(FirstTimeDownload(this->mockApi, this->mockWindows, this->mockCurl));
        }

        TEST_F(LoaderTest, FirstTimeDownloadBadUpdateData)
        {
            EXPECT_CALL(this->mockWindows, FileExists(std::wstring(L"bin/UKControllerPluginUpdater.dll")))
                .Times(1)
                .WillOnce(testing::Return(false));

            nlohmann::json apiData{
                {"version", 123}, // Bad version
                {"updater_download_url", "foo"},
                {"core_download_url", "bar"},
                {"loader_download_url", "baz"},
            };

            EXPECT_CALL(this->mockApi, GetUpdateDetails())
                .Times(1)
                .WillOnce(testing::Return(apiData));

            EXPECT_CALL(this->mockCurl, MakeCurlRequest(testing::_))
                .Times(0);

            EXPECT_CALL(
                this->mockWindows,
                WriteToFile(std::wstring(L"bin/UKControllerPluginUpdater.dll"), "3.0.1.updater", true)
            )
                .Times(0);

            EXPECT_CALL(
                this->mockWindows,
                OpenMessageBox(testing::_, testing::_, MB_OK | MB_ICONSTOP)
            )
                .Times(1);

            EXPECT_FALSE(FirstTimeDownload(this->mockApi, this->mockWindows, this->mockCurl));
        }
    } // namespace Updater
} // namespace UKControllerPluginUpdaterTest
