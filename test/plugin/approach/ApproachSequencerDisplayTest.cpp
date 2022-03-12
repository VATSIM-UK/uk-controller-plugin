#include "approach/ApproachSequencerDisplay.h"
#include "approach/ApproachSequencerDisplayOptions.h"

using UKControllerPlugin::Approach::ApproachSequencerDisplay;
using UKControllerPlugin::Approach::ApproachSequencerDisplayOptions;

namespace UKControllerPluginTest::Approach {
    class ApproachSequencerDisplayTest : public testing::Test
    {
        public:
        ApproachSequencerDisplayTest()
            : selectorList(std::make_shared<List::MockPopupList>()),
              options(std::make_shared<ApproachSequencerDisplayOptions>()), display(options, selectorList, 55)
        {
        }

        std::shared_ptr<List::MockPopupList> selectorList;
        testing::NiceMock<Euroscope::MockEuroscopeRadarScreenLoopbackInterface> radarScreen;
        std::shared_ptr<ApproachSequencerDisplayOptions> options;
        ApproachSequencerDisplay display;
    };

    TEST_F(ApproachSequencerDisplayTest, ItTogglesContentCollapsing)
    {
        EXPECT_FALSE(options->ContentCollapsed());
        display.LeftClick(radarScreen, 1, "collapseButton", {}, {});
        EXPECT_TRUE(options->ContentCollapsed());
        display.LeftClick(radarScreen, 1, "collapseButton", {}, {});
        EXPECT_FALSE(options->ContentCollapsed());
    }

    TEST_F(ApproachSequencerDisplayTest, ItCanBeClosed)
    {
        options->SetVisible(true);
        display.LeftClick(radarScreen, 1, "closeButton", {}, {});
        EXPECT_FALSE(display.IsVisible());
    }

    TEST_F(ApproachSequencerDisplayTest, ItTriggersAirfieldList)
    {
        EXPECT_CALL(*selectorList, Trigger(PointEq(POINT{1, 2}))).Times(1);

        display.LeftClick(radarScreen, 1, "aircraftSelector", {1, 2}, {});
    }
} // namespace UKControllerPluginTest::Approach
