#pragma once

// Forward declare
namespace UKControllerPlugin {
    namespace Euroscope {
        class EuroScopeCRadarTargetInterface;
        class EuroScopeCFlightPlanInterface;
    } // namespace Euroscope
} // namespace UKControllerPlugin
// END

namespace UKControllerPlugin {
    namespace Flightplan {

        /*
            An interface for handling events related to flightplans.
        */
        class FlightPlanEventHandlerInterface
        {
            public:
            virtual ~FlightPlanEventHandlerInterface(void)
            {
            } // namespace Flightplan
            virtual void FlightPlanEvent(
                UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface& flightPlan,
                UKControllerPlugin::Euroscope::EuroScopeCRadarTargetInterface& radarTarget) = 0;
            virtual void
            FlightPlanDisconnectEvent(UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface& flightPlan) = 0;
            virtual void ControllerFlightPlanDataEvent(
                UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface& flightPlan, int dataType) = 0;
        };
    } // namespace Flightplan
} // namespace UKControllerPlugin
