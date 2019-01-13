#include "pch/pch.h"
#include "setting/SettingRepositoryFactory.h"
#include "setting/SettingRepository.h"
#include "mock/MockWinApi.h"

using UKControllerPlugin::Setting::SettingRepository;
using UKControllerPlugin::Setting::SettingRepositoryFactory;
using UKControllerPluginTest::Windows::MockWinApi;
using ::testing::StrictMock;
using ::testing::Return;

namespace UKControllerPluginTest {
    namespace Setting {

        TEST(SettingRepositoryFactory, CreateLoadsApiConfigSettings)
        {
            StrictMock<MockWinApi> mockWinApi;

            EXPECT_CALL(mockWinApi, FileExists("settings/api-settings.json"))
                .Times(1)
                .WillOnce(Return(true));

            EXPECT_CALL(mockWinApi, ReadFromFileMock("settings/api-settings.json", true))
                .Times(1)
                .WillOnce(Return("{\"api-key\": \"testkey\"}"));

            SettingRepository repo = SettingRepositoryFactory::Create(mockWinApi);
            EXPECT_TRUE("testkey" == repo.GetSetting("api-key"));
        }
    }  // namespace Setting
}  // namespace UKControllerPluginTest
