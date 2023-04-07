#include "approach/ApproachSequencerDistanceOptions.h"

using UKControllerPlugin::Approach::DistanceOptions;

namespace UKControllerPluginTest::Approach {
    class ApproachSequencerDistanceOptionsTest : public testing::Test
    {
    };

    TEST_F(ApproachSequencerDistanceOptionsTest, ItHasOptions)
    {
        std::set<double> expected{
            2.5,
            3.0,
            4.0,
            5.0,
            6.0,
            7.0,
            8.0,
            9.0,
            10.0,
            11.0,
            12.0,
            13.0,
            14.0,
            15.0,
        };

        EXPECT_EQ(expected, DistanceOptions());
    }
} // namespace UKControllerPluginTest::Approach
