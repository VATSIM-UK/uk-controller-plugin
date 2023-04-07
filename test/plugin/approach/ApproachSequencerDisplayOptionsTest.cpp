#include "approach/ApproachSequencerDisplayOptions.h"

using UKControllerPlugin::Approach::ApproachSequencerDisplayOptions;

namespace UKControllerPluginTest::Approach {
    class ApproachSequencerDisplayOptionsTest : public testing::Test
    {
        public:
        ApproachSequencerDisplayOptions options;
    };

    TEST_F(ApproachSequencerDisplayOptionsTest, ItStartsWithDefaultPosition)
    {
        EXPECT_EQ(200, options.Position().x);
        EXPECT_EQ(200, options.Position().y);
    }

    TEST_F(ApproachSequencerDisplayOptionsTest, PositionCanBeSet)
    {
        options.Position({300, 400});
        EXPECT_EQ(300, options.Position().x);
        EXPECT_EQ(400, options.Position().y);
    }

    TEST_F(ApproachSequencerDisplayOptionsTest, ItStartsWithNoAirfield)
    {
        EXPECT_TRUE(options.Airfield().empty());
    }

    TEST_F(ApproachSequencerDisplayOptionsTest, AirfieldCanBeSet)
    {
        options.Airfield("EGKK");
        EXPECT_EQ("EGKK", options.Airfield());
    }

    TEST_F(ApproachSequencerDisplayOptionsTest, ItStartsNotVisible)
    {
        EXPECT_FALSE(options.IsVisible());
    }

    TEST_F(ApproachSequencerDisplayOptionsTest, ItTogglesVisibility)
    {
        EXPECT_FALSE(options.IsVisible());
        options.ToggleVisible();
        EXPECT_TRUE(options.IsVisible());
        options.ToggleVisible();
        EXPECT_FALSE(options.IsVisible());
    }

    TEST_F(ApproachSequencerDisplayOptionsTest, ItSetsVisibility)
    {
        EXPECT_FALSE(options.IsVisible());
        options.SetVisible(true);
        EXPECT_TRUE(options.IsVisible());
        options.SetVisible(true);
        EXPECT_TRUE(options.IsVisible());
        options.SetVisible(false);
        EXPECT_FALSE(options.IsVisible());
    }

    TEST_F(ApproachSequencerDisplayOptionsTest, ItStartsNotCollapsed)
    {
        EXPECT_FALSE(options.ContentCollapsed());
    }

    TEST_F(ApproachSequencerDisplayOptionsTest, ItTogglesCollapsed)
    {
        EXPECT_FALSE(options.ContentCollapsed());
        options.ToggleCollapsed();
        EXPECT_TRUE(options.ContentCollapsed());
        options.ToggleCollapsed();
        EXPECT_FALSE(options.ContentCollapsed());
    }

    TEST_F(ApproachSequencerDisplayOptionsTest, ItSetsCollapsed)
    {
        EXPECT_FALSE(options.ContentCollapsed());
        options.SetCollapsed(true);
        EXPECT_TRUE(options.ContentCollapsed());
        options.SetCollapsed(true);
        EXPECT_TRUE(options.ContentCollapsed());
        options.SetCollapsed(false);
        EXPECT_FALSE(options.ContentCollapsed());
    }
} // namespace UKControllerPluginTest::Approach
