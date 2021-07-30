#include "pch/pch.h"
#include "prenote/DeparturePrenote.h"
#include "mock/MockEuroScopeCFlightplanInterface.h"
#include "controller/ControllerPositionHierarchy.h"

using UKControllerPlugin::Prenote::DeparturePrenote;
using UKControllerPluginTest::Euroscope::MockEuroScopeCFlightPlanInterface;
using UKControllerPlugin::Controller::ControllerPositionHierarchy;
using ::testing::NiceMock;
using ::testing::Return;

namespace UKControllerPluginTest {
    namespace Prenote {

        TEST(DeparturePrenote, IsApplicableReturnsFalseWrongAirfield)
        {
            DeparturePrenote prenote(NULL, "EGKK", "SAM1X");

            NiceMock<MockEuroScopeCFlightPlanInterface> flightplan;
            ON_CALL(flightplan, GetOrigin())
                .WillByDefault(Return("EGLL"));

            ON_CALL(flightplan, GetSidName())
                .WillByDefault(Return("SAM1X"));

            EXPECT_FALSE(prenote.IsApplicable(flightplan));
        }

        TEST(DeparturePrenote, IsApplicableReturnsFalseWrongDeparture)
        {
            DeparturePrenote prenote(NULL, "EGKK", "SAM1X");

            NiceMock<MockEuroScopeCFlightPlanInterface> flightplan;
            ON_CALL(flightplan, GetOrigin())
                .WillByDefault(Return("EGKK"));

            ON_CALL(flightplan, GetSidName())
                .WillByDefault(Return("SAM2X"));

            EXPECT_FALSE(prenote.IsApplicable(flightplan));
        }

        TEST(DeparturePrenote, IsApplicableReturnsTrueCorrectIcaoAndDeparture)
        {
            DeparturePrenote prenote(NULL, "EGKK", "SAM1X");

            NiceMock<MockEuroScopeCFlightPlanInterface> flightplan;
            ON_CALL(flightplan, GetOrigin())
                .WillByDefault(Return("EGKK"));

            ON_CALL(flightplan, GetSidName())
                .WillByDefault(Return("SAM1X"));

            EXPECT_TRUE(prenote.IsApplicable(flightplan));
        }

        TEST(DeparturePrenote, GetSummaryStringSummarisesThePrenote)
        {
            DeparturePrenote prenote(NULL, "EGKK", "SAM1X");

            NiceMock<MockEuroScopeCFlightPlanInterface> flightplan;
            ON_CALL(flightplan, GetOrigin())
                .WillByDefault(Return("EGKK"));

            ON_CALL(flightplan, GetSidName())
                .WillByDefault(Return("SAM1X"));

            EXPECT_TRUE("EGKK, SAM1X" == prenote.GetSummaryString());
        }
    }  // namespace Prenote
}  // namespace UKControllerPluginTest
