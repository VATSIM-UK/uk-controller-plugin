#pragma once
#include "ExitDetermination.h"

namespace UKControllerPlugin::IntentionCode {
    class ExitCone : public ExitDetermination
    {
        public:
        ExitCone(int startDirection, int endDirection);
        [[nodiscard]] auto AircraftIsExiting(
            const Flightplan::FlightplanPoint& flightplanPoint,
            const Euroscope::EuroScopeCFlightPlanInterface& flightplan) const -> bool override;
        [[nodiscard]] auto StartDirection() const -> int;
        [[nodiscard]] auto EndDirection() const -> int;

        private:
        // The start of the exit direction cone
        int startDirection;

        // The end of the exit direction cone, is clockwise from the start
        int endDirection;
    };
} // namespace UKControllerPlugin::IntentionCode
