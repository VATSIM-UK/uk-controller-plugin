#include "radarscreen/ConfigurableDisplayCallbackFactory.h"

namespace UKControllerPluginTest::RadarScreen {
    class ConfigurableDisplayCallbackFactoryTest : public testing::Test
    {
    };

    TEST_F(ConfigurableDisplayCallbackFactoryTest, ItReturnsACallback)
    {
        const auto mockDisplay = std::make_shared<RadarScreen::MockConfigurableDisplay>();
        const auto callback =
            UKControllerPlugin::RadarScreen::CreateConfigurableDisplayCallback(1, "test", mockDisplay);

        EXPECT_EQ(1, callback.functionId);
        EXPECT_EQ("test", callback.description);
        EXPECT_EQ(0, mockDisplay->configured);
        callback.function(2, "test", RECT{1, 2, 3, 4});
        EXPECT_EQ(1, mockDisplay->configured);
    }
} // namespace UKControllerPluginTest::RadarScreen
