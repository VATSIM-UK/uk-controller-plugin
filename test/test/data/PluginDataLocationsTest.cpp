#include "pch/updatertestpch.h"
#include "api/ApiException.h"
#include "data/PluginDataLocations.h"
#include "mock/MockWinApi.h"
#include "mock/MockApiInterface.h"
#include "mock/MockCurlApi.h"
#include "curl/CurlRequest.h"
#include "curl/CurlResponse.h"
#include "update/LoadChangelog.h"

using testing::Test;
using testing::NiceMock;
using UKControllerPluginTest::Windows::MockWinApi;
using UKControllerPlugin::Curl::CurlRequest;
using UKControllerPlugin::Curl::CurlResponse;

namespace UKControllerPluginUtilsTest {
    namespace Data {
        class PluginDataLocationsTest : public Test
        {
            public:

                std::wstring GetExpectedUkcpFolder(void)
                {
                    TCHAR* folderPath = nullptr;
                    HRESULT result = SHGetKnownFolderPath(FOLDERID_LocalAppData, KF_FLAG_SIMPLE_IDLIST, nullptr,
                                                          &folderPath);

                    std::wstring widePath(folderPath);
                    std::replace(widePath.begin(), widePath.end(), L'\\', L'/');
                    CoTaskMemFree(folderPath);

                    return widePath + L"/UKControllerPlugin";
                }

                NiceMock<MockWinApi> mockWindows;
        };

        TEST_F(PluginDataLocationsTest, ItHasAVersionLockfileLocation)
        {
            EXPECT_EQ(L"version.lock", GetVersionLockfileLocation());
        }

        TEST_F(PluginDataLocationsTest, ItHasABinaryFolder)
        {
            EXPECT_EQ(L"bin", GetBinariesFolderRelativePath());
        }

        TEST_F(PluginDataLocationsTest, TheUpdaterBinaryHasARelativePath)
        {
            EXPECT_EQ(L"bin/UKControllerPluginUpdater.dll", GetUpdaterBinaryRelativePath());
        }

        TEST_F(PluginDataLocationsTest, TheOldUpdaterBinaryHasARelativePath)
        {
            EXPECT_EQ(L"bin/UKControllerPluginUpdater.dll.old", GetOldUpdaterBinaryRelativePath());
        }

        TEST_F(PluginDataLocationsTest, TheCoreBinaryHasARelativePath)
        {
            EXPECT_EQ(L"bin/UKControllerPluginCore.dll", GetCoreBinaryRelativePath());
        }

        TEST_F(PluginDataLocationsTest, ItHasAPluginDataRoot)
        {
            EXPECT_EQ(GetExpectedUkcpFolder(), GetFullPluginDataRoot());
        }

        TEST_F(PluginDataLocationsTest, SetupUkcpFolderRootThrowsExceptionOnFailedCreatingUkcpFolder)
        {
            ON_CALL(this->mockWindows, CreateFolderRecursive(this->GetExpectedUkcpFolder()))
                .WillByDefault(testing::Return(false));


            EXPECT_THROW(CreatePluginDataRoot(this->mockWindows), std::runtime_error);
        }

        TEST_F(PluginDataLocationsTest, SetupUkcpFolderRootCreatesFolders)
        {
            ON_CALL(this->mockWindows, SetPermissions(this->GetExpectedUkcpFolder(), std::filesystem::perms::all))
                .WillByDefault(testing::Return(true));

            EXPECT_CALL(this->mockWindows, CreateFolderRecursive(this->GetExpectedUkcpFolder()))
                .Times(1)
                .WillOnce(testing::Return(true));

            EXPECT_NO_THROW(CreatePluginDataRoot(this->mockWindows));
        }

        TEST_F(PluginDataLocationsTest, SetupUkcpFolderRootSetsPermissions)
        {
            EXPECT_CALL(this->mockWindows, CreateFolderRecursive(this->GetExpectedUkcpFolder()))
                .WillRepeatedly(testing::Return(true));

            EXPECT_CALL(
                this->mockWindows,
                SetPermissions(this->GetExpectedUkcpFolder(), std::filesystem::perms::all)
            )
                .Times(1)
                .WillOnce(testing::Return(true));

            EXPECT_NO_THROW(CreatePluginDataRoot(this->mockWindows));
        }

        TEST_F(PluginDataLocationsTest, SetupUkcpFolderRootThrowsExceptionOnFailedSettingFolderPermissions)
        {
            ON_CALL(this->mockWindows, CreateFolderRecursive(testing::_))
                .WillByDefault(testing::Return(true));

            ON_CALL(this->mockWindows, SetPermissions(this->GetExpectedUkcpFolder(), std::filesystem::perms::all))
                .WillByDefault(testing::Return(false));

            EXPECT_THROW(CreatePluginDataRoot(this->mockWindows), std::runtime_error);
        }
    } // namespace Data
} // namespace UKControllerPluginUtilsTest
