#include "flightplan/FlightplanPoint.h"
#include "flightplan/ParsedFlightplan.h"
#include "intention/RoutingVia.h"

using UKControllerPlugin::Flightplan::FlightplanPoint;
using UKControllerPlugin::Flightplan::ParsedFlightplan;
using UKControllerPlugin::IntentionCode::Condition;
using UKControllerPlugin::IntentionCode::RoutingVia;

namespace UKControllerPluginTest::IntentionCode {
    class RoutingViaTest : public testing::Test
    {
        public:
        RoutingViaTest()
            : coordinate(std::make_shared<testing::NiceMock<Euroscope::MockEuroscopeCoordinateInterface>>()),
              parsedFlightplan(std::make_shared<ParsedFlightplan>()), routingVia("KOK")
        {
            ON_CALL(flightplan, GetParsedFlightplan).WillByDefault(testing::Return(parsedFlightplan));
        }

        std::shared_ptr<testing::NiceMock<Euroscope::MockEuroscopeCoordinateInterface>> coordinate;
        std::shared_ptr<ParsedFlightplan> parsedFlightplan;
        testing::NiceMock<Euroscope::MockEuroScopeCFlightPlanInterface> flightplan;
        testing::NiceMock<Euroscope::MockEuroScopeCRadarTargetInterface> radarTarget;
        RoutingVia routingVia;
    };

    TEST_F(RoutingViaTest, ItFailsIfNotRoutingViaPoint)
    {
        parsedFlightplan->AddPoint(std::make_shared<FlightplanPoint>(0, "KONAN", coordinate));
        parsedFlightplan->AddPoint(std::make_shared<FlightplanPoint>(1, "VABIK", coordinate));

        EXPECT_FALSE(routingVia.Passes(flightplan, radarTarget));
    }

    TEST_F(RoutingViaTest, ItPassesIfRoutingViaPoint)
    {
        parsedFlightplan->AddPoint(std::make_shared<FlightplanPoint>(0, "KONAN", coordinate));
        parsedFlightplan->AddPoint(std::make_shared<FlightplanPoint>(1, "KOK", coordinate));
        parsedFlightplan->AddPoint(std::make_shared<FlightplanPoint>(2, "VABIK", coordinate));

        EXPECT_TRUE(routingVia.Passes(flightplan, radarTarget));
    }
} // namespace UKControllerPluginTest::IntentionCode
