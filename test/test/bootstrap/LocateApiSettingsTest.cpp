#include "pch/pch.h"
#include "bootstrap/LocateApiSettings.h"
#include "mock/MockWinApi.h"
#include "setting/SettingRepository.h"
#include "setting/SettingValue.h"
#include "helper/Matchers.h"

using testing::NiceMock;
using testing::Return;
using testing::Test;
using testing::_;
using UKControllerPluginTest::Windows::MockWinApi;
using UKControllerPlugin::Setting::SettingRepository;
using UKControllerPlugin::Setting::SettingValue;
using UKControllerPlugin::Bootstrap::LocateApiSettings;
using UKControllerPlugin::Bootstrap::ReplaceApiSettings;
using UKControllerPlugin::Bootstrap::UserRequestedKeyUpdate;
using UKControllerPlugin::Bootstrap::UserRequestedKeyUpdateNoPrompts;

namespace UKControllerPluginTest {
namespace Bootstrap {

    class LocateApiSettingsTest : public Test
    {
        public:
            LocateApiSettingsTest()
                : settings(mockWindows)
            {

            }

            NiceMock<MockWinApi> mockWindows;
            SettingRepository settings;
    };

    TEST_F(LocateApiSettingsTest, LocateApiSettingsDoesNothingIfSettingsPresent)
    {
        this->settings.AddSettingValue({"api-key", "testkey", "settings/api-settings.json"});
        this->settings.AddSettingValue({"api-url", "testurl", "settings/api-settings.json"});
        EXPECT_NO_THROW(LocateApiSettings(this->mockWindows, this->settings));
    }

    TEST_F(LocateApiSettingsTest, LocateApiSettingsPromptsForFileAndWritesToDisk)
    {
        ON_CALL(this->mockWindows, FileOpenDialog(_, _, _))
            .WillByDefault(Return(L"testfile.json"));

        nlohmann::json testJson;
        testJson["api-url"] = "testurl";
        testJson["api-key"] = "testkey";

        EXPECT_CALL(this->mockWindows, ReadFromFileMock(std::wstring(L"testfile.json"), false))
            .Times(1)
            .WillOnce(Return(testJson.dump()));

        ON_CALL(this->mockWindows, ReadFromFileMock("settings/api-settings.json", true))
            .WillByDefault(Return(testJson.dump()));

        ON_CALL(this->mockWindows, FileExists("settings/api-settings.json"))
            .WillByDefault(Return(true));

        EXPECT_CALL(this->mockWindows, WriteToFile("settings/api-settings.json", testJson.dump(), true))
            .Times(1);

        LocateApiSettings(this->mockWindows, this->settings);
    }

    TEST_F(LocateApiSettingsTest, LocateApiSettingsDoesntWriteToSettingsRepoIfFileSelectCancelled)
    {
        ON_CALL(this->mockWindows, FileOpenDialog(_, _, _))
            .WillByDefault(Return(L""));

        LocateApiSettings(this->mockWindows, this->settings);

        EXPECT_TRUE("" == this->settings.GetSetting("api-url"));
        EXPECT_TRUE("" == this->settings.GetSetting("api-key"));
    }

    TEST_F(LocateApiSettingsTest, LocateApiSettingsSavesToSettingsRepository)
    {
        ON_CALL(this->mockWindows, FileOpenDialog(_, _, _))
            .WillByDefault(Return(L"testfile.json"));

        nlohmann::json testJson;
        testJson["api-url"] = "testurl";
        testJson["api-key"] = "testkey";

        ON_CALL(this->mockWindows, ReadFromFileMock(std::wstring(L"testfile.json"), false))
            .WillByDefault(Return(testJson.dump()));

        ON_CALL(this->mockWindows, ReadFromFileMock("settings/api-settings.json", true))
            .WillByDefault(Return(testJson.dump()));

        ON_CALL(this->mockWindows, FileExists("settings/api-settings.json"))
            .WillByDefault(Return(true));

        LocateApiSettings(this->mockWindows, this->settings);

        EXPECT_TRUE("testurl" == this->settings.GetSetting("api-url"));
        EXPECT_TRUE("testkey" == this->settings.GetSetting("api-key"));
    }

    TEST_F(LocateApiSettingsTest, ReplaceApiSettingsPromptsForFileLoadsAndSavesSettingsToDisk)
    {
        ON_CALL(this->mockWindows, FileOpenDialog(_, _, _))
            .WillByDefault(Return(L"testfile.json"));

        nlohmann::json testJson;
        testJson["api-url"] = "testurl";
        testJson["api-key"] = "testkey";

        EXPECT_CALL(this->mockWindows, ReadFromFileMock(std::wstring(L"testfile.json"), false))
            .Times(1)
            .WillOnce(Return(testJson.dump()));

        ON_CALL(this->mockWindows, ReadFromFileMock("settings/api-settings.json", true))
            .WillByDefault(Return(testJson.dump()));

        ON_CALL(this->mockWindows, FileExists("settings/api-settings.json"))
            .WillByDefault(Return(true));

        EXPECT_CALL(this->mockWindows, WriteToFile("settings/api-settings.json", testJson.dump(), true))
            .Times(1);

        ReplaceApiSettings(this->mockWindows);
    }

    TEST_F(LocateApiSettingsTest, ReplaceApiSettingsReturnsTrueOnSelectedFile)
    {
        ON_CALL(this->mockWindows, FileOpenDialog(_, _, _))
            .WillByDefault(Return(L"testfile.json"));

        nlohmann::json testJson;
        testJson["api-url"] = "testurl";
        testJson["api-key"] = "testkey";

        EXPECT_CALL(this->mockWindows, ReadFromFileMock(std::wstring(L"testfile.json"), false))
            .Times(1)
            .WillOnce(Return(testJson.dump()));

        ON_CALL(this->mockWindows, ReadFromFileMock("settings/api-settings.json", true))
            .WillByDefault(Return(testJson.dump()));

        ON_CALL(this->mockWindows, FileExists("settings/api-settings.json"))
            .WillByDefault(Return(true));

        EXPECT_TRUE(ReplaceApiSettings(this->mockWindows));
    }

    TEST_F(LocateApiSettingsTest, ReplaceApiSettingsDoesNothingIfCancelled)
    {
        ON_CALL(this->mockWindows, FileOpenDialog(_, _, _))
            .WillByDefault(Return(L""));

        EXPECT_CALL(this->mockWindows, WriteToFile("settings/api-settings.json", _, true))
            .Times(0);

        ReplaceApiSettings(this->mockWindows);
    }

    TEST_F(LocateApiSettingsTest, ReplaceApiSettingsReturnsFalseOnCancelled)
    {
        ON_CALL(this->mockWindows, FileOpenDialog(_, _, _))
            .WillByDefault(Return(L""));

        EXPECT_FALSE(ReplaceApiSettings(this->mockWindows));
    }

    TEST_F(LocateApiSettingsTest, UserRequestedKeyUpdatePromptsForFileLoadsAndSavesSettingsToDisk)
    {
        ON_CALL(this->mockWindows, FileOpenDialog(_, _, _))
            .WillByDefault(Return(L"testfile.json"));

        ON_CALL(
            this->mockWindows,
            OpenMessageBox(
                L"Please select the key file to use, this will overwrite your previous key.",
                L"UKCP Message",
                MB_OKCANCEL | MB_ICONINFORMATION
            )
        )
            .WillByDefault(Return(IDOK));

        nlohmann::json testJson;
        testJson["api-url"] = "testurl";
        testJson["api-key"] = "testkey";

        EXPECT_CALL(this->mockWindows, ReadFromFileMock(std::wstring(L"testfile.json"), false))
            .Times(1)
            .WillOnce(Return(testJson.dump()));

        ON_CALL(this->mockWindows, ReadFromFileMock("settings/api-settings.json", true))
            .WillByDefault(Return(testJson.dump()));

        ON_CALL(this->mockWindows, FileExists("settings/api-settings.json"))
            .WillByDefault(Return(true));

        EXPECT_CALL(this->mockWindows, WriteToFile("settings/api-settings.json", testJson.dump(), true))
            .Times(1);

        UserRequestedKeyUpdate(this->mockWindows);
    }

    TEST_F(LocateApiSettingsTest, UserRequestedKeyUpdateDoesNothingIfPromptCancelled)
    {
        EXPECT_CALL(this->mockWindows, FileOpenDialog(_, _, _))
            .Times(0);

        EXPECT_CALL(
            this->mockWindows,
            OpenMessageBox(
                L"Please select the key file to use, this will overwrite your previous key.",
                L"UKCP Message",
                MB_OKCANCEL | MB_ICONINFORMATION
            )
        )
            .Times(1)
            .WillOnce(Return(IDCANCEL));

        UserRequestedKeyUpdate(this->mockWindows);
    }

    TEST_F(LocateApiSettingsTest, UserRequestedKeyUpdateNoPromptsPromptsForFileLoadsAndSavesSettingsToDisk)
    {
        ON_CALL(this->mockWindows, FileOpenDialog(_, _, _))
            .WillByDefault(Return(L"testfile.json"));

        ON_CALL(
            this->mockWindows,
            OpenMessageBox(
            L"Please select the key file to use, this will overwrite your previous key.",
            L"UKCP Message",
            MB_OKCANCEL | MB_ICONINFORMATION
        )
        )
            .WillByDefault(Return(IDOK));

        nlohmann::json testJson;
        testJson["api-url"] = "testurl";
        testJson["api-key"] = "testkey";

        EXPECT_CALL(this->mockWindows, ReadFromFileMock(std::wstring(L"testfile.json"), false))
            .Times(1)
            .WillOnce(Return(testJson.dump()));

        ON_CALL(this->mockWindows, ReadFromFileMock("settings/api-settings.json", true))
            .WillByDefault(Return(testJson.dump()));

        ON_CALL(this->mockWindows, FileExists("settings/api-settings.json"))
            .WillByDefault(Return(true));

        EXPECT_CALL(this->mockWindows, WriteToFile("settings/api-settings.json", testJson.dump(), true))
            .Times(1);

        UserRequestedKeyUpdateNoPrompts(this->mockWindows, this->settings);
    }

    TEST_F(LocateApiSettingsTest, UserRequestedKeyUpdateNoPromptsUpdatesSettingsRepo)
    {
        ON_CALL(this->mockWindows, FileOpenDialog(_, _, _))
            .WillByDefault(Return(L"testfile.json"));

        nlohmann::json testJson;
        testJson["api-url"] = "testurl2";
        testJson["api-key"] = "testkey2";

        EXPECT_CALL(this->mockWindows, ReadFromFileMock(std::wstring(L"testfile.json"), false))
            .Times(1)
            .WillOnce(Return(testJson.dump()));

        ON_CALL(this->mockWindows, ReadFromFileMock("settings/api-settings.json", true))
            .WillByDefault(Return(testJson.dump()));

        ON_CALL(this->mockWindows, FileExists("settings/api-settings.json"))
            .WillByDefault(Return(true));

        UserRequestedKeyUpdateNoPrompts(this->mockWindows, this->settings);

        EXPECT_EQ("testurl2", this->settings.GetSetting("api-url"));
        EXPECT_EQ("testkey2", this->settings.GetSetting("api-key"));
    }

    TEST_F(LocateApiSettingsTest, UserRequestedKeyUpdateNoPromptsOverwritesSettingsRepo)
    {
        ON_CALL(this->mockWindows, FileOpenDialog(_, _, _))
            .WillByDefault(Return(L"testfile.json"));

        nlohmann::json testJson;
        testJson["api-url"] = "testurl2";
        testJson["api-key"] = "testkey2";

        EXPECT_CALL(this->mockWindows, ReadFromFileMock(std::wstring(L"testfile.json"), false))
            .Times(1)
            .WillOnce(Return(testJson.dump()));

        ON_CALL(this->mockWindows, ReadFromFileMock("settings/api-settings.json", true))
            .WillByDefault(Return(testJson.dump()));

        ON_CALL(this->mockWindows, FileExists("settings/api-settings.json"))
            .WillByDefault(Return(true));

        // Set settings
        SettingValue apiKeySetting{"api-key", "testkey"};
        SettingValue apiUrlSetting{"api-url", "testurl"};
        this->settings.AddSettingValue(apiKeySetting);
        this->settings.AddSettingValue(apiUrlSetting);

        UserRequestedKeyUpdateNoPrompts(this->mockWindows, this->settings);

        EXPECT_EQ("testurl2", this->settings.GetSetting("api-url"));
        EXPECT_EQ("testkey2", this->settings.GetSetting("api-key"));
    }

    TEST_F(LocateApiSettingsTest, UserRequestedKeyUpdateNoPromptsDoesNothingOnUserCancel)
    {
        ON_CALL(this->mockWindows, FileOpenDialog(_, _, _))
            .WillByDefault(Return(L""));

        UserRequestedKeyUpdateNoPrompts(this->mockWindows, this->settings);

        EXPECT_FALSE(this->settings.HasSetting("api-url"));
        EXPECT_FALSE(this->settings.HasSetting("api-key"));
    }
}  // namespace Bootstrap
}  // namespace UKControllerPluginTest
