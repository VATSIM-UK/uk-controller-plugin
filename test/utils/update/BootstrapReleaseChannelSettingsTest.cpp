#include "setting/SettingRepository.h"
#include "update/BootstrapReleaseChannelSettings.h"

using UKControllerPlugin::Setting::SettingRepository;
using UKControllerPluginTest::Windows::MockWinApi;
using UKControllerPluginUtils::Update::BootstrapReleaseChannelSettings;

namespace UKControllerPluginUtilsTest::Update {
    class BootstrapReleaseChannelSettingsTest : public testing::Test
    {
        public:
        testing::NiceMock<MockWinApi> windows;
        SettingRepository settings;
    };

    TEST_F(BootstrapReleaseChannelSettingsTest, ItBootstrapsTheSettings)
    {
        BootstrapReleaseChannelSettings(settings, windows);
        EXPECT_EQ(1, settings.CountSettings());
        EXPECT_TRUE(settings.HasSetting("release_channel"));
    }
} // namespace UKControllerPluginUtilsTest::Update
