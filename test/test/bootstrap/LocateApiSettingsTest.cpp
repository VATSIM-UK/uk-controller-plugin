#include "pch/pch.h"
#include "bootstrap/LocateApiSettings.h"
#include "mock/MockWinApi.h"
#include "setting/SettingRepository.h"
#include "helper/Matchers.h"

using testing::NiceMock;
using testing::Return;
using testing::Test;
using testing::_;
using UKControllerPluginTest::Windows::MockWinApi;
using UKControllerPlugin::Setting::SettingRepository;
using UKControllerPlugin::Bootstrap::LocateApiSettings;

namespace UKControllerPluginTest {
namespace Bootstrap {

    class LocateApiSettingsTest : public Test
    {
        public:
            LocateApiSettingsTest()
                : settings(mockWindows)
            {

            }

            SettingRepository settings;
            NiceMock<MockWinApi> mockWindows;
    };

    TEST_F(LocateApiSettingsTest, ItDoesNothingIfSettingsPresent)
    {
        this->settings.AddSettingValue({"api-key", "testkey", "settings/api-settings.json"});
        this->settings.AddSettingValue({"api-url", "testurl", "settings/api-settings.json"});
        EXPECT_NO_THROW(LocateApiSettings(this->mockWindows, this->settings));
    }

    TEST_F(LocateApiSettingsTest, ItPromptsForFileLoadsAndSavesSettings)
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

        EXPECT_TRUE("testurl" == this->settings.GetSetting("api-url"));
        EXPECT_TRUE("testkey" == this->settings.GetSetting("api-key"));
    }
}  // namespace Bootstrap
}  // namespace UKControllerPluginTest
