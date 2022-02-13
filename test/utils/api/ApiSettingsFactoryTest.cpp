#include "api/ApiSettings.h"
#include "api/ApiSettingsFactory.h"

using UKControllerPluginTest::Setting::MockSettingRepository;
using UKControllerPluginUtils::Api::Make;

namespace UKControllerPluginUtilsTest::Api {
    class ApiSettingsFactoryTest : public testing::Test
    {
        public:
        ApiSettingsFactoryTest()
        {
            ON_CALL(settings, GetSetting("api-url", "https://ukcp.vatsim.uk"))
                .WillByDefault(testing::Return("url"));

            ON_CALL(settings, GetSetting("api-key", "")).WillByDefault(testing::Return("key"));
        }
        testing::NiceMock<MockSettingRepository> settings;
    };

    TEST_F(ApiSettingsFactoryTest, ItReturnsSettingsWithUrl)
    {
        EXPECT_EQ("url", Make(settings)->Url());
    }

    TEST_F(ApiSettingsFactoryTest, ItReturnsSettingsWitKey)
    {
        EXPECT_EQ("key", Make(settings)->Key());
    }

    TEST_F(ApiSettingsFactoryTest, ItReturnsTheSameSettingsEachTime)
    {
        EXPECT_EQ(Make(settings), Make(settings));
    }
} // namespace UKControllerPluginUtilsTest::Api
