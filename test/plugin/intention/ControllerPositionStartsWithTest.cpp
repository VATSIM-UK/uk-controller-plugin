#include "controller/ActiveCallsign.h"
#include "controller/ActiveCallsignCollection.h"
#include "controller/ControllerPosition.h"
#include "intention/ControllerPositionStartsWith.h"

using UKControllerPlugin::Controller::ActiveCallsign;
using UKControllerPlugin::Controller::ActiveCallsignCollection;
using UKControllerPlugin::Controller::ControllerPosition;
using UKControllerPlugin::IntentionCode::Condition;
using UKControllerPlugin::IntentionCode::ControllerPositionStartsWith;

namespace UKControllerPluginTest::IntentionCode {
    class ControllerPositionStartsWithTest : public testing::Test
    {
        public:
        ControllerPositionStartsWithTest()
            : position1(1, "LON_S_CTR", 129.420, std::vector<std::string>{}, true, false),
              position2(2, "SCO_CTR", 129.420, std::vector<std::string>{}, true, false), controllers(callsigns, "LON")
        {
        }

        ControllerPosition position1;
        ControllerPosition position2;
        ActiveCallsignCollection callsigns;
        testing::NiceMock<Euroscope::MockEuroScopeCFlightPlanInterface> flightplan;
        testing::NiceMock<Euroscope::MockEuroScopeCRadarTargetInterface> radarTarget;
        ControllerPositionStartsWith controllers;
    };

    TEST_F(ControllerPositionStartsWithTest, ItFailsIfUserNotLoggedIn)
    {
        EXPECT_FALSE(controllers.Passes(flightplan, radarTarget));
    }

    TEST_F(ControllerPositionStartsWithTest, ItFailsIfNotUserOnMatchingPosition)
    {
        ActiveCallsign callsign("LON_S_CTR", "Test", position1, false);
        callsigns.AddCallsign(callsign);

        EXPECT_FALSE(controllers.Passes(flightplan, radarTarget));
    }

    TEST_F(ControllerPositionStartsWithTest, ItFailsIfUserNotOnMatchingPosition)
    {
        ActiveCallsign callsign("LON_S_CTR", "Test", position2, true);
        callsigns.AddUserCallsign(callsign);

        EXPECT_FALSE(controllers.Passes(flightplan, radarTarget));
    }

    TEST_F(ControllerPositionStartsWithTest, ItPassesIfUserOnMatchingPosition)
    {
        ActiveCallsign callsign("LON_S_CTR", "Test", position1, true);
        callsigns.AddUserCallsign(callsign);

        EXPECT_TRUE(controllers.Passes(flightplan, radarTarget));
    }
} // namespace UKControllerPluginTest::IntentionCode
