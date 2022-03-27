#include "ApproachFlightplanEventHandler.h"
#include "ApproachSequencer.h"
#include "euroscope/EuroScopeCFlightPlanInterface.h"

namespace UKControllerPlugin::Approach {

    ApproachFlightplanEventHandler::ApproachFlightplanEventHandler(ApproachSequencer& sequencer) : sequencer(sequencer)
    {
    }

    void ApproachFlightplanEventHandler::FlightPlanEvent(
        Euroscope::EuroScopeCFlightPlanInterface& flightPlan, Euroscope::EuroScopeCRadarTargetInterface& radarTarget)
    {
        const auto& airfield = sequencer.AirfieldForAircraft(flightPlan.GetCallsign());
        if (airfield.empty()) {
            return;
        }

        if (airfield != flightPlan.GetDestination()) {
            sequencer.RemoveAircraftFromSequences(flightPlan.GetCallsign());
        }
    }

    void ApproachFlightplanEventHandler::FlightPlanDisconnectEvent(Euroscope::EuroScopeCFlightPlanInterface& flightPlan)
    {
        sequencer.RemoveAircraftFromSequences(flightPlan.GetCallsign());
    }
} // namespace UKControllerPlugin::Approach
