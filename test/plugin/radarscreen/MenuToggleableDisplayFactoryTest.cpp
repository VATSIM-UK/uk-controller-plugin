#include "plugin/FunctionCallEventHandler.h"
#include "radarscreen/ConfigurableDisplayCollection.h"
#include "radarscreen/MenuToggleableDisplayFactory.h"

using UKControllerPlugin::Plugin::FunctionCallEventHandler;
using UKControllerPlugin::RadarScreen::ConfigurableDisplayCollection;
using UKControllerPlugin::RadarScreen::MenuToggleableDisplayFactory;

namespace UKControllerPluginTest::RadarScreen {
    class MenuToggleableDisplayFactoryTest : public testing::Test
    {
        public:
        MenuToggleableDisplayFactoryTest()
            : mockDisplay(std::make_shared<testing::NiceMock<RadarScreen::MockMenuToggleableDisplay>>()),
              factory(callbacks, displays)
        {
        }

        testing::NiceMock<Euroscope::MockEuroScopeCFlightPlanInterface> mockFlightplan;
        testing::NiceMock<Euroscope::MockEuroScopeCRadarTargetInterface> mockRadarTarget;
        std::shared_ptr<testing::NiceMock<RadarScreen::MockMenuToggleableDisplay>> mockDisplay;
        FunctionCallEventHandler callbacks;
        ConfigurableDisplayCollection displays;
        MenuToggleableDisplayFactory factory;
    };

    TEST_F(MenuToggleableDisplayFactoryTest, ItRegistersCallbackFunction)
    {
        EXPECT_CALL(*mockDisplay, Toggle).Times(1);

        factory.RegisterDisplay(mockDisplay, "Test");
        EXPECT_TRUE(callbacks.HasCallbackByDescription("Test"));
        callbacks.CallFunction(
            callbacks.ReserveNextDynamicFunctionId() - 1, "", mockFlightplan, mockRadarTarget, {}, {});
    }

    TEST_F(MenuToggleableDisplayFactoryTest, ItRegistersConfigurableDisplay)
    {
        factory.RegisterDisplay(mockDisplay, "Test");
        EXPECT_EQ(1, displays.CountDisplays());
    }
} // namespace UKControllerPluginTest::RadarScreen
