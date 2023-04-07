#pragma once

namespace UKControllerPlugin {
    namespace Euroscope {
        class EuroScopeCFlightPlanInterface;
        class EuroScopeCRadarTargetInterface;
    } // namespace Euroscope
} // namespace UKControllerPlugin

namespace UKControllerPlugin {
    namespace Flightplan {

        // Forward Declarations
        class FlightPlanEventHandlerInterface;
        // END

        /*
            A repository of event handlers for FlightPlan events. When an event is received, it will
            call each of the handlers in turn.
        */
        class FlightPlanEventHandlerCollection
        {
            public:
            int CountHandlers(void) const;
            void FlightPlanEvent(
                UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface& flightPlan,
                UKControllerPlugin::Euroscope::EuroScopeCRadarTargetInterface& radarTarget) const;
            void ControllerFlightPlanDataEvent(
                UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface& flightPlan,
                UKControllerPlugin::Euroscope::EuroScopeCRadarTargetInterface& radarTarget,
                int dataType) const;
            void
            FlightPlanDisconnectEvent(UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface& flightPlan) const;
            void RegisterHandler(std::shared_ptr<FlightPlanEventHandlerInterface> handler);

            private:
            // Vector of registered handlers
            std::list<std::shared_ptr<FlightPlanEventHandlerInterface>> handlerList;
        };
    } // namespace Flightplan
} // namespace UKControllerPlugin
