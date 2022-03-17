#include "approach/AirfieldApproachOptions.h"
#include "approach/ApproachSequencerOptions.h"

using UKControllerPlugin::Approach::AirfieldApproachOptions;
using UKControllerPlugin::Approach::ApproachSequencerOptions;
using UKControllerPlugin::Approach::ApproachSequencingMode;

namespace UKControllerPluginTest::Approach {
    class ApproachSequencerOptionsTest : public testing::Test
    {
        public:
        ApproachSequencerOptions options;
    };

    TEST_F(ApproachSequencerOptionsTest, ItHasSetsOptionsForAirfield)
    {
        options.Set(
            "EGKK", std::make_shared<AirfieldApproachOptions>(ApproachSequencingMode::MinimumDistance, 4.5, 6.5));
        auto settings = options.Get("EGKK");
        EXPECT_EQ(ApproachSequencingMode::MinimumDistance, settings.defaultMode);
        EXPECT_DOUBLE_EQ(6.5, settings.minimumSeparationRequirement);
        EXPECT_DOUBLE_EQ(4.5, settings.targetDistance);
    }
} // namespace UKControllerPluginTest::Approach
