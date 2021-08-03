#include "pch/pch.h"
#include "loader/loader.h"
#include "mock/MockWinApi.h"
#include "mock/MockApiInterface.h"
#include "mock/MockCurlApi.h"
#include "curl/CurlRequest.h"
#include "curl/CurlResponse.h"

using testing::Test;
using testing::NiceMock;
using testing::Return;
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

                void SetUp() override
                {
                    this->unloadFunctionCalled = false;
                    this->loadFunctionCalled = false;
                    this->updateFunctionReturnValue = true;
                    this->updaterFunctionCalled = false;
                }

                static bool SetUnloadFunctionCalled()
                {
                    unloadFunctionCalled = true;
                    return updateFunctionReturnValue;
                }

                static void SetLoadFunctionCalled()
                {
                    loadFunctionCalled = true;
                }

                static bool SetUpdaterFunctionCalled()
                {
                    updaterFunctionCalled = true;
                    return updateFunctionReturnValue;
                }

                NiceMock<MockWinApi> mockWindows;
                NiceMock<MockApiInterface> mockApi;
                NiceMock<MockCurlApi> mockCurl;

                inline static bool unloadFunctionCalled;
                inline static bool loadFunctionCalled;
                inline static EuroScopePlugIn::CPlugIn* pluginInstance = reinterpret_cast<EuroScopePlugIn::CPlugIn*>(456
                );
                inline static bool updateFunctionReturnValue;
                inline static bool updaterFunctionCalled;
        };

        void UnloadFunction()
        {
            LoaderTest::SetUnloadFunctionCalled();
        }

        EuroScopePlugIn::CPlugIn* LoadFunction()
        {
            LoaderTest::SetLoadFunctionCalled();
            return LoaderTest::pluginInstance;
        }

        bool UpdateFunction()
        {
            return LoaderTest::SetUpdaterFunctionCalled();
        }


        TEST_F(LoaderTest, LoadPluginLibraryReturnsHandle)
        {
            HINSTANCE returnHandle = reinterpret_cast<HINSTANCE>(123);
            EXPECT_CALL(this->mockWindows, LoadLibraryRelative(std::wstring(L"bin/UKControllerPluginCore.dll")))
                .Times(1)
            .WillOnce(Return(returnHandle));

            EXPECT_EQ(returnHandle, LoadPluginLibrary(this->mockWindows));
        }

        TEST_F(LoaderTest, LoadPluginHandlesNullLibrary)
        {
            EXPECT_CALL(this->mockWindows, GetFunctionPointerFromLibrary(testing::_, testing::_))
                .Times(0);

            EXPECT_EQ(nullptr, LoadPlugin(nullptr, this->mockWindows));
        }

        TEST_F(LoaderTest, LoadPluginHandlesNotFindingCoreBinaryFunction)
        {
            HINSTANCE handle = reinterpret_cast<HINSTANCE>(123);

            EXPECT_CALL(this->mockWindows, GetFunctionPointerFromLibrary(handle, "LoadPlugin"))
                .Times(1)
            .WillOnce(Return(nullptr));

            EXPECT_CALL(
                this->mockWindows,
                OpenMessageBox(testing::_, testing::_, MB_OK | MB_ICONSTOP)
            )
                .Times(1);

            EXPECT_EQ(nullptr, LoadPlugin(handle, this->mockWindows));
        }

        TEST_F(LoaderTest, LoadPluginLoadsCorePlugin)
        {
            HINSTANCE handle = reinterpret_cast<HINSTANCE>(123);
            EuroScopePlugIn::CPlugIn* pluginInstance = reinterpret_cast<EuroScopePlugIn::CPlugIn*>(456);

            EXPECT_CALL(this->mockWindows, GetFunctionPointerFromLibrary(handle, "LoadPlugin"))
            .Times(1)
            .WillOnce(Return(reinterpret_cast<FARPROC>(LoadFunction)));

            EXPECT_EQ(pluginInstance, LoadPlugin(handle, this->mockWindows));
            EXPECT_TRUE(this->loadFunctionCalled);
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

        TEST_F(LoaderTest, UnloadPluginDoesntCallUnloadIfNullptr)
        {
            EXPECT_CALL(this->mockWindows, GetFunctionPointerFromLibrary(testing::_, testing::_))
                .Times(0);

            UnloadPlugin(nullptr, this->mockWindows);
        }


        TEST_F(LoaderTest, UnloadPluginHandlesNotFindingCoreBinaryFunction)
        {
            HINSTANCE handle = reinterpret_cast<HINSTANCE>(123);

            EXPECT_CALL(this->mockWindows, GetFunctionPointerFromLibrary(handle, "UnloadPlugin"))
                .Times(1)
            .WillOnce(Return(nullptr));

            EXPECT_CALL(
                this->mockWindows,
                OpenMessageBox(testing::_, testing::_, MB_OK | MB_ICONSTOP)
            )
                .Times(1);

            UnloadPlugin(handle, this->mockWindows);
        }

        TEST_F(LoaderTest, UnloadPluginUnloadsCorePlugin)
        {
            HINSTANCE handle = reinterpret_cast<HINSTANCE>(123);

            EXPECT_CALL(this->mockWindows, GetFunctionPointerFromLibrary(handle, "UnloadPlugin"))
                .Times(1)
            .WillOnce(Return(reinterpret_cast<FARPROC>(UnloadFunction)));

            UnloadPlugin(handle, this->mockWindows);
            EXPECT_TRUE(this->unloadFunctionCalled);
        }

        TEST_F(LoaderTest, RunUpdaterPreventsUpdatesIfNoLibraryLoader)
        {
            EXPECT_CALL(this->mockWindows, LoadLibraryRelative(std::wstring(L"bin/UKControllerPluginUpdater.dll")))
                .Times(1)
            .WillOnce(Return(nullptr));

            EXPECT_CALL(this->mockWindows, GetFunctionPointerFromLibrary(testing::_, testing::_))
                .Times(0);

            EXPECT_CALL(
                this->mockWindows,
                OpenMessageBox(testing::_, testing::_, MB_OK | MB_ICONSTOP)
            )
                .Times(1);

            EXPECT_THROW(RunUpdater(this->mockWindows), std::exception);
            EXPECT_FALSE(this->updaterFunctionCalled);
        }

        TEST_F(LoaderTest, RunUpdaterRunsUpdater)
        {
            HINSTANCE handle = reinterpret_cast<HINSTANCE>(123);
            EXPECT_CALL(this->mockWindows, LoadLibraryRelative(std::wstring(L"bin/UKControllerPluginUpdater.dll")))
                .Times(1)
            .WillOnce(Return(handle));

            EXPECT_CALL(this->mockWindows, GetFunctionPointerFromLibrary(handle, "PerformUpdates"))
                .Times(1)
            .WillOnce(Return(reinterpret_cast<FARPROC>(UpdateFunction)));

            EXPECT_CALL(this->mockWindows, UnloadLibrary(handle))
                .Times(1);

            RunUpdater(this->mockWindows);
            EXPECT_TRUE(this->updaterFunctionCalled);
        }

        TEST_F(LoaderTest, RunUpdaterThrowsExceptionIfUpdateNotPerformed)
        {
            HINSTANCE handle = reinterpret_cast<HINSTANCE>(123);
            this->updateFunctionReturnValue = false;
            EXPECT_CALL(this->mockWindows, LoadLibraryRelative(std::wstring(L"bin/UKControllerPluginUpdater.dll")))
                .Times(1)
            .WillOnce(Return(handle));

            EXPECT_CALL(this->mockWindows, GetFunctionPointerFromLibrary(handle, "PerformUpdates"))
                .Times(1)
            .WillOnce(Return(reinterpret_cast<FARPROC>(UpdateFunction)));

            EXPECT_CALL(this->mockWindows, UnloadLibrary(handle))
                .Times(1);

            EXPECT_THROW(RunUpdater(this->mockWindows), std::exception);
            EXPECT_TRUE(this->updaterFunctionCalled);
        }

        TEST_F(LoaderTest, FirstTimeDownloadDoesNothingIfUpdaterExists)
        {
            EXPECT_CALL(this->mockWindows, FileExists(std::wstring(L"bin/UKControllerPluginUpdater.dll")))
                .Times(1)
            .WillOnce(Return(true));

            EXPECT_TRUE(FirstTimeDownload(this->mockApi, this->mockWindows, this->mockCurl));
        }

        TEST_F(LoaderTest, FirstTimeDownloadDoesNothingIfUserDoesNotConsent)
        {
            EXPECT_CALL(this->mockWindows, FileExists(std::wstring(L"bin/UKControllerPluginUpdater.dll")))
                .Times(1)
                .WillOnce(Return(false));

            EXPECT_CALL(
                this->mockWindows,
                OpenMessageBox(testing::_, testing::_, MB_OKCANCEL | MB_ICONINFORMATION)
            )
                .Times(1)
                .WillOnce(Return(IDCANCEL));

            EXPECT_CALL(this->mockApi, GetUpdateDetails())
                .Times(0);

            EXPECT_FALSE(FirstTimeDownload(this->mockApi, this->mockWindows, this->mockCurl));
        }

        TEST_F(LoaderTest, FirstTimeDownloadDownloadsTheUpdater)
        {
            EXPECT_CALL(this->mockWindows, FileExists(std::wstring(L"bin/UKControllerPluginUpdater.dll")))
                .Times(1)
            .WillOnce(Return(false));

            EXPECT_CALL(
                this->mockWindows,
                OpenMessageBox(testing::_, testing::_, MB_OKCANCEL | MB_ICONINFORMATION)
            )
                .Times(1)
                .WillOnce(Return(IDOK));


            nlohmann::json apiData{
                {"version", "3.0.1"},
                {"updater_download_url", "foo"},
                {"core_download_url", "bar"},
                {"loader_download_url", "baz"},
            };

            EXPECT_CALL(this->mockApi, GetUpdateDetails())
                .Times(1)
                .WillOnce(Return(apiData));

            // Updater request
            CurlRequest expectedUpdaterRequest(
                "foo",
                CurlRequest::METHOD_GET
            );
            expectedUpdaterRequest.SetMaxRequestTime(0);

            CurlResponse updaterResponse("3.0.1.updater", false, 200);

            EXPECT_CALL(this->mockCurl, MakeCurlRequest(expectedUpdaterRequest))
                .Times(1)
            .WillOnce(Return(updaterResponse));

            EXPECT_CALL(
                this->mockWindows,
                WriteToFile(std::wstring(L"bin/UKControllerPluginUpdater.dll"), "3.0.1.updater", true, true)
            )
                .Times(1);


            EXPECT_TRUE(FirstTimeDownload(this->mockApi, this->mockWindows, this->mockCurl));
        }

        TEST_F(LoaderTest, FirstTimeDownloadHandlesFailureToDownloadUpdater)
        {
            EXPECT_CALL(this->mockWindows, FileExists(std::wstring(L"bin/UKControllerPluginUpdater.dll")))
                .Times(1)
            .WillOnce(Return(false));

            EXPECT_CALL(
                this->mockWindows,
                OpenMessageBox(testing::_, testing::_, MB_OKCANCEL | MB_ICONINFORMATION)
            )
                .Times(1)
                .WillOnce(Return(IDOK));


            nlohmann::json apiData{
                {"version", "3.0.1"},
                {"updater_download_url", "foo"},
                {"core_download_url", "bar"},
                {"loader_download_url", "baz"},
            };

            EXPECT_CALL(this->mockApi, GetUpdateDetails())
                .Times(1)
                .WillOnce(Return(apiData));

            // Updater request
            CurlRequest expectedUpdaterRequest(
                "foo",
                CurlRequest::METHOD_GET
            );
            expectedUpdaterRequest.SetMaxRequestTime(0);

            CurlResponse updaterResponse("3.0.1.updater", true, 200);

            EXPECT_CALL(this->mockCurl, MakeCurlRequest(expectedUpdaterRequest))
                .Times(1)
            .WillOnce(Return(updaterResponse));

            EXPECT_CALL(
                this->mockWindows,
                WriteToFile(std::wstring(L"bin/UKControllerPluginUpdater.dll"), "3.0.1.updater", true, true)
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
            .WillOnce(Return(false));

            EXPECT_CALL(
                this->mockWindows,
                OpenMessageBox(testing::_, testing::_, MB_OKCANCEL | MB_ICONINFORMATION)
            )
                .Times(1)
                .WillOnce(Return(IDOK));

            nlohmann::json apiData{
                {"version", 123}, // Bad version
                {"updater_download_url", "foo"},
                {"core_download_url", "bar"},
                {"loader_download_url", "baz"},
            };

            EXPECT_CALL(this->mockApi, GetUpdateDetails())
                .Times(1)
                .WillOnce(Return(apiData));

            EXPECT_CALL(this->mockCurl, MakeCurlRequest(testing::_))
                .Times(0);

            EXPECT_CALL(
                this->mockWindows,
                WriteToFile(std::wstring(L"bin/UKControllerPluginUpdater.dll"), "3.0.1.updater", true, true)
            )
                .Times(0);

            EXPECT_CALL(
                this->mockWindows,
                OpenMessageBox(testing::_, testing::_, MB_OK | MB_ICONSTOP)
            )
                .Times(1);

            EXPECT_FALSE(FirstTimeDownload(this->mockApi, this->mockWindows, this->mockCurl));
        }
    } // namespace Loader
} // namespace UKControllerPluginLoaderTest
