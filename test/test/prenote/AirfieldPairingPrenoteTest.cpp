#include "pch/pch.h"
#include "prenote/AirfieldPairingPrenote.h"
#include "mock/MockEuroScopeCFlightplanInterface.h"
#include "controller/ControllerPositionHierarchy.h"

using UKControllerPlugin::Prenote::AirfieldPairingPrenote;
using UKControllerPluginTest::Euroscope::MockEuroScopeCFlightPlanInterface;
using UKControllerPlugin::Controller::ControllerPositionHierarchy;
using ::testing::NiceMock;
using ::testing::Return;

namespace UKControllerPluginTest {
    namespace Prenote {

        TEST(AirfieldPairingPrenote, IsApplicableReturnsFalseWrongOrigin)
        {
            AirfieldPairingPrenote prenote(NULL, "EGKK", "EGHI");

            NiceMock<MockEuroScopeCFlightPlanInterface> flightplan;
            ON_CALL(flightplan, GetOrigin())
                .WillByDefault(Return("EGLL"));

            ON_CALL(flightplan, GetDestination())
                .WillByDefault(Return("EGHI"));

            EXPECT_FALSE(prenote.IsApplicable(flightplan));
        }

        TEST(AirfieldPairingPrenote, IsApplicableReturnsFalseWrongDestination)
        {
            AirfieldPairingPrenote prenote(NULL, "EGKK", "EGHI");

            NiceMock<MockEuroScopeCFlightPlanInterface> flightplan;
            ON_CALL(flightplan, GetOrigin())
                .WillByDefault(Return("EGKK"));

            ON_CALL(flightplan, GetDestination())
                .WillByDefault(Return("EGHH"));

            EXPECT_FALSE(prenote.IsApplicable(flightplan));
        }

        TEST(AirfieldPairingPrenote, IsApplicableReturnsTrueCorrectOriginAndDestination)
        {
            AirfieldPairingPrenote prenote(NULL, "EGKK", "EGHI");

            NiceMock<MockEuroScopeCFlightPlanInterface> flightplan;
            ON_CALL(flightplan, GetOrigin())
                .WillByDefault(Return("EGKK"));

            ON_CALL(flightplan, GetDestination())
                .WillByDefault(Return("EGHI"));

            EXPECT_TRUE(prenote.IsApplicable(flightplan));
        }

        TEST(AirfieldPairingPrenote, GetSummaryStringSummarisesThePrenote)
        {
            AirfieldPairingPrenote prenote(NULL, "EGKK", "EGHI");

            NiceMock<MockEuroScopeCFlightPlanInterface> flightplan;
            ON_CALL(flightplan, GetOrigin())
                .WillByDefault(Return("EGKK"));

            ON_CALL(flightplan, GetDestination())
                .WillByDefault(Return("EGHI"));

            EXPECT_TRUE("EGKK -> EGHI" == prenote.GetSummaryString());
        }
    }  // namespace Prenote
}  // namespace UKControllerPluginTest
