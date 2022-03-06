#include "api/ApiSettings.h"
#include "api/ConfigApiSettingsProvider.h"

using UKControllerPluginTest::Setting::MockSettingRepository;
using UKControllerPluginTest::Windows::MockWinApi;
using UKControllerPluginUtils::Api::ConfigApiSettingsProvider;

namespace UKControllerPluginUtilsTest::Api {
    class ConfigApiSettingsProviderTest : public testing::Test
    {
        public:
        ConfigApiSettingsProviderTest() : provider(settings, windows)
        {
        }

        testing::NiceMock<MockSettingRepository> settings;
        testing::NiceMock<MockWinApi> windows;
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

    TEST_F(ConfigApiSettingsProviderTest, ItDoesntReloadSettingsIfUserDoesntSelectFile)
    {
        ON_CALL(settings, GetSetting("api-url", "https://ukcp.vatsim.uk"))
            .WillByDefault(testing::Return("https://ukcp.vatsim.uk"));

        ON_CALL(settings, GetSetting("api-key", "")).WillByDefault(testing::Return("key"));

        ON_CALL(windows, FileOpenDialog(std::wstring(L"Select API Settings File"), 1, testing::_))
            .WillByDefault(testing::Return(L""));

        EXPECT_CALL(windows, WriteToFile).Times(0);

        EXPECT_FALSE(provider.Reload());
        auto& apiSettings = provider.Get();
        EXPECT_EQ("https://ukcp.vatsim.uk", apiSettings.Url());
        EXPECT_EQ("key", apiSettings.Key());
    }

    TEST_F(ConfigApiSettingsProviderTest, ItReloadsSettings)
    {
        ON_CALL(settings, GetSetting("api-url", "https://ukcp.vatsim.uk"))
            .WillByDefault(testing::Return("https://ukcp2.vatsim.uk"));

        ON_CALL(settings, GetSetting("api-key", "")).WillByDefault(testing::Return("key2"));

        ON_CALL(windows, FileOpenDialog(std::wstring(L"Select API Settings File"), 1, testing::_))
            .WillByDefault(testing::Return(L"foo/bar.json"));

        EXPECT_CALL(windows, ReadFromFileMock(std::wstring(L"foo/bar.json"), false))
            .Times(1)
            .WillOnce(testing::Return("foo"));

        EXPECT_CALL(windows, WriteToFile(std::wstring(L"settings/api-settings.json"), "foo", true, false)).Times(1);

        EXPECT_CALL(settings, ReloadSetting("api-key")).Times(1);

        EXPECT_CALL(settings, ReloadSetting("api-url")).Times(1);

        EXPECT_TRUE(provider.Reload());
        auto& apiSettings = provider.Get();
        EXPECT_EQ("https://ukcp2.vatsim.uk", apiSettings.Url());
        EXPECT_EQ("key2", apiSettings.Key());
    }
} // namespace UKControllerPluginUtilsTest::Api
