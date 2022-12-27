#pragma once

namespace UKControllerPlugin {
    namespace Euroscope {
        class EuroScopeCFlightPlanInterface;   
    } // namespace Euroscope
    namespace Flightplan {
        class FlightplanPoint;
    } // namespace Flightplan
}

namespace UKControllerPlugin::IntentionCode {
    class ExitDetermination
    {
        public:
            virtual [[nodiscard]] auto AircraftIsExiting(
                const Flightplan::FlightplanPoint& flightplanPoint,
                Euroscope::EuroScopeCFlightPlanInterface& flightplan
            ) = 0;
    }
} // namespace UKControllerPlugin::IntentionCode