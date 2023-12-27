#include "runway/Runway.h"

using UKControllerPlugin::Runway::Runway;

namespace UKControllerPluginTest::Runway {
    class RunwayTest : public testing::Test
    {
        public:
        RunwayTest() : runway(1, 2, "27L", 123, GetPosition())
        {
        }

        class Runway runway;

        private:
        auto GetPosition() -> EuroScopePlugIn::CPosition
        {
            EuroScopePlugIn::CPosition position;
            position.m_Latitude = 3;
            position.m_Longitude = 4;
            return position;
        }
    };

    TEST_F(RunwayTest, ItHasAnId)
    {
        EXPECT_EQ(1, runway.Id());
    }

    TEST_F(RunwayTest, ItHasAnAirfieldId)
    {
        EXPECT_EQ(2, runway.AirfieldId());
    }

    TEST_F(RunwayTest, ItHasAnIdentifier)
    {
        EXPECT_EQ("27L", runway.Identifier());
    }

    TEST_F(RunwayTest, ItHasAHeading)
    {
        EXPECT_EQ(123, runway.Heading());
    }

    TEST_F(RunwayTest, ItHasAThreshold)
    {
        EXPECT_EQ(3, runway.Threshold().m_Latitude);
        EXPECT_EQ(4, runway.Threshold().m_Longitude);
    }

    TEST_F(RunwayTest, ItCalculatesGlideslopeAltitudeAtDistance)
    {
        // At the threshold
        EXPECT_EQ(196, runway.GlideslopeAltitudeAtDistance(0));

        // 6.95 nm (8 miles)
        EXPECT_EQ(2407, runway.GlideslopeAltitudeAtDistance(6.95));
    }

    TEST_F(RunwayTest, ItCalculatesRunwayHeadingLineSlope)
    {
        EXPECT_DOUBLE_EQ(-1.5398649638145827, runway.RunwayHeadingLineSlope());
    }

    TEST_F(RunwayTest, ItCalculatesRunwayPerpendicularHeadingLineSlope)
    {
        EXPECT_DOUBLE_EQ(0.64940759319751062, runway.RunwayPerpendicularHeadingLineSlope());
    }
} // namespace UKControllerPluginTest::Runway
