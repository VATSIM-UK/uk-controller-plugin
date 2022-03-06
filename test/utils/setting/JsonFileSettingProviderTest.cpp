#include "setting/JsonFileSettingProvider.h"

using UKControllerPlugin::Setting::JsonFileSettingProvider;

namespace UKControllerPluginTest::Setting {
    class JsonFileSettingProviderTest : public testing::Test
    {
        public:
        JsonFileSettingProviderTest()
        {
            ON_CALL(windows, FileExists(std::wstring(L"settings/setting-file.json")))
                .WillByDefault(testing::Return(true));

            nlohmann::json settings{
                {"setting1", "value1"},
                {"setting2", "value2"},
            };

            ON_CALL(windows, ReadFromFileMock(std::wstring(L"settings/setting-file.json"), true))
                .WillByDefault(testing::Return(settings.dump()));
        }

        [[nodiscard]] auto GetProvider() -> JsonFileSettingProvider
        {
            return {L"setting-file.json", std::set<std::string>{"setting1", "setting2"}, windows};
        }

        testing::NiceMock<Windows::MockWinApi> windows;
    };

    TEST_F(JsonFileSettingProviderTest, ItReturnsSettingValues)
    {
        auto provider = GetProvider();
        EXPECT_EQ("value1", provider.Get("setting1"));
        EXPECT_EQ("value2", provider.Get("setting2"));
    }

    TEST_F(JsonFileSettingProviderTest, ItReturnsEmptyNoSetting)
    {
        EXPECT_TRUE(GetProvider().Get("setting3").empty());
    }

    TEST_F(JsonFileSettingProviderTest, ItProvidesSettingValues)
    {
        EXPECT_EQ(std::set<std::string>({"setting1", "setting2"}), GetProvider().Provides());
    }

    TEST_F(JsonFileSettingProviderTest, ItWritesSettings)
    {
        nlohmann::json updatedSettings{{"setting2", "value2a"}, {"setting1", "value1"}};

        EXPECT_CALL(
            windows, WriteToFile(std::wstring(L"settings/setting-file.json"), updatedSettings.dump(), true, false))
            .Times(1);

        auto provider = GetProvider();
        provider.Save("setting2", "value2a");
        EXPECT_EQ("value2a", provider.Get("setting2"));
    }

    TEST_F(JsonFileSettingProviderTest, ItReloadsSettingValues)
    {
        auto provider = GetProvider();
        EXPECT_EQ("value1", provider.Get("setting1"));
        EXPECT_EQ("value2", provider.Get("setting2"));

        nlohmann::json settings{
            {"setting1", "value1a"},
            {"setting2", "value2a"},
        };

        ON_CALL(windows, ReadFromFileMock(std::wstring(L"settings/setting-file.json"), true))
            .WillByDefault(testing::Return(settings.dump()));

        provider.Reload();
        EXPECT_EQ("value1a", provider.Get("setting1"));
        EXPECT_EQ("value2a", provider.Get("setting2"));
    }

    TEST_F(JsonFileSettingProviderTest, ItLoadsNoSettingsIfSettingFileDoesNotExist)
    {
        ON_CALL(windows, FileExists(std::wstring(L"settings/setting-file.json"))).WillByDefault(testing::Return(false));
        auto provider = GetProvider();
        EXPECT_TRUE(provider.Get("setting1").empty());
        EXPECT_TRUE(provider.Get("setting2").empty());
    }

    TEST_F(JsonFileSettingProviderTest, ItLoadsNoSettingsIfFileIsNotValidJson)
    {
        ON_CALL(windows, ReadFromFileMock(std::wstring(L"settings/setting-file.json"), true))
            .WillByDefault(testing::Return("{]"));
        auto provider = GetProvider();
        EXPECT_TRUE(provider.Get("setting1").empty());
        EXPECT_TRUE(provider.Get("setting2").empty());
    }

    TEST_F(JsonFileSettingProviderTest, ItLoadsNoSettingsIfJsonIsNotObject)
    {
        ON_CALL(windows, ReadFromFileMock(std::wstring(L"settings/setting-file.json"), true))
            .WillByDefault(testing::Return("[]"));
        auto provider = GetProvider();
        EXPECT_TRUE(provider.Get("setting1").empty());
        EXPECT_TRUE(provider.Get("setting2").empty());
    }

    TEST_F(JsonFileSettingProviderTest, ItLoadsNoSettingsIfKeysNotStrings)
    {
        nlohmann::json settings{
            {"setting1", 1},
            {"setting2", 2},
        };

        ON_CALL(windows, ReadFromFileMock(std::wstring(L"settings/setting-file.json"), true))
            .WillByDefault(testing::Return(settings.dump()));

        auto provider = GetProvider();
        EXPECT_TRUE(provider.Get("setting1").empty());
        EXPECT_TRUE(provider.Get("setting2").empty());
    }
} // namespace UKControllerPluginTest::Setting
