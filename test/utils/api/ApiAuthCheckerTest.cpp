#include "api/ApiAuthChecker.h"
#include "api/ApiNotAuthorisedException.h"
#include "setting/SettingRepository.h"

using ::testing::_;
using ::testing::NiceMock;
using ::testing::Return;
using ::testing::Test;
using ::testing::Throw;
using UKControllerPlugin::Api::ApiAuthChecker;
using UKControllerPlugin::Api::ApiException;
using UKControllerPlugin::Api::ApiNotAuthorisedException;
using UKControllerPlugin::Setting::SettingRepository;
using UKControllerPluginTest::Api::MockApiInterface;
using UKControllerPluginTest::Windows::MockWinApi;

namespace UKControllerPluginUtilsTest {
    namespace Api {

        class ApiAuthCheckerTest : public Test
        {
            public:
            ApiAuthCheckerTest() : settingRepo(mockWindows)
            {
            }

            NiceMock<MockApiInterface> mockApi;
            NiceMock<MockWinApi> mockWindows;
            SettingRepository settingRepo;
        };

        TEST_F(ApiAuthCheckerTest, IsAuthorisedReturnsFalseIfNotAuthorised)
        {
            EXPECT_CALL(mockWindows, OpenMessageBox(_, testing::StrEq(L"UKCP API Authentication Warning"), _))
                .Times(1)
                .WillOnce(Return(IDCANCEL));

            EXPECT_CALL(mockApi, CheckApiAuthorisation())
                .Times(1)
                .WillOnce(Throw(ApiNotAuthorisedException("not allowed")));

            EXPECT_FALSE(ApiAuthChecker::IsAuthorised(this->mockApi, this->mockWindows, this->settingRepo));
        }

        TEST_F(ApiAuthCheckerTest, IsAuthorisedPromptsForCredentialsOnAuthFailure)
        {
            ON_CALL(this->mockWindows, FileOpenDialog(_, _, _)).WillByDefault(Return(L"testfile.json"));

            nlohmann::json testJson;
            testJson["api-url"] = "testurl";
            testJson["api-key"] = "testkey";

            ON_CALL(this->mockWindows, ReadFromFileMock(std::wstring(L"testfile.json"), false))
                .WillByDefault(Return(testJson.dump()));

            ON_CALL(this->mockWindows, ReadFromFileMock(std::wstring(L"settings/api-settings.json"), true))
                .WillByDefault(Return(testJson.dump()));

            ON_CALL(this->mockWindows, FileExists(std::wstring(L"settings/api-settings.json")))
                .WillByDefault(Return(true));

            EXPECT_CALL(mockWindows, OpenMessageBox(_, testing::StrEq(L"UKCP API Authentication Warning"), _))
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
            ON_CALL(this->mockWindows, FileOpenDialog(_, _, _)).WillByDefault(Return(L"testfile.json"));

            nlohmann::json testJson;
            testJson["api-url"] = "testurl";
            testJson["api-key"] = "testkey";

            ON_CALL(this->mockWindows, ReadFromFileMock(std::wstring(L"testfile.json"), false))
                .WillByDefault(Return(testJson.dump()));

            ON_CALL(this->mockWindows, ReadFromFileMock(std::wstring(L"settings/api-settings.json"), true))
                .WillByDefault(Return(testJson.dump()));

            ON_CALL(this->mockWindows, FileExists(std::wstring(L"settings/api-settings.json")))
                .WillByDefault(Return(true));

            EXPECT_CALL(mockWindows, OpenMessageBox(_, testing::StrEq(L"UKCP API Authentication Warning"), _))
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
            ON_CALL(this->mockWindows, FileOpenDialog(_, _, _)).WillByDefault(Return(L"testfile.json"));

            nlohmann::json testJson;
            testJson["api-url"] = "testurl";
            testJson["api-key"] = "testkey";

            ON_CALL(this->mockWindows, ReadFromFileMock(std::wstring(L"testfile.json"), false))
                .WillByDefault(Return(testJson.dump()));

            ON_CALL(this->mockWindows, ReadFromFileMock(std::wstring(L"settings/api-settings.json"), true))
                .WillByDefault(Return(testJson.dump()));

            ON_CALL(this->mockWindows, FileExists(std::wstring(L"settings/api-settings.json")))
                .WillByDefault(Return(true));

            EXPECT_CALL(mockWindows, OpenMessageBox(_, testing::StrEq(L"UKCP API Authentication Warning"), _))
                .Times(1)
                .WillOnce(Return(IDOK));

            EXPECT_CALL(mockApi, CheckApiAuthorisation())
                .Times(2)
                .WillOnce(Throw(ApiNotAuthorisedException("not allowed")))
                .WillOnce(Return(true));

            EXPECT_CALL(mockApi, SetApiDomain("testurl")).Times(1);

            EXPECT_CALL(mockApi, SetApiKey("testkey")).Times(1);

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
            EXPECT_CALL(mockApi, CheckApiAuthorisation()).Times(1).WillOnce(Return(true));

            EXPECT_TRUE(ApiAuthChecker::IsAuthorised(this->mockApi, this->mockWindows, this->settingRepo));
        }
    } // namespace Api
} // namespace UKControllerPluginUtilsTest
