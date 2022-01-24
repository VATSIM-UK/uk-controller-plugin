#include "wake/WakeCalculatorDisplay.h"
#include "wake/WakeCalculatorOptions.h"

using UKControllerPlugin::Wake::WakeCalculatorDisplay;
using UKControllerPlugin::Wake::WakeCalculatorOptions;

namespace UKControllerPluginTest::Wake {
    class WakeCalculatorDisplayTest : public testing::Test
    {
        public:
        WakeCalculatorDisplayTest()
            : leadCallsignSelector(std::make_shared<testing::NiceMock<List::MockPopupList>>()),
              followingCallsignSelector(std::make_shared<testing::NiceMock<List::MockPopupList>>()),
              wakesSchemeSelector(std::make_shared<testing::NiceMock<List::MockPopupList>>()),
              options(std::make_shared<WakeCalculatorOptions>()),
              display(options, leadCallsignSelector, followingCallsignSelector, wakesSchemeSelector, plugin, 1)
        {
        }

        testing::NiceMock<Euroscope::MockEuroscopeRadarScreenLoopbackInterface> radarScreen;
        testing::NiceMock<Euroscope::MockEuroscopePluginLoopbackInterface> plugin;
        std::shared_ptr<testing::NiceMock<List::MockPopupList>> leadCallsignSelector;
        std::shared_ptr<testing::NiceMock<List::MockPopupList>> followingCallsignSelector;
        std::shared_ptr<testing::NiceMock<List::MockPopupList>> wakesSchemeSelector;
        std::shared_ptr<WakeCalculatorOptions> options;
        WakeCalculatorDisplay display;
    };

    TEST_F(WakeCalculatorDisplayTest, LeadCallsignClickTriggersList)
    {
        EXPECT_CALL(*leadCallsignSelector, Trigger(PointEq(POINT({1, 2})))).Times(1);

        display.LeftClick(radarScreen, 1, "leadcallsign", {1, 2}, {});
    }

    TEST_F(WakeCalculatorDisplayTest, FollowingCallsignClickTriggersList)
    {
        EXPECT_CALL(*followingCallsignSelector, Trigger(PointEq(POINT({1, 2})))).Times(1);

        display.LeftClick(radarScreen, 1, "followcallsign", {1, 2}, {});
    }

    TEST_F(WakeCalculatorDisplayTest, SchemeClickTriggersList)
    {
        EXPECT_CALL(*wakesSchemeSelector, Trigger(PointEq(POINT({1, 2})))).Times(1);

        display.LeftClick(radarScreen, 1, "scheme", {1, 2}, {});
    }

    TEST_F(WakeCalculatorDisplayTest, IntermediateClickTriggersList)
    {
        EXPECT_FALSE(options->Intermediate());
        display.LeftClick(radarScreen, 1, "intermediate", {1, 2}, {});
        EXPECT_TRUE(options->Intermediate());
        display.LeftClick(radarScreen, 1, "intermediate", {1, 2}, {});
        EXPECT_FALSE(options->Intermediate());
    }
} // namespace UKControllerPluginTest::Wake
