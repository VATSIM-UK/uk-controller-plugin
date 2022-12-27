#include "flightplan/FlightplanPoint.h"
#include "flightplan/ParsedFlightplan.h"
#include "intention/ExitCone.h"

using UKControllerPlugin::Flightplan::FlightplanPoint;
using UKControllerPlugin::Flightplan::ParsedFlightplan;
using UKControllerPlugin::IntentionCode::ExitCone;

namespace UKControllerPluginTest::IntentionCode {
    class ExitConeTest : testing::Test
    {
        public:
            ExitConeTest()
                : mockCoordinate(std::make_shared<testing::NiceMock<Euroscope::MockEuroscopeCoordinateInterface>>()),
                mockCoordinate2(std::make_shared<testing::NiceMock<Euroscope::MockEuroscopeCoordinateInterface>>())
            {

            }
            testing::NiceMock<MockEuroScopeCFlightPlanInterface> flightplan;
            std::shared_ptr<testing::NiceMock<Euroscope::MockEuroscopeCoordinateInterface>> mockCoordinate;
            std::shared_ptr<testing::NiceMock<Euroscope::MockEuroscopeCoordinateInterface>> mockCoordinate2;
    };

    TEST_F(ExitConeTest, AircraftIsNotExitingIfNoFollowingPoint)
    {
        ExitCone cone(100, 200);
        const auto parsedFlightplan = std::make_shared<ParsedFlightplan>();
        const auto point = std::make_shared<FlightplanPoint>(1, "FOOO", mockCoordinate);
        parsedFlightplan->AddPoint(point);

        ON_CALL(flightplan, GetParsedFlightplan)
            .WillByDefault(testing::Return(parsedFlightplan));

        EXPECT_FALSE(cone.AircraftIsExiting(*point, flightplan));
    }

    TEST_F(ExitConeTest, AircraftIsNotExitingBeforeConeStart)
    {
        ExitCone cone(100, 200);
        const auto parsedFlightplan = std::make_shared<ParsedFlightplan>();
        const auto point = std::make_shared<FlightplanPoint>(1, "FOOO", mockCoordinate);
        parsedFlightplan->AddPoint(point);
        parsedFlightplan->AddPoint(std::make_shared<FlightplanPoint>(2, "BOOO", mockCoordinate2))

        ON_CALL(*mockCoordinate, DirectionTo(testing::Ref(*mockCoordinate2)))
            .WillByDefault(testing::Return(99.0));

        ON_CALL(flightplan, GetParsedFlightplan)
            .WillByDefault(testing::Return(parsedFlightplan));

        EXPECT_FALSE(cone.AircraftIsExiting(*point, flightplan));
    }

    TEST_F(ExitConeTest, AircraftIsNotExitingAfterConeEnd)
    {
        ExitCone cone(100, 200);
        const auto parsedFlightplan = std::make_shared<ParsedFlightplan>();
        const auto point = std::make_shared<FlightplanPoint>(1, "FOOO", mockCoordinate);
        parsedFlightplan->AddPoint(point);
        parsedFlightplan->AddPoint(std::make_shared<FlightplanPoint>(2, "BOOO", mockCoordinate2))

        ON_CALL(*mockCoordinate, DirectionTo(testing::Ref(*mockCoordinate2)))
            .WillByDefault(testing::Return(201.0));

        ON_CALL(flightplan, GetParsedFlightplan)
            .WillByDefault(testing::Return(parsedFlightplan));

        EXPECT_FALSE(cone.AircraftIsExiting(*point, flightplan));
    }

    TEST_F(ExitConeTest, AircraftIsExitingLowerBoundaryOfCone)
    {
        ExitCone cone(100, 200);
        const auto parsedFlightplan = std::make_shared<ParsedFlightplan>();
        const auto point = std::make_shared<FlightplanPoint>(1, "FOOO", mockCoordinate);
        parsedFlightplan->AddPoint(point);
        parsedFlightplan->AddPoint(std::make_shared<FlightplanPoint>(2, "BOOO", mockCoordinate2))

        ON_CALL(*mockCoordinate, DirectionTo(testing::Ref(*mockCoordinate2)))
            .WillByDefault(testing::Return(100.0));

        ON_CALL(flightplan, GetParsedFlightplan)
            .WillByDefault(testing::Return(parsedFlightplan));

        EXPECT_TRUE(cone.AircraftIsExiting(*point, flightplan));
    }

    TEST_F(ExitConeTest, AircraftIsExitingUpperBoundaryOfCone)
    {
        ExitCone cone(100, 200);
        const auto parsedFlightplan = std::make_shared<ParsedFlightplan>();
        const auto point = std::make_shared<FlightplanPoint>(1, "FOOO", mockCoordinate);
        parsedFlightplan->AddPoint(point);
        parsedFlightplan->AddPoint(std::make_shared<FlightplanPoint>(2, "BOOO", mockCoordinate2))

        ON_CALL(*mockCoordinate, DirectionTo(testing::Ref(*mockCoordinate2)))
            .WillByDefault(testing::Return(200.0));

        ON_CALL(flightplan, GetParsedFlightplan)
            .WillByDefault(testing::Return(parsedFlightplan));

        EXPECT_TRUE(cone.AircraftIsExiting(*point, flightplan));
    }

    TEST_F(ExitConeTest, AircraftIsExitingMiddleOfCone)
    {
        ExitCone cone(100, 200);
        const auto parsedFlightplan = std::make_shared<ParsedFlightplan>();
        const auto point = std::make_shared<FlightplanPoint>(1, "FOOO", mockCoordinate);
        parsedFlightplan->AddPoint(point);
        parsedFlightplan->AddPoint(std::make_shared<FlightplanPoint>(2, "BOOO", mockCoordinate2))

        ON_CALL(*mockCoordinate, DirectionTo(testing::Ref(*mockCoordinate2)))
            .WillByDefault(testing::Return(150.0));

        ON_CALL(flightplan, GetParsedFlightplan)
            .WillByDefault(testing::Return(parsedFlightplan));

        EXPECT_TRUE(cone.AircraftIsExiting(*point, flightplan));
    }

    TEST_F(ExitConeTest, AircraftIsNotExitingBeforeConeStartPastNorth)
    {
        ExitCone cone(340, 20);
        const auto parsedFlightplan = std::make_shared<ParsedFlightplan>();
        const auto point = std::make_shared<FlightplanPoint>(1, "FOOO", mockCoordinate);
        parsedFlightplan->AddPoint(point);
        parsedFlightplan->AddPoint(std::make_shared<FlightplanPoint>(2, "BOOO", mockCoordinate2))

        ON_CALL(*mockCoordinate, DirectionTo(testing::Ref(*mockCoordinate2)))
            .WillByDefault(testing::Return(339.0));

        ON_CALL(flightplan, GetParsedFlightplan)
            .WillByDefault(testing::Return(parsedFlightplan));

        EXPECT_FALSE(cone.AircraftIsExiting(*point, flightplan));
    }

    TEST_F(ExitConeTest, AircraftIsNotExitingAfterConeEndPastNorth)
    {
        ExitCone cone(340, 20);
        const auto parsedFlightplan = std::make_shared<ParsedFlightplan>();
        const auto point = std::make_shared<FlightplanPoint>(1, "FOOO", mockCoordinate);
        parsedFlightplan->AddPoint(point);
        parsedFlightplan->AddPoint(std::make_shared<FlightplanPoint>(2, "BOOO", mockCoordinate2))

        ON_CALL(*mockCoordinate, DirectionTo(testing::Ref(*mockCoordinate2)))
            .WillByDefault(testing::Return(21));

        ON_CALL(flightplan, GetParsedFlightplan)
            .WillByDefault(testing::Return(parsedFlightplan));

        EXPECT_FALSE(cone.AircraftIsExiting(*point, flightplan));
    }

    TEST_F(ExitConeTest, AircraftIsExitingLowerBoundaryOfConePastNorth)
    {
        ExitCone cone(340, 20);
        const auto parsedFlightplan = std::make_shared<ParsedFlightplan>();
        const auto point = std::make_shared<FlightplanPoint>(1, "FOOO", mockCoordinate);
        parsedFlightplan->AddPoint(point);
        parsedFlightplan->AddPoint(std::make_shared<FlightplanPoint>(2, "BOOO", mockCoordinate2))

        ON_CALL(*mockCoordinate, DirectionTo(testing::Ref(*mockCoordinate2)))
            .WillByDefault(testing::Return(340.0));

        ON_CALL(flightplan, GetParsedFlightplan)
            .WillByDefault(testing::Return(parsedFlightplan));

        EXPECT_TRUE(cone.AircraftIsExiting(*point, flightplan));
    }

    TEST_F(ExitConeTest, AircraftIsExitingUpperBoundaryOfConePastNorth)
    {
        ExitCone cone(340, 20);
        const auto parsedFlightplan = std::make_shared<ParsedFlightplan>();
        const auto point = std::make_shared<FlightplanPoint>(1, "FOOO", mockCoordinate);
        parsedFlightplan->AddPoint(point);
        parsedFlightplan->AddPoint(std::make_shared<FlightplanPoint>(2, "BOOO", mockCoordinate2))

        ON_CALL(*mockCoordinate, DirectionTo(testing::Ref(*mockCoordinate2)))
            .WillByDefault(testing::Return(20.0));

        ON_CALL(flightplan, GetParsedFlightplan)
            .WillByDefault(testing::Return(parsedFlightplan));

        EXPECT_TRUE(cone.AircraftIsExiting(*point, flightplan));
    }

    TEST_F(ExitConeTest, AircraftIsExitingMiddleOfConePastNorth)
    {
        ExitCone cone(340, 20);
        const auto parsedFlightplan = std::make_shared<ParsedFlightplan>();
        const auto point = std::make_shared<FlightplanPoint>(1, "FOOO", mockCoordinate);
        parsedFlightplan->AddPoint(point);
        parsedFlightplan->AddPoint(std::make_shared<FlightplanPoint>(2, "BOOO", mockCoordinate2))

        ON_CALL(*mockCoordinate, DirectionTo(testing::Ref(*mockCoordinate2)))
            .WillByDefault(testing::Return(0));

        ON_CALL(flightplan, GetParsedFlightplan)
            .WillByDefault(testing::Return(parsedFlightplan));

        EXPECT_TRUE(cone.AircraftIsExiting(*point, flightplan));
    }

    TEST_F(ExitConeTest, AircraftIsExitingFullCircleGoingNorth)
    {
        ExitCone cone(360, 360);
        const auto parsedFlightplan = std::make_shared<ParsedFlightplan>();
        const auto point = std::make_shared<FlightplanPoint>(1, "FOOO", mockCoordinate);
        parsedFlightplan->AddPoint(point);
        parsedFlightplan->AddPoint(std::make_shared<FlightplanPoint>(2, "BOOO", mockCoordinate2))

        ON_CALL(*mockCoordinate, DirectionTo(testing::Ref(*mockCoordinate2)))
            .WillByDefault(testing::Return(360.0));

        ON_CALL(flightplan, GetParsedFlightplan)
            .WillByDefault(testing::Return(parsedFlightplan));

        EXPECT_TRUE(cone.AircraftIsExiting(*point, flightplan));
    }

    TEST_F(ExitConeTest, AircraftIsExitingFullCircleNotNorth)
    {
        ExitCone cone(360, 360);
        const auto parsedFlightplan = std::make_shared<ParsedFlightplan>();
        const auto point = std::make_shared<FlightplanPoint>(1, "FOOO", mockCoordinate);
        parsedFlightplan->AddPoint(point);
        parsedFlightplan->AddPoint(std::make_shared<FlightplanPoint>(2, "BOOO", mockCoordinate2))

        ON_CALL(*mockCoordinate, DirectionTo(testing::Ref(*mockCoordinate2)))
            .WillByDefault(testing::Return(50));

        ON_CALL(flightplan, GetParsedFlightplan)
            .WillByDefault(testing::Return(parsedFlightplan));

        EXPECT_TRUE(cone.AircraftIsExiting(*point, flightplan));
    }
} // namespace UKControllerPluginTest::IntentionCode