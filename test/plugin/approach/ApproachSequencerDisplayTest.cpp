#include "approach/ApproachSequencerDisplay.h"
#include "approach/ApproachSequencerDisplayOptions.h"

using UKControllerPlugin::Approach::ApproachSequencerDisplay;
using UKControllerPlugin::Approach::ApproachSequencerDisplayOptions;

namespace UKControllerPluginTest::Approach {
    class ApproachSequencerDisplayTest : public testing::Test
    {
        public:
        ApproachSequencerDisplayTest()
            : options(std::make_shared<ApproachSequencerDisplayOptions>()), display(options, 55)
        {
        }

        testing::NiceMock<Euroscope::MockEuroscopeRadarScreenLoopbackInterface> radarScreen;
        std::shared_ptr<ApproachSequencerDisplayOptions> options;
        ApproachSequencerDisplay display;
    };

    TEST_F(ApproachSequencerDisplayTest, ItTogglesContentCollapsing)
    {
        EXPECT_FALSE(display.ContentCollapsed());
        display.LeftClick(radarScreen, 1, "collapseButton", {}, {});
        EXPECT_TRUE(display.ContentCollapsed());
        display.LeftClick(radarScreen, 1, "collapseButton", {}, {});
        EXPECT_FALSE(display.ContentCollapsed());
    }

    TEST_F(ApproachSequencerDisplayTest, ItCanBeClosed)
    {
        EXPECT_TRUE(display.IsVisible());
        display.LeftClick(radarScreen, 1, "closeButton", {}, {});
        EXPECT_FALSE(display.IsVisible());
    }
} // namespace UKControllerPluginTest::Approach
