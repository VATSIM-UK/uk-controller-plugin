#include "euroscope/EuroscopeCoordinateWrapper.h"
#include "flightplan/FlightplanPoint.h"

using UKControllerPlugin::Euroscope::EuroscopeCoordinateWrapper;
using UKControllerPlugin::Flightplan::FlightplanPoint;

namespace UKControllerPluginTest::Flightplan {

    class FlightplanPointTest : public testing::Test
    {
        public:
        FlightplanPointTest() : point(1, "FOOOD", std::make_shared<EuroscopeCoordinateWrapper>(GetPosition()))
        {
        }

        [[nodiscard]] static auto GetPosition() -> EuroScopePlugIn::CPosition
        {
            EuroScopePlugIn::CPosition position;
            position.m_Latitude = 5.0;
            position.m_Longitude = 6.0;
            return position;
        }

        FlightplanPoint point;
    };

    TEST_F(FlightplanPointTest, ItHasAnIndex)
    {
        EXPECT_EQ(1, point.Index());
    }

    TEST_F(FlightplanPointTest, ItHasAnIdentifier)
    {
        EXPECT_EQ("FOOOD", point.Identifier());
    }

    TEST_F(FlightplanPointTest, ItHasAPosition)
    {
        EXPECT_FLOAT_EQ(5, point.Position().ToEuroscopePosition().m_Latitude);
        EXPECT_FLOAT_EQ(6, point.Position().ToEuroscopePosition().m_Longitude);
    }
} // namespace UKControllerPluginTest::Flightplan
