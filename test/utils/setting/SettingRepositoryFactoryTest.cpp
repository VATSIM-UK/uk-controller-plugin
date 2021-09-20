#include "setting/SettingRepositoryFactory.h"
#include "setting/SettingRepository.h"

using ::testing::Return;
using ::testing::StrictMock;
using UKControllerPlugin::Setting::SettingRepository;
using UKControllerPlugin::Setting::SettingRepositoryFactory;
using UKControllerPluginTest::Windows::MockWinApi;

namespace UKControllerPluginUtilsTest {
    namespace Setting {

        TEST(SettingRepositoryFactory, CreateLoadsApiConfigSettings)
        {
            StrictMock<MockWinApi> mockWinApi;

            EXPECT_CALL(mockWinApi, FileExists(std::wstring(L"settings/api-settings.json")))
                .Times(1)
                .WillOnce(Return(true));

            EXPECT_CALL(mockWinApi, ReadFromFileMock(std::wstring(L"settings/api-settings.json"), true))
                .Times(1)
                .WillOnce(Return("{\"api-key\": \"testkey\"}"));

            std::unique_ptr<SettingRepository> repo = SettingRepositoryFactory::Create(mockWinApi);
            EXPECT_TRUE("testkey" == repo->GetSetting("api-key"));
        }
    } // namespace Setting
} // namespace UKControllerPluginUtilsTest
