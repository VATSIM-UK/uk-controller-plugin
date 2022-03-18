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

    TEST_F(ApproachSequencerOptionsTest, ItStartsWithNoAirfieldOptions)
    {
        EXPECT_EQ(0, options.CountAirfieldOptions());
    }

    TEST_F(ApproachSequencerOptionsTest, ItDoesntHaveOptionsForAirfield)
    {
        EXPECT_FALSE(options.HasAirfield("EGKK"));
    }

    TEST_F(ApproachSequencerOptionsTest, ItHasOptionsForAirfield)
    {
        options.Set("EGKK", nullptr);
        EXPECT_TRUE(options.HasAirfield("EGKK"));
    }

    TEST_F(ApproachSequencerOptionsTest, ItHasSetsOptionsForAirfield)
    {
        options.Set(
            "EGKK", std::make_shared<AirfieldApproachOptions>(ApproachSequencingMode::MinimumDistance, 4.5, 6.5));
        auto settings = options.Get("EGKK");
        EXPECT_EQ(ApproachSequencingMode::MinimumDistance, settings.defaultMode);
        EXPECT_DOUBLE_EQ(6.5, settings.minimumSeparationRequirement);
        EXPECT_DOUBLE_EQ(4.5, settings.targetDistance);
        EXPECT_EQ(1, options.CountAirfieldOptions());
    }

    TEST_F(ApproachSequencerOptionsTest, ItHasDefaultOptionsForAirfield)
    {
        auto settings = options.Get("EGKK");
        EXPECT_EQ(ApproachSequencingMode::WakeTurbulence, settings.defaultMode);
        EXPECT_DOUBLE_EQ(3, settings.minimumSeparationRequirement);
        EXPECT_DOUBLE_EQ(6, settings.targetDistance);
        EXPECT_EQ(1, options.CountAirfieldOptions());
    }

    TEST_F(ApproachSequencerOptionsTest, ItDoesntIterateAirfieldsIfNotPreviouslyRetrieved)
    {
        bool functionCalled = false;
        options.ForEach(
            [&functionCalled](const std::string&, const AirfieldApproachOptions&) { functionCalled = true; });

        EXPECT_FALSE(functionCalled);
    }

    TEST_F(ApproachSequencerOptionsTest, ItIteratesAirfields)
    {
        static_cast<void>(options.Get("EGKK"));
        bool functionCalled = false;
        options.ForEach([&functionCalled](const std::string& airfield, const AirfieldApproachOptions& options) {
            functionCalled = true;
            EXPECT_EQ("EGKK", airfield);
            EXPECT_EQ(ApproachSequencingMode::WakeTurbulence, options.defaultMode);
            EXPECT_DOUBLE_EQ(3.0, options.minimumSeparationRequirement);
            EXPECT_DOUBLE_EQ(6.0, options.targetDistance);
        });
        EXPECT_TRUE(functionCalled);
    }
} // namespace UKControllerPluginTest::Approach
