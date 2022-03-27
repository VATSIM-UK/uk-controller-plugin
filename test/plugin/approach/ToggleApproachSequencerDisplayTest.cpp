#include "approach/ApproachSequencerDisplayOptions.h"
#include "approach/ToggleApproachSequencerDisplay.h"

using UKControllerPlugin::Approach::ApproachSequencerDisplayOptions;
using UKControllerPlugin::Approach::ToggleApproachSequencerDisplay;

namespace UKControllerPluginTest::Approach {
    class ToggleApproachSequencerDisplayTest : public testing::Test
    {
        public:
        ToggleApproachSequencerDisplayTest()
            : options(std::make_shared<ApproachSequencerDisplayOptions>()), toggle(options)
        {
        }

        std::shared_ptr<ApproachSequencerDisplayOptions> options;
        ToggleApproachSequencerDisplay toggle;
    };

    TEST_F(ToggleApproachSequencerDisplayTest, ItHasAMenuItemString)
    {
        EXPECT_EQ("Show Approach Sequencer", toggle.MenuItem());
    }

    TEST_F(ToggleApproachSequencerDisplayTest, ItHasIsVisible)
    {
        EXPECT_FALSE(toggle.IsVisible());
    }

    TEST_F(ToggleApproachSequencerDisplayTest, ItTogglesVisibility)
    {
        EXPECT_FALSE(toggle.IsVisible());
        toggle.Toggle();
        EXPECT_TRUE(toggle.IsVisible());
        toggle.Toggle();
        EXPECT_FALSE(toggle.IsVisible());
    }
} // namespace UKControllerPluginTest::Approach
