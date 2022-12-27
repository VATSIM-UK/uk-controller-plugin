#pragma once
#include "ExitDetermination.h"

namespace UKControllerPlugin::IntentionCode {
    class ExitCone : public ExitDetermination
    {
        public:
            ExitCone(int startDirection, int endDirection);
            [[nodiscard]] auto AircraftIsExiting(
                const Flightplan::FlightplanPoint& flightplanPoint,
                Euroscope::EuroScopeCFlightPlanInterface& flightplan
            ) override;

        private:
        // The start of the exit direction cone
        int exitDirectionStart;

        // The end of the exit direction cone, is clockwise from the start
        int exitDirectionEnd;
    }
} // namespace UKControllerPlugin::IntentionCode