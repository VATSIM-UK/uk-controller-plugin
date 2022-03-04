#include "api/ApiSettings.h"

using UKControllerPluginUtils::Api::ApiSettings;

namespace UKControllerPluginUtilsTest::Api {
    class ApiSettingsTest : public testing::Test
    {
        public:
        ApiSettingsTest() : settings("url", "key")
        {
        }
        ApiSettings settings;
    };

    TEST_F(ApiSettingsTest, ItHasAUrl)
    {
        EXPECT_EQ("url", settings.Url());
    }

    TEST_F(ApiSettingsTest, ItHasAKey)
    {
        EXPECT_EQ("key", settings.Key());
    }

    TEST_F(ApiSettingsTest, ItCanSetItsUrl)
    {
        settings.Url("url2");
        EXPECT_EQ("url2", settings.Url());
        EXPECT_EQ("key", settings.Key());
    }

    TEST_F(ApiSettingsTest, ItCanSetItsKey)
    {
        settings.Key("key2");
        EXPECT_EQ("url", settings.Url());
        EXPECT_EQ("key2", settings.Key());
    }
} // namespace UKControllerPluginUtilsTest::Api
