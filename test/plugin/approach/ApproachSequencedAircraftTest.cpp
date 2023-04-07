#include "approach/ApproachSequencedAircraft.h"

using UKControllerPlugin::Approach::ApproachSequencedAircraft;
using UKControllerPlugin::Approach::ApproachSequencingMode;

namespace UKControllerPluginTest::Approach {
    class ApproachSequencedAircraftTest : public testing::Test
    {
        public:
        ApproachSequencedAircraftTest()
            : aircraft("BAW123", ApproachSequencingMode::WakeTurbulence),
              anotherAircraft(
                  std::make_shared<ApproachSequencedAircraft>("BAW456", ApproachSequencingMode::WakeTurbulence))
        {
        }

        ApproachSequencedAircraft aircraft;
        std::shared_ptr<ApproachSequencedAircraft> anotherAircraft;
    };

    TEST_F(ApproachSequencedAircraftTest, ItHasACallsign)
    {
        EXPECT_EQ("BAW123", aircraft.Callsign());
    }

    TEST_F(ApproachSequencedAircraftTest, ItHasAMode)
    {
        EXPECT_EQ(ApproachSequencingMode::WakeTurbulence, aircraft.Mode());
    }

    TEST_F(ApproachSequencedAircraftTest, ModeCanBeSet)
    {
        aircraft.Mode(ApproachSequencingMode::MinimumDistance);
        EXPECT_EQ(ApproachSequencingMode::MinimumDistance, aircraft.Mode());
    }

    TEST_F(ApproachSequencedAircraftTest, ItHasAnExpectedDistance)
    {
        EXPECT_DOUBLE_EQ(6.0, aircraft.ExpectedDistance());
    }

    TEST_F(ApproachSequencedAircraftTest, ExpectedDistanceCanBeSet)
    {
        aircraft.ExpectedDistance(7.0);
        EXPECT_DOUBLE_EQ(7.0, aircraft.ExpectedDistance());
    }

    TEST_F(ApproachSequencedAircraftTest, ItHasANextAircraft)
    {
        EXPECT_EQ(nullptr, aircraft.Next());
        aircraft.Next(anotherAircraft);
        EXPECT_EQ(anotherAircraft, aircraft.Next());
    }

    TEST_F(ApproachSequencedAircraftTest, ItHasAPreviousAircraft)
    {
        EXPECT_EQ(nullptr, aircraft.Previous());
        aircraft.Previous(anotherAircraft);
        EXPECT_EQ(anotherAircraft, aircraft.Previous());
    }

    TEST_F(ApproachSequencedAircraftTest, ItDrawsByDefault)
    {
        EXPECT_TRUE(aircraft.ShouldDraw());
    }

    TEST_F(ApproachSequencedAircraftTest, ItTogglesDraw)
    {
        EXPECT_TRUE(aircraft.ShouldDraw());
        aircraft.ToggleDraw();
        EXPECT_FALSE(aircraft.ShouldDraw());
        aircraft.ToggleDraw();
        EXPECT_TRUE(aircraft.ShouldDraw());
    }
} // namespace UKControllerPluginTest::Approach
