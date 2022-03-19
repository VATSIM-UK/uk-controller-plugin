#pragma once
#include "flightplan/FlightPlanEventHandlerInterface.h"

namespace UKControllerPlugin::Approach {
    class ApproachSequencer;

    class ApproachFlightplanEventHandler : public Flightplan::FlightPlanEventHandlerInterface
    {
        public:
        ApproachFlightplanEventHandler(ApproachSequencer& sequencer);
        void FlightPlanEvent(
            Euroscope::EuroScopeCFlightPlanInterface& flightPlan,
            Euroscope::EuroScopeCRadarTargetInterface& radarTarget) override;
        void FlightPlanDisconnectEvent(Euroscope::EuroScopeCFlightPlanInterface& flightPlan) override;
        void
        ControllerFlightPlanDataEvent(Euroscope::EuroScopeCFlightPlanInterface& flightPlan, int dataType) override{};

        private:
        // Sequences approaches
        ApproachSequencer& sequencer;
    };
} // namespace UKControllerPlugin::Approach
