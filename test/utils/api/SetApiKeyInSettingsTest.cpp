#include "api/ApiSettings.h"
#include "api/SetApiKeyInSettings.h"

namespace UKControllerPluginUtilsTest::Api {
    class SetApiKeyInSettingsTest : public testing::Test
    {
        public:
        SetApiKeyInSettingsTest() : apiSettings("", ""), setApiKey(apiSettings)
        {
        }

        UKControllerPluginUtils::Api::ApiSettings apiSettings;
        UKControllerPluginUtils::Api::SetApiKeyInSettings setApiKey;
    };

    TEST_F(SetApiKeyInSettingsTest, ItSetsTheKey)
    {
        setApiKey.OnEvent({"abc"});
        EXPECT_EQ("", apiSettings.Url());
        EXPECT_EQ("abc", apiSettings.Key());
    }
} // namespace UKControllerPluginUtilsTest::Api
