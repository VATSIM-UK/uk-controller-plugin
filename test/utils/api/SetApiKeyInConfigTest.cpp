#include "api/SetApiKeyInConfig.h"

namespace UKControllerPluginUtilsTest::Api {
    class SetApiKeyInConfigTest : public testing::Test
    {
        public:
        SetApiKeyInConfigTest() : setApiKey(settingProvider)
        {
        }

        testing::NiceMock<UKControllerPluginTest::Setting::MockSettingRepository> settingProvider;
        UKControllerPluginUtils::Api::SetApiKeyInConfig setApiKey;
    };

    TEST_F(SetApiKeyInConfigTest, ItSetsTheKey)
    {
        EXPECT_CALL(settingProvider, UpdateSetting("api-key", "abc")).Times(1);

        setApiKey.OnEvent({"abc"});
    }
} // namespace UKControllerPluginUtilsTest::Api
