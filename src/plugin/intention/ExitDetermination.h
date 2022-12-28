#pragma once

namespace UKControllerPlugin {
    namespace Euroscope {
        class EuroScopeCFlightPlanInterface;
    } // namespace Euroscope
    namespace Flightplan {
        class FlightplanPoint;
    } // namespace Flightplan
} // namespace UKControllerPlugin

namespace UKControllerPlugin::IntentionCode {

    /*
     * An interface for determining whether or not an aircraft is exiting an FIR here.
     */
    class ExitDetermination
    {
        public:
        virtual ~ExitDetermination() = default;
        [[nodiscard]] virtual auto AircraftIsExiting(
            const Flightplan::FlightplanPoint& flightplanPoint,
            Euroscope::EuroScopeCFlightPlanInterface& flightplan) const -> bool = 0;
    };
} // namespace UKControllerPlugin::IntentionCode
