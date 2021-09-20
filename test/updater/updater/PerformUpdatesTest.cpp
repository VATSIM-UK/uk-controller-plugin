#include "api/ApiException.h"
#include "update/LoadChangelog.h"
#include "updater/PerformUpdates.h"

using testing::NiceMock;
using testing::Test;
using UKControllerPlugin::Curl::CurlRequest;
using UKControllerPlugin::Curl::CurlResponse;
using UKControllerPluginTest::Api::MockApiInterface;
using UKControllerPluginTest::Curl::MockCurlApi;
using UKControllerPluginTest::Windows::MockWinApi;

namespace UKControllerPluginUpdaterTest::Updater {
    class PerformUpdatesTest : public Test
    {
        public:
        void SetUp() override
        {
            this->version = "3.0.0";
            this->versionFunctionCalled = false;
        }

        static const char* VersionFunctionCalled()
        {
            versionFunctionCalled = true;
            return version;
        }

        inline static bool versionFunctionCalled;
        inline static const char* version;
        NiceMock<MockWinApi> mockWindows;
        NiceMock<MockApiInterface> mockApi;
        NiceMock<MockCurlApi> mockCurl;
    };

    __stdcall const char* VersionFunction()
    {
        return PerformUpdatesTest::VersionFunctionCalled();
    }

    TEST_F(PerformUpdatesTest, ItHasAnOldUpdaterLocation)
    {
        EXPECT_EQ(L"bin/UKControllerPluginUpdater.dll.old", GetOldUpdaterLocation());
    }

    TEST_F(PerformUpdatesTest, ItGetsVersionFromJson)
    {
        nlohmann::json version{{"version", "3.0.1"}, {"not_version", "3.0.2"}};
        EXPECT_EQ("3.0.1", GetVersionFromJson(version));
    }

    TEST_F(PerformUpdatesTest, UpdateIsRequiredIfNoCoreBinary)
    {
        nlohmann::json version{{"version", "3.0.1"}, {"not_version", "3.0.2"}};

        ON_CALL(this->mockWindows, FileExists(std::wstring(L"bin/UKControllerPluginCore.dll")))
            .WillByDefault(testing::Return(false));

        EXPECT_TRUE(UpdateRequired(mockWindows, version));
    }

    TEST_F(PerformUpdatesTest, UpdateIsRequiredIfCoreBinaryCantBeLoaded)
    {
        nlohmann::json version{{"version", "3.0.1"}, {"not_version", "3.0.2"}};

        ON_CALL(this->mockWindows, FileExists(std::wstring(L"bin/UKControllerPluginCore.dll")))
            .WillByDefault(testing::Return(true));

        HINSTANCE handle = nullptr;
        EXPECT_CALL(this->mockWindows, LoadLibraryRelative(std::wstring(L"bin/UKControllerPluginCore.dll")))
            .Times(1)
            .WillOnce(testing::Return(handle));

        EXPECT_TRUE(UpdateRequired(mockWindows, version));
    }

    TEST_F(PerformUpdatesTest, UpdateIsRequiredIfVersionFunctionCantBeFoundInCoreBinary)
    {
        nlohmann::json version{{"version", "3.0.1"}, {"not_version", "3.0.2"}};

        ON_CALL(this->mockWindows, FileExists(std::wstring(L"bin/UKControllerPluginCore.dll")))
            .WillByDefault(testing::Return(true));

        HINSTANCE handle = reinterpret_cast<HINSTANCE>(123);
        EXPECT_CALL(this->mockWindows, LoadLibraryRelative(std::wstring(L"bin/UKControllerPluginCore.dll")))
            .Times(1)
            .WillOnce(testing::Return(handle));

        EXPECT_CALL(this->mockWindows, GetFunctionPointerFromLibrary(handle, "GetPluginVersion"))
            .Times(1)
            .WillOnce(testing::Return(nullptr));

        EXPECT_CALL(this->mockWindows, UnloadLibrary(handle)).Times(1);

        EXPECT_TRUE(UpdateRequired(mockWindows, version));
    }

    TEST_F(PerformUpdatesTest, UpdateIsRequiredIfVersionOfCoreBinaryIsNotLatest)
    {
        nlohmann::json version{{"version", "3.0.1"}, {"not_version", "3.0.2"}};

        ON_CALL(this->mockWindows, FileExists(std::wstring(L"bin/UKControllerPluginCore.dll")))
            .WillByDefault(testing::Return(true));

        HINSTANCE handle = reinterpret_cast<HINSTANCE>(123);
        EXPECT_CALL(this->mockWindows, LoadLibraryRelative(std::wstring(L"bin/UKControllerPluginCore.dll")))
            .Times(1)
            .WillOnce(testing::Return(handle));

        EXPECT_CALL(this->mockWindows, GetFunctionPointerFromLibrary(handle, "GetPluginVersion"))
            .Times(1)
            .WillOnce(testing::Return(reinterpret_cast<FARPROC>(VersionFunction))); // NOLINT

        EXPECT_CALL(this->mockWindows, UnloadLibrary(handle)).Times(1);

        EXPECT_TRUE(UpdateRequired(mockWindows, version));
        EXPECT_TRUE(this->versionFunctionCalled);
    }

    TEST_F(PerformUpdatesTest, UpdateIsNotRequiredIfVersionOfCoreBinaryIsLatest)
    {
        nlohmann::json version{{"version", "3.0.0"}, {"not_version", "3.0.2"}};

        ON_CALL(this->mockWindows, FileExists(std::wstring(L"bin/UKControllerPluginCore.dll")))
            .WillByDefault(testing::Return(true));

        HINSTANCE handle = reinterpret_cast<HINSTANCE>(123);
        EXPECT_CALL(this->mockWindows, LoadLibraryRelative(std::wstring(L"bin/UKControllerPluginCore.dll")))
            .Times(1)
            .WillOnce(testing::Return(handle));

        EXPECT_CALL(this->mockWindows, GetFunctionPointerFromLibrary(handle, "GetPluginVersion"))
            .Times(1)
            .WillOnce(testing::Return(reinterpret_cast<FARPROC>(VersionFunction))); // NOLINT

        EXPECT_CALL(this->mockWindows, UnloadLibrary(handle)).Times(1);

        EXPECT_FALSE(UpdateRequired(mockWindows, version));
        EXPECT_TRUE(this->versionFunctionCalled);
    }

    TEST_F(PerformUpdatesTest, ItDoesntDoUpdateIfPluginIsDuplicate)
    {
        EXPECT_CALL(this->mockApi, GetUpdateDetails).Times(0);

        EXPECT_TRUE(CheckForUpdates(mockApi, mockWindows, mockCurl, true));
    }

    TEST_F(PerformUpdatesTest, ItHandlesApiExceptionsWhenGettingUpdateDetails)
    {
        ON_CALL(this->mockApi, GetUpdateDetails)
            .WillByDefault(testing::Throw(UKControllerPlugin::Api::ApiException("foo")));

        EXPECT_CALL(this->mockWindows, OpenMessageBox(testing::_, testing::_, MB_OK | MB_ICONSTOP)).Times(1);

        EXPECT_TRUE(CheckForUpdates(mockApi, mockWindows, mockCurl, false));
    }

    TEST_F(PerformUpdatesTest, ItHandlesInvalidApiDataWhenGettingUpdateDetails)
    {
        nlohmann::json apiData{{"foo", "bar"}};

        ON_CALL(this->mockApi, GetUpdateDetails).WillByDefault(testing::Return(apiData));

        EXPECT_CALL(this->mockWindows, OpenMessageBox(testing::_, testing::_, MB_OK | MB_ICONSTOP)).Times(1);

        EXPECT_TRUE(CheckForUpdates(mockApi, mockWindows, mockCurl, false));
    }

    TEST_F(PerformUpdatesTest, ItDoesntDoUpdateIfNoUpdateRequired)
    {
        nlohmann::json apiData{
            {"version", "3.0.0"},
            {"updater_download_url", "foo"},
            {"core_download_url", "bar"},
            {"loader_download_url", "baz"},
        };

        ON_CALL(this->mockApi, GetUpdateDetails).WillByDefault(testing::Return(apiData));

        ON_CALL(this->mockWindows, FileExists(std::wstring(L"bin/UKControllerPluginCore.dll")))
            .WillByDefault(testing::Return(true));

        HINSTANCE handle = reinterpret_cast<HINSTANCE>(123);
        EXPECT_CALL(this->mockWindows, LoadLibraryRelative(std::wstring(L"bin/UKControllerPluginCore.dll")))
            .Times(1)
            .WillOnce(testing::Return(handle));

        EXPECT_CALL(this->mockWindows, GetFunctionPointerFromLibrary(handle, "GetPluginVersion"))
            .Times(1)
            .WillOnce(testing::Return(reinterpret_cast<FARPROC>(VersionFunction))); // NOLINT

        EXPECT_CALL(this->mockWindows, UnloadLibrary(handle)).Times(1);

        EXPECT_CALL(this->mockWindows, MoveFileToNewLocation(testing::_, testing::_)).Times(0);

        EXPECT_TRUE(CheckForUpdates(mockApi, mockWindows, mockCurl, false));
    }

    TEST_F(PerformUpdatesTest, ItDoesntUpdateIfUserDoesntConsent)
    {
        nlohmann::json apiData{
            {"version", "3.0.1"},
            {"updater_download_url", "foo"},
            {"core_download_url", "bar"},
            {"loader_download_url", "baz"},
        };

        ON_CALL(this->mockApi, GetUpdateDetails).WillByDefault(testing::Return(apiData));

        ON_CALL(this->mockWindows, FileExists(std::wstring(L"bin/UKControllerPluginCore.dll")))
            .WillByDefault(testing::Return(false));

        EXPECT_CALL(this->mockWindows, OpenMessageBox(testing::_, testing::_, MB_YESNO | MB_ICONINFORMATION))
            .Times(1)
            .WillOnce(testing::Return(IDNO));

        EXPECT_CALL(this->mockWindows, OpenMessageBox(testing::_, testing::_, MB_YESNO | MB_ICONEXCLAMATION))
            .Times(1)
            .WillOnce(testing::Return(IDNO));

        EXPECT_CALL(this->mockWindows, MoveFileToNewLocation(testing::_, testing::_)).Times(0);

        EXPECT_FALSE(CheckForUpdates(mockApi, mockWindows, mockCurl, false));
    }

    TEST_F(PerformUpdatesTest, ItPerformsUpdatesWithNoChangelog)
    {
        nlohmann::json apiData{
            {"version", "3.0.1"},
            {"updater_download_url", "foo"},
            {"core_download_url", "bar"},
            {"loader_download_url", "baz"},
        };

        ON_CALL(this->mockApi, GetUpdateDetails).WillByDefault(testing::Return(apiData));

        ON_CALL(this->mockWindows, FileExists(std::wstring(L"bin/UKControllerPluginCore.dll")))
            .WillByDefault(testing::Return(false));

        EXPECT_CALL(this->mockWindows, OpenMessageBox(testing::_, testing::_, MB_YESNO | MB_ICONINFORMATION))
            .Times(2)
            .WillOnce(testing::Return(IDYES))
            .WillOnce(testing::Return(IDNO));

        EXPECT_CALL(
            this->mockWindows,
            MoveFileToNewLocation(
                std::wstring(L"bin/UKControllerPluginUpdater.dll"),
                std::wstring(L"bin/UKControllerPluginUpdater.dll.old")))
            .Times(1);

        // Core things
        CurlRequest expectedCoreRequest("bar", CurlRequest::METHOD_GET);
        expectedCoreRequest.SetMaxRequestTime(0);

        CurlResponse coreResponse("3.0.1.core", false, 200);

        EXPECT_CALL(this->mockCurl, MakeCurlRequest(expectedCoreRequest))
            .Times(1)
            .WillOnce(testing::Return(coreResponse));

        EXPECT_CALL(
            this->mockWindows, WriteToFile(std::wstring(L"bin/UKControllerPluginCore.dll"), "3.0.1.core", true, true))
            .Times(1);

        // Updater things
        CurlRequest expectedUpdaterRequest("foo", CurlRequest::METHOD_GET);
        expectedUpdaterRequest.SetMaxRequestTime(0);

        CurlResponse updaterResponse("3.0.1.updater", false, 200);

        EXPECT_CALL(this->mockCurl, MakeCurlRequest(expectedUpdaterRequest))
            .Times(1)
            .WillOnce(testing::Return(updaterResponse));

        EXPECT_CALL(
            this->mockWindows,
            WriteToFile(std::wstring(L"bin/UKControllerPluginUpdater.dll"), "3.0.1.updater", true, true))
            .Times(1);

        // Changelog
        EXPECT_CALL(this->mockWindows, OpenWebBrowser(testing::_)).Times(0);

        EXPECT_TRUE(CheckForUpdates(mockApi, mockWindows, mockCurl, false));
    }

    TEST_F(PerformUpdatesTest, ItPerformsUpdatesWithChangelog)
    {
        nlohmann::json apiData{
            {"version", "3.0.1"},
            {"updater_download_url", "foo"},
            {"core_download_url", "bar"},
            {"loader_download_url", "baz"},
        };

        ON_CALL(this->mockApi, GetUpdateDetails).WillByDefault(testing::Return(apiData));

        ON_CALL(this->mockWindows, FileExists(std::wstring(L"bin/UKControllerPluginCore.dll")))
            .WillByDefault(testing::Return(false));

        EXPECT_CALL(this->mockWindows, OpenMessageBox(testing::_, testing::_, MB_YESNO | MB_ICONINFORMATION))
            .Times(2)
            .WillOnce(testing::Return(IDYES))
            .WillOnce(testing::Return(IDYES));

        EXPECT_CALL(
            this->mockWindows,
            MoveFileToNewLocation(
                std::wstring(L"bin/UKControllerPluginUpdater.dll"),
                std::wstring(L"bin/UKControllerPluginUpdater.dll.old")))
            .Times(1);

        // Core things
        CurlRequest expectedCoreRequest("bar", CurlRequest::METHOD_GET);
        expectedCoreRequest.SetMaxRequestTime(0);

        CurlResponse coreResponse("3.0.1.core", false, 200);

        EXPECT_CALL(this->mockCurl, MakeCurlRequest(expectedCoreRequest))
            .Times(1)
            .WillOnce(testing::Return(coreResponse));

        EXPECT_CALL(
            this->mockWindows, WriteToFile(std::wstring(L"bin/UKControllerPluginCore.dll"), "3.0.1.core", true, true))
            .Times(1);

        // Updater things
        CurlRequest expectedUpdaterRequest("foo", CurlRequest::METHOD_GET);
        expectedUpdaterRequest.SetMaxRequestTime(0);

        CurlResponse updaterResponse("3.0.1.updater", false, 200);

        EXPECT_CALL(this->mockCurl, MakeCurlRequest(expectedUpdaterRequest))
            .Times(1)
            .WillOnce(testing::Return(updaterResponse));

        EXPECT_CALL(
            this->mockWindows,
            WriteToFile(std::wstring(L"bin/UKControllerPluginUpdater.dll"), "3.0.1.updater", true, true))
            .Times(1);

        // View changelog
        EXPECT_CALL(this->mockWindows, OpenWebBrowser(UKControllerPlugin::Update::changelogUrl)).Times(1);

        EXPECT_TRUE(CheckForUpdates(mockApi, mockWindows, mockCurl, false));
    }

    TEST_F(PerformUpdatesTest, ItHandlesFailedUpdaterDownloads)
    {
        nlohmann::json apiData{
            {"version", "3.0.1"},
            {"updater_download_url", "foo"},
            {"core_download_url", "bar"},
            {"loader_download_url", "baz"},
        };

        ON_CALL(this->mockApi, GetUpdateDetails).WillByDefault(testing::Return(apiData));

        ON_CALL(this->mockWindows, FileExists(std::wstring(L"bin/UKControllerPluginCore.dll")))
            .WillByDefault(testing::Return(false));

        EXPECT_CALL(this->mockWindows, OpenMessageBox(testing::_, testing::_, MB_YESNO | MB_ICONINFORMATION))
            .Times(1)
            .WillOnce(testing::Return(IDYES));

        EXPECT_CALL(
            this->mockWindows,
            MoveFileToNewLocation(
                std::wstring(L"bin/UKControllerPluginUpdater.dll"),
                std::wstring(L"bin/UKControllerPluginUpdater.dll.old")))
            .Times(1);

        // Core things
        CurlRequest expectedCoreRequest("bar", CurlRequest::METHOD_GET);
        expectedCoreRequest.SetMaxRequestTime(0);

        CurlResponse coreResponse("3.0.1.core", false, 200);

        EXPECT_CALL(this->mockCurl, MakeCurlRequest(expectedCoreRequest))
            .Times(1)
            .WillOnce(testing::Return(coreResponse));

        EXPECT_CALL(
            this->mockWindows, WriteToFile(std::wstring(L"bin/UKControllerPluginCore.dll"), "3.0.1.core", true, true))
            .Times(1);

        // Updater things
        CurlRequest expectedUpdaterRequest("foo", CurlRequest::METHOD_GET);
        expectedUpdaterRequest.SetMaxRequestTime(0);

        CurlResponse updaterResponse("3.0.1.updater", true, 200);

        EXPECT_CALL(this->mockCurl, MakeCurlRequest(expectedUpdaterRequest))
            .Times(1)
            .WillOnce(testing::Return(updaterResponse));

        EXPECT_CALL(
            this->mockWindows,
            WriteToFile(std::wstring(L"bin/UKControllerPluginUpdater.dll"), "3.0.1.updater", true, true))
            .Times(0);

        // Messagebox
        EXPECT_CALL(this->mockWindows, OpenMessageBox(testing::_, testing::_, MB_OK | MB_ICONSTOP)).Times(1);

        EXPECT_TRUE(CheckForUpdates(mockApi, mockWindows, mockCurl, false));
    }

    TEST_F(PerformUpdatesTest, ItHandlesFailedCoreDownloads)
    {
        nlohmann::json apiData{
            {"version", "3.0.1"},
            {"updater_download_url", "foo"},
            {"core_download_url", "bar"},
            {"loader_download_url", "baz"},
        };

        ON_CALL(this->mockApi, GetUpdateDetails).WillByDefault(testing::Return(apiData));

        ON_CALL(this->mockWindows, FileExists(std::wstring(L"bin/UKControllerPluginCore.dll")))
            .WillByDefault(testing::Return(false));

        EXPECT_CALL(this->mockWindows, OpenMessageBox(testing::_, testing::_, MB_YESNO | MB_ICONINFORMATION))
            .Times(1)
            .WillOnce(testing::Return(IDYES));

        EXPECT_CALL(
            this->mockWindows,
            MoveFileToNewLocation(
                std::wstring(L"bin/UKControllerPluginUpdater.dll"),
                std::wstring(L"bin/UKControllerPluginUpdater.dll.old")))
            .Times(1);

        // Core things
        CurlRequest expectedCoreRequest("bar", CurlRequest::METHOD_GET);
        expectedCoreRequest.SetMaxRequestTime(0);

        CurlResponse coreResponse("3.0.1.core", true, 200);

        EXPECT_CALL(this->mockCurl, MakeCurlRequest(expectedCoreRequest))
            .Times(1)
            .WillOnce(testing::Return(coreResponse));

        EXPECT_CALL(
            this->mockWindows, WriteToFile(std::wstring(L"bin/UKControllerPluginCore.dll"), "3.0.1.core", true, true))
            .Times(0);

        // Updater things
        CurlRequest expectedUpdaterRequest("foo", CurlRequest::METHOD_GET);
        expectedUpdaterRequest.SetMaxRequestTime(0);

        CurlResponse updaterResponse("3.0.1.updater", true, 200);

        EXPECT_CALL(this->mockCurl, MakeCurlRequest(expectedUpdaterRequest)).Times(0);

        EXPECT_CALL(
            this->mockWindows,
            WriteToFile(std::wstring(L"bin/UKControllerPluginUpdater.dll"), "3.0.1.updater", true, true))
            .Times(0);

        // Messagebox
        EXPECT_CALL(this->mockWindows, OpenMessageBox(testing::_, testing::_, MB_OK | MB_ICONSTOP)).Times(1);

        EXPECT_TRUE(CheckForUpdates(mockApi, mockWindows, mockCurl, false));
    }

    TEST_F(PerformUpdatesTest, ItPerformsUpdatesOnSecondPrompt)
    {
        nlohmann::json apiData{
            {"version", "3.0.1"},
            {"updater_download_url", "foo"},
            {"core_download_url", "bar"},
            {"loader_download_url", "baz"},
        };

        ON_CALL(this->mockApi, GetUpdateDetails).WillByDefault(testing::Return(apiData));

        ON_CALL(this->mockWindows, FileExists(std::wstring(L"bin/UKControllerPluginCore.dll")))
            .WillByDefault(testing::Return(false));

        EXPECT_CALL(this->mockWindows, OpenMessageBox(testing::_, testing::_, MB_YESNO | MB_ICONINFORMATION))
            .Times(2)
            .WillOnce(testing::Return(IDNO))
            .WillOnce(testing::Return(IDNO));

        EXPECT_CALL(this->mockWindows, OpenMessageBox(testing::_, testing::_, MB_YESNO | MB_ICONWARNING))
            .Times(1)
            .WillOnce(testing::Return(IDYES));

        EXPECT_CALL(
            this->mockWindows,
            MoveFileToNewLocation(
                std::wstring(L"bin/UKControllerPluginUpdater.dll"),
                std::wstring(L"bin/UKControllerPluginUpdater.dll.old")))
            .Times(1);

        // Core things
        CurlRequest expectedCoreRequest("bar", CurlRequest::METHOD_GET);
        expectedCoreRequest.SetMaxRequestTime(0);

        CurlResponse coreResponse("3.0.1.core", false, 200);

        EXPECT_CALL(this->mockCurl, MakeCurlRequest(expectedCoreRequest))
            .Times(1)
            .WillOnce(testing::Return(coreResponse));

        EXPECT_CALL(
            this->mockWindows, WriteToFile(std::wstring(L"bin/UKControllerPluginCore.dll"), "3.0.1.core", true, true))
            .Times(1);

        // Updater things
        CurlRequest expectedUpdaterRequest("foo", CurlRequest::METHOD_GET);
        expectedUpdaterRequest.SetMaxRequestTime(0);

        CurlResponse updaterResponse("3.0.1.updater", false, 200);

        EXPECT_CALL(this->mockCurl, MakeCurlRequest(expectedUpdaterRequest))
            .Times(1)
            .WillOnce(testing::Return(updaterResponse));

        EXPECT_CALL(
            this->mockWindows,
            WriteToFile(std::wstring(L"bin/UKControllerPluginUpdater.dll"), "3.0.1.updater", true, true))
            .Times(1);

        // Changelog
        EXPECT_CALL(this->mockWindows, OpenWebBrowser(testing::_)).Times(0);

        EXPECT_TRUE(CheckForUpdates(mockApi, mockWindows, mockCurl, false));
    }
} // namespace UKControllerPluginUpdaterTest::Updater
