#include "approach/ApproachSequencerDisplayOptions.h"

using UKControllerPlugin::Approach::ApproachSequencerDisplayOptions;

namespace UKControllerPluginTest::Approach {
    class ApproachSequencerDisplayOptionsTest : public testing::Test
    {
        public:
        ApproachSequencerDisplayOptions options;
    };

    TEST_F(ApproachSequencerDisplayOptionsTest, ItStartsWithNoAirfield)
    {
        EXPECT_TRUE(options.Airfield().empty());
    }

    TEST_F(ApproachSequencerDisplayOptionsTest, AirfieldCanBeSet)
    {
        options.Airfield("EGKK");
        EXPECT_EQ("EGKK", options.Airfield());
    }
} // namespace UKControllerPluginTest::Approach
