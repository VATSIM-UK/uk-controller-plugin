#include "pch/pch.h"
#include "api/ApiAuthChecker.h"
#include "mock/MockWinApi.h"
#include "mock/MockApiInterface.h"
#include "api/ApiNotAuthorisedException.h"
#include "api/ApiException.h"
#include "setting/SettingRepository.h"

using UKControllerPlugin::Api::ApiAuthChecker;
using UKControllerPluginTest::Windows::MockWinApi;
using UKControllerPluginTest::Api::MockApiInterface;
using UKControllerPlugin::Api::ApiNotAuthorisedException;
using UKControllerPlugin::Api::ApiException;
using UKControllerPlugin::Setting::SettingRepository;
using ::testing::Test;
using ::testing::NiceMock;
using ::testing::Throw;
using ::testing::Return;
using ::testing::_;

namespace UKControllerPluginUtilsTest {
    namespace Api {

        class ApiAuthCheckerTest : public Test
        {
            public:
                ApiAuthCheckerTest()
                    : settingRepo(mockWindows)
                {

                }

                NiceMock<MockApiInterface> mockApi;
                NiceMock<MockWinApi> mockWindows;
                SettingRepository settingRepo;
        };

        TEST_F(ApiAuthCheckerTest, IsAuthorisedReturnsFalseIfNotAuthorised)
        {
            EXPECT_CALL(mockWindows, OpenMessageBox(_, L"UKCP API Authentication Warning", _))
                .Times(1)
                .WillOnce(Return(IDCANCEL));

            EXPECT_CALL(mockApi, CheckApiAuthorisation())
                .Times(1)
                .WillOnce(Throw(ApiNotAuthorisedException("not allowed")));

            EXPECT_FALSE(ApiAuthChecker::IsAuthorised(this->mockApi, this->mockWindows, this->settingRepo));
        }

        TEST_F(ApiAuthCheckerTest, IsAuthorisedPromptsForCredentialsOnAuthFailure)
        {
            ON_CALL(this->mockWindows, FileOpenDialog(_, _, _))
                .WillByDefault(Return(L"testfile.json"));

            nlohmann::json testJson;
            testJson["api-url"] = "testurl";
            testJson["api-key"] = "testkey";

            ON_CALL(this->mockWindows, ReadFromFileMock(std::wstring(L"testfile.json"), false))
                .WillByDefault(Return(testJson.dump()));

            ON_CALL(this->mockWindows, ReadFromFileMock(std::wstring(L"settings/api-settings.json"), true))
                .WillByDefault(Return(testJson.dump()));

            ON_CALL(this->mockWindows, FileExists(std::wstring(L"settings/api-settings.json")))
                .WillByDefault(Return(true));

            EXPECT_CALL(mockWindows, OpenMessageBox(_, L"UKCP API Authentication Warning", _))
                .Times(1)
                .WillOnce(Return(IDOK));

            EXPECT_CALL(mockApi, CheckApiAuthorisation())
                .Times(2)
                .WillOnce(Throw(ApiNotAuthorisedException("not allowed")))
                .WillOnce(Return(true));

            EXPECT_TRUE(ApiAuthChecker::IsAuthorised(this->mockApi, this->mockWindows, this->settingRepo));
        }

        TEST_F(ApiAuthCheckerTest, IsAuthorisedReturnsLastActionOnAuthFailure)
        {
            ON_CALL(this->mockWindows, FileOpenDialog(_, _, _))
                .WillByDefault(Return(L"testfile.json"));

            nlohmann::json testJson;
            testJson["api-url"] = "testurl";
            testJson["api-key"] = "testkey";

            ON_CALL(this->mockWindows, ReadFromFileMock(std::wstring(L"testfile.json"), false))
                .WillByDefault(Return(testJson.dump()));

            ON_CALL(this->mockWindows, ReadFromFileMock(std::wstring(L"settings/api-settings.json"), true))
                .WillByDefault(Return(testJson.dump()));

            ON_CALL(this->mockWindows, FileExists(std::wstring(L"settings/api-settings.json")))
                .WillByDefault(Return(true));

            EXPECT_CALL(mockWindows, OpenMessageBox(_, L"UKCP API Authentication Warning", _))
                .Times(2)
                .WillOnce(Return(IDOK))
                .WillOnce(Return(IDCANCEL));

            EXPECT_CALL(mockApi, CheckApiAuthorisation())
                .Times(2)
                .WillRepeatedly(Throw(ApiNotAuthorisedException("not allowed")));

            EXPECT_FALSE(ApiAuthChecker::IsAuthorised(this->mockApi, this->mockWindows, this->settingRepo));
        }

        TEST_F(ApiAuthCheckerTest, IsAuthorisedUsesPromptedCredentialsOnTheApi)
        {
            ON_CALL(this->mockWindows, FileOpenDialog(_, _, _))
                .WillByDefault(Return(L"testfile.json"));

            nlohmann::json testJson;
            testJson["api-url"] = "testurl";
            testJson["api-key"] = "testkey";

            ON_CALL(this->mockWindows, ReadFromFileMock(std::wstring(L"testfile.json"), false))
                .WillByDefault(Return(testJson.dump()));

            ON_CALL(this->mockWindows, ReadFromFileMock(std::wstring(L"settings/api-settings.json"), true))
                .WillByDefault(Return(testJson.dump()));

            ON_CALL(this->mockWindows, FileExists(std::wstring(L"settings/api-settings.json")))
                .WillByDefault(Return(true));

            EXPECT_CALL(mockWindows, OpenMessageBox(_, L"UKCP API Authentication Warning", _))
                .Times(1)
                .WillOnce(Return(IDOK));

            EXPECT_CALL(mockApi, CheckApiAuthorisation())
                .Times(2)
                .WillOnce(Throw(ApiNotAuthorisedException("not allowed")))
                .WillOnce(Return(true));

            EXPECT_CALL(mockApi, SetApiDomain("testurl"))
                .Times(1);

            EXPECT_CALL(mockApi, SetApiKey("testkey"))
                .Times(1);

            ApiAuthChecker::IsAuthorised(this->mockApi, this->mockWindows, this->settingRepo);
        }

        TEST_F(ApiAuthCheckerTest, IsAuthorisedReturnsFalseIfApiFails)
        {
            EXPECT_CALL(mockApi, CheckApiAuthorisation())
                .Times(1)
                .WillOnce(Throw(ApiException("something went wrong")));

            EXPECT_FALSE(ApiAuthChecker::IsAuthorised(this->mockApi, this->mockWindows, this->settingRepo));
        }

        TEST_F(ApiAuthCheckerTest, IsAuthorisedReturnsTrueIfCheckOk)
        {
            EXPECT_CALL(mockApi, CheckApiAuthorisation())
                .Times(1)
                .WillOnce(Return(true));

            EXPECT_TRUE(ApiAuthChecker::IsAuthorised(this->mockApi, this->mockWindows, this->settingRepo));
        }
    }  // namespace Api
}  // namespace UKControllerPluginUtilsTest
