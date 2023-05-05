#include "api/ApiSettings.h"
#include "api/ConfigApiSettingsProvider.h"

using UKControllerPluginTest::Setting::MockSettingRepository;
using UKControllerPluginTest::Windows::MockWinApi;
using UKControllerPluginUtils::Api::ConfigApiSettingsProvider;

namespace UKControllerPluginUtilsTest::Api {
    class ConfigApiSettingsProviderTest : public testing::Test
    {
        public:
        ConfigApiSettingsProviderTest() : provider(settings)
        {
        }

        testing::NiceMock<MockSettingRepository> settings;
        ConfigApiSettingsProvider provider;
    };

    TEST_F(ConfigApiSettingsProviderTest, ItReturnsSettings)
    {
        ON_CALL(settings, GetSetting("api-url", "https://ukcp.vatsim.uk"))
            .WillByDefault(testing::Return("https://ukcp2.vatsim.uk"));

        ON_CALL(settings, GetSetting("api-key", "")).WillByDefault(testing::Return("key"));

        auto& apiSettings = provider.Get();
        EXPECT_EQ("https://ukcp2.vatsim.uk", apiSettings.Url());
        EXPECT_EQ("key", apiSettings.Key());
    }

    TEST_F(ConfigApiSettingsProviderTest, ItReturnsSameSettingsEachTime)
    {
        ON_CALL(settings, GetSetting("api-url", "https://ukcp.vatsim.uk"))
            .WillByDefault(testing::Return("https://ukcp2.vatsim.uk"));

        ON_CALL(settings, GetSetting("api-key", "")).WillByDefault(testing::Return("key"));

        EXPECT_EQ(&provider.Get(), &provider.Get());
    }

    TEST_F(ConfigApiSettingsProviderTest, ItHasSettingsIfAKeyIsPresent)
    {
        ON_CALL(settings, GetSetting("api-url", "https://ukcp.vatsim.uk"))
            .WillByDefault(testing::Return("https://ukcp2.vatsim.uk"));

        ON_CALL(settings, GetSetting("api-key", "")).WillByDefault(testing::Return("key"));

        EXPECT_TRUE(provider.Has());
    }

    TEST_F(ConfigApiSettingsProviderTest, ItDoesntHaveSettingsIfAKeyIsNotPresent)
    {
        ON_CALL(settings, GetSetting("api-url", "https://ukcp.vatsim.uk"))
            .WillByDefault(testing::Return("https://ukcp2.vatsim.uk"));

        ON_CALL(settings, GetSetting("api-key", "")).WillByDefault(testing::Return(""));

        EXPECT_FALSE(provider.Has());
    }
} // namespace UKControllerPluginUtilsTest::Api
