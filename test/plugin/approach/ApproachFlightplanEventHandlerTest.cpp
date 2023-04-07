#include "approach/ApproachFlightplanEventHandler.h"
#include "approach/ApproachSequence.h"
#include "approach/ApproachSequencer.h"

using UKControllerPlugin::Approach::ApproachFlightplanEventHandler;
using UKControllerPlugin::Approach::ApproachSequencer;
using UKControllerPlugin::Approach::ApproachSequencingMode;

namespace UKControllerPluginTest::Approach {
    class ApproachFlightplanEventHandlerTest : public testing::Test
    {
        public:
        ApproachFlightplanEventHandlerTest() : handler(sequencer)
        {
            ON_CALL(flightplan, GetCallsign).WillByDefault(testing::Return("BAW456"));
            ON_CALL(flightplan, GetDestination).WillByDefault(testing::Return("EGKK"));

            sequencer.AddAircraftToSequence("EGKK", "BAW123", ApproachSequencingMode::WakeTurbulence);
            sequencer.AddAircraftToSequence("EGKK", "BAW456", ApproachSequencingMode::WakeTurbulence);
        }

        testing::NiceMock<Euroscope::MockEuroScopeCFlightPlanInterface> flightplan;
        testing::NiceMock<Euroscope::MockEuroScopeCRadarTargetInterface> radarTarget;
        ApproachSequencer sequencer;
        ApproachFlightplanEventHandler handler;
    };

    TEST_F(ApproachFlightplanEventHandlerTest, ItRemovesFromSequencerOnDisconnect)
    {
        handler.FlightPlanDisconnectEvent(flightplan);
        EXPECT_TRUE(sequencer.AirfieldForAircraft("BAW456").empty());
        EXPECT_EQ(std::list<std::string>({"BAW123"}), sequencer.GetForAirfield("EGKK").Callsigns());
    }

    TEST_F(ApproachFlightplanEventHandlerTest, ItRemovesFromSequencerOnDestinationChange)
    {
        ON_CALL(flightplan, GetDestination).WillByDefault(testing::Return("EGLL"));
        handler.FlightPlanEvent(flightplan, radarTarget);
        EXPECT_TRUE(sequencer.AirfieldForAircraft("BAW456").empty());
        EXPECT_EQ(std::list<std::string>({"BAW123"}), sequencer.GetForAirfield("EGKK").Callsigns());
    }

    TEST_F(ApproachFlightplanEventHandlerTest, ItDoesntRemoveFromSequencerOnSameDestination)
    {
        handler.FlightPlanEvent(flightplan, radarTarget);
        EXPECT_EQ("EGKK", sequencer.AirfieldForAircraft("BAW456"));
        EXPECT_EQ(std::list<std::string>({"BAW123", "BAW456"}), sequencer.GetForAirfield("EGKK").Callsigns());
    }

    TEST_F(ApproachFlightplanEventHandlerTest, ItDoesntRemoveFromSequencerIfNotInSequence)
    {
        sequencer.RemoveAircraftFromSequences("BAW456");
        handler.FlightPlanEvent(flightplan, radarTarget);
        EXPECT_TRUE(sequencer.AirfieldForAircraft("BAW456").empty());
        EXPECT_EQ(std::list<std::string>({"BAW123"}), sequencer.GetForAirfield("EGKK").Callsigns());
    }
} // namespace UKControllerPluginTest::Approach
