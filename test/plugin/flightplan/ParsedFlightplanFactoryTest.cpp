#include "flightplan/ParsedFlightplan.h"
#include "flightplan/ParsedFlightplanFactory.h"

using UKControllerPlugin::Flightplan::ParseFlightplanFromEuroscope;

namespace UKControllerPluginTest::Flightplan {

    class ParsedFlightplanFactoryTest : public testing::Test
    {
    };

    TEST_F(ParsedFlightplanFactoryTest, TestItCreatesParsedFlightplan)
    {
        testing::NiceMock<Euroscope::MockEuroscopeExtractedRouteInterface> mockExtractedRoute;
        EuroScopePlugIn::CPosition position1;
        position1.m_Latitude = 5.0;
        position1.m_Longitude = 6.0;

        EuroScopePlugIn::CPosition position2;
        position2.m_Latitude = 5.0;
        position2.m_Longitude = 6.0;

        EXPECT_CALL(mockExtractedRoute, GetPointsNumber()).WillRepeatedly(testing::Return(2));
        EXPECT_CALL(mockExtractedRoute, GetPointName(0)).Times(1).WillOnce(testing::Return("ALAN"));
        EXPECT_CALL(mockExtractedRoute, GetPointPosition(0)).Times(1).WillOnce(testing::Return(position1));
        EXPECT_CALL(mockExtractedRoute, GetPointName(1)).Times(1).WillOnce(testing::Return("STEVE"));
        EXPECT_CALL(mockExtractedRoute, GetPointPosition(1)).Times(1).WillOnce(testing::Return(position2));

        const auto parsed = ParseFlightplanFromEuroscope(mockExtractedRoute);
        EXPECT_EQ(2, parsed->CountPoints());
        EXPECT_TRUE(parsed->HasPointByIdentifier("ALAN"));
        EXPECT_TRUE(parsed->HasPointByIdentifier("STEVE"));
    }
} // namespace UKControllerPluginTest::Flightplan
