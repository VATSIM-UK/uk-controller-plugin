#include "prenote/AirfieldPairingPrenote.h"
#include "controller/ControllerPositionHierarchy.h"

using ::testing::NiceMock;
using ::testing::Return;
using UKControllerPlugin::Controller::ControllerPositionHierarchy;
using UKControllerPlugin::Prenote::AirfieldPairingPrenote;
using UKControllerPluginTest::Euroscope::MockEuroScopeCFlightPlanInterface;

namespace UKControllerPluginTest {
    namespace Prenote {

        TEST(AirfieldPairingPrenote, IsApplicableReturnsFalseWrongOrigin)
        {
            AirfieldPairingPrenote prenote(nullptr, "EGKK", "EGHI", "");

            NiceMock<MockEuroScopeCFlightPlanInterface> flightplan;
            ON_CALL(flightplan, GetOrigin()).WillByDefault(Return("EGLL"));

            ON_CALL(flightplan, GetDestination()).WillByDefault(Return("EGHI"));

            ON_CALL(flightplan, GetFlightRules()).WillByDefault(Return("ABC"));

            EXPECT_FALSE(prenote.IsApplicable(flightplan));
        }

        TEST(AirfieldPairingPrenote, IsApplicableReturnsFalseWrongDestination)
        {
            AirfieldPairingPrenote prenote(nullptr, "EGKK", "EGHI", "");

            NiceMock<MockEuroScopeCFlightPlanInterface> flightplan;
            ON_CALL(flightplan, GetOrigin()).WillByDefault(Return("EGKK"));

            ON_CALL(flightplan, GetDestination()).WillByDefault(Return("EGHH"));

            ON_CALL(flightplan, GetFlightRules()).WillByDefault(Return("ABC"));

            EXPECT_FALSE(prenote.IsApplicable(flightplan));
        }

        TEST(AirfieldPairingPrenote, IsApplicableReturnsTrueWrongRules)
        {
            AirfieldPairingPrenote prenote(nullptr, "EGKK", "EGHI", "V");

            NiceMock<MockEuroScopeCFlightPlanInterface> flightplan;
            ON_CALL(flightplan, GetOrigin()).WillByDefault(Return("EGKK"));

            ON_CALL(flightplan, GetFlightRules()).WillByDefault(Return("I"));

            ON_CALL(flightplan, GetDestination()).WillByDefault(Return("EGHI"));

            EXPECT_FALSE(prenote.IsApplicable(flightplan));
        }

        TEST(AirfieldPairingPrenote, IsApplicableReturnsTrueCorrectOriginAndDestination)
        {
            AirfieldPairingPrenote prenote(nullptr, "EGKK", "EGHI", "");

            NiceMock<MockEuroScopeCFlightPlanInterface> flightplan;
            ON_CALL(flightplan, GetOrigin()).WillByDefault(Return("EGKK"));

            ON_CALL(flightplan, GetFlightRules()).WillByDefault(Return("ABC"));

            ON_CALL(flightplan, GetDestination()).WillByDefault(Return("EGHI"));

            EXPECT_TRUE(prenote.IsApplicable(flightplan));
        }

        TEST(AirfieldPairingPrenote, IsApplicableReturnsTrueCorrectOriginDestinationAndRules)
        {
            AirfieldPairingPrenote prenote(nullptr, "EGKK", "EGHI", "I");

            NiceMock<MockEuroScopeCFlightPlanInterface> flightplan;
            ON_CALL(flightplan, GetOrigin()).WillByDefault(Return("EGKK"));

            ON_CALL(flightplan, GetFlightRules()).WillByDefault(Return("I"));

            ON_CALL(flightplan, GetDestination()).WillByDefault(Return("EGHI"));

            EXPECT_TRUE(prenote.IsApplicable(flightplan));
        }

        TEST(AirfieldPairingPrenote, GetSummaryStringSummarisesThePrenote)
        {
            AirfieldPairingPrenote prenote(nullptr, "EGKK", "EGHI", "");
            EXPECT_TRUE("EGKK -> EGHI" == prenote.GetSummaryString());
        }

        TEST(AirfieldPairingPrenote, GetFlightRulesReturnsRules)
        {
            AirfieldPairingPrenote prenote(nullptr, "EGKK", "EGHI", "I");
            EXPECT_TRUE("I" == prenote.GetFlightRules());
        }
    } // namespace Prenote
} // namespace UKControllerPluginTest
