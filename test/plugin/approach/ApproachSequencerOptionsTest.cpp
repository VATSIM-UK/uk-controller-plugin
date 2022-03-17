#include "approach/ApproachSequencerOptions.h"

using UKControllerPlugin::Approach::ApproachSequencerOptions;
using UKControllerPlugin::Approach::ApproachSequencingMode;

namespace UKControllerPluginTest::Approach {
    class ApproachSequencerOptionsTest : public testing::Test
    {
        public:
        ApproachSequencerOptions options;
    };

    TEST_F(ApproachSequencerOptionsTest, ItHasADefaultModeValue)
    {
        EXPECT_EQ(ApproachSequencingMode::WakeTurbulence, options.DefaultMode("EGKK"));
    }

    TEST_F(ApproachSequencerOptionsTest, ItSetsDefaultModeForAirfield)
    {
        options.DefaultMode("EGKK", ApproachSequencingMode::MinimumDistance);
        EXPECT_EQ(ApproachSequencingMode::MinimumDistance, options.DefaultMode("EGKK"));
        EXPECT_EQ(ApproachSequencingMode::WakeTurbulence, options.DefaultMode("EGLL"));
    }

    TEST_F(ApproachSequencerOptionsTest, ItHasADefaultMinimumSeparationValue)
    {
        EXPECT_DOUBLE_EQ(3.0, options.MinimumSeparation("EGKK"));
    }

    TEST_F(ApproachSequencerOptionsTest, ItSetsAMinimumSeparationValueForAirfield)
    {
        options.MinimumSeparation("EGKK", 4.5);
        EXPECT_DOUBLE_EQ(4.5, options.MinimumSeparation("EGKK"));
        EXPECT_DOUBLE_EQ(3.0, options.MinimumSeparation("EGLL"));
    }

    TEST_F(ApproachSequencerOptionsTest, ItHasADefaultTargetValue)
    {
        EXPECT_DOUBLE_EQ(6.0, options.TargetDistance("EGKK"));
    }

    TEST_F(ApproachSequencerOptionsTest, ItSetsATargetValueForAirfield)
    {
        options.TargetDistance("EGKK", 4.5);
        EXPECT_DOUBLE_EQ(4.5, options.TargetDistance("EGKK"));
        EXPECT_DOUBLE_EQ(6.0, options.TargetDistance("EGLL"));
    }
} // namespace UKControllerPluginTest::Approach
