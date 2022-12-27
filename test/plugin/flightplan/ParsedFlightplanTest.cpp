#include "euroscope/EuroscopeCoordinateWrapper.h"
#include "flightplan/FlightplanPoint.h"
#include "flightplan/ParsedFlightplan.h"

using UKControllerPlugin::Euroscope::EuroscopeCoordinateWrapper;
using UKControllerPlugin::Flightplan::ParsedFlightplan;

namespace UKControllerPluginTest::Flightplan {
    class ParsedFlightplanTest : public testing::Test
    {
        public:
            auto GetPosition() -> std::shared_ptr<EuroscopeCoordinateWrapper>
            {
                return std::make_shared<EuroscopeCoordinateWrapper>(EuroScopePlugIn::CPosition());
            }

            ParsedFlightplan flightplan;
    };

    TEST_F(ParsedFlightplanTest, ItAddsAPoint)
    {
        flightplan.AddPoint(std::make_shared<FlightplanPoint>(1, "FOOOD", GetPosition()));
        EXPECT_EQ(1, flightplan.CountPoints());
    }

    TEST_F(ParsedFlightplanTest, ItDoesntAddDuplicatePointsByIndex)
    {
        flightplan.AddPoint(std::make_shared<FlightplanPoint>(1, "FOOOD", GetPosition()));
        flightplan.AddPoint(std::make_shared<FlightplanPoint>(2, "FOOOD", GetPosition()));
        flightplan.AddPoint(std::make_shared<FlightplanPoint>(3, "FOOOD", GetPosition()));
        flightplan.AddPoint(std::make_shared<FlightplanPoint>(1, "FOOOD", GetPosition()));
        EXPECT_EQ(3, flightplan.CountPoints());
    }

    TEST_F(ParsedFlightplanTest, ItDoesntHaveAPointByIdentifier)
    {
        flightplan.AddPoint(std::make_shared<FlightplanPoint>(1, "FOOOD", GetPosition()));
        EXPECT_FALSE(flightplan.HasPointByIdentifier("NO"));
    }

    TEST_F(ParsedFlightplanTest, ItHasAPointByIdentifier)
    {
        flightplan.AddPoint(std::make_shared<FlightplanPoint>(1, "FOOOD", GetPosition()));
        EXPECT_TRUE(flightplan.HasPointByIdentifier("FOOOD"));
    }

    TEST_F(ParsedFlightplanTest, ItGetsNoPointsByIdentifier)
    {
        EXPECT_EQ(std::list<std::shared_ptr<FlightplanPoint>>(), flightplan.PointsByIdentifier("LOL"));
    }

    TEST_F(ParsedFlightplanTest, ItGetsPointsByIdentifier)
    {
        auto point1 = std::make_shared<FlightplanPoint>(1, "FOOOD", GetPosition());
        auto point2 = std::make_shared<FlightplanPoint>(2, "FOOOD", GetPosition());
        flightplan.AddPoint(point1);
        flightplan.AddPoint(point2);
        flightplan.AddPoint(std::make_shared<FlightplanPoint>(3, "ROOOD", GetPosition()));

        EXPECT_EQ(std::list<std::shared_ptr<FlightplanPoint>>({point1, point2}), flightplan.PointsByIdentifier("FOOOD"));
    }

    TEST_F(ParsedFlightplanTest, ItReturnsNullPtrIfNoPointByIndex)
    {
        flightplan.AddPoint(std::make_shared<FlightplanPoint>(1, "FOOOD", GetPosition()));
        flightplan.AddPoint(std::make_shared<FlightplanPoint>(2, "FOOOD", GetPosition()));
        flightplan.AddPoint(std::make_shared<FlightplanPoint>(3, "FOOOD", GetPosition()));

        EXPECT_EQ(nullptr, flightplan.PointByIndex(55));
    }

    TEST_F(ParsedFlightplanTest, ItReturnsPointByIndex)
    {
        auto point1 = std::make_shared<FlightplanPoint>(1, "FOOOD", GetPosition());
        auto point2 = std::make_shared<FlightplanPoint>(2, "FOOOD", GetPosition());
        flightplan.AddPoint(point1);
        flightplan.AddPoint(point2);
        flightplan.AddPoint(std::make_shared<FlightplanPoint>(3, "ROOOD", GetPosition()));

        EXPECT_EQ(point2, flightplan.PointByIndex(2));
    }
} // UKControllerPluginTest::Flightplan
