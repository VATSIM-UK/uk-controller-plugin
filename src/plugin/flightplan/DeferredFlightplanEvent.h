#pragma once
#include "timedevent/DeferredEventRunnerInterface.h"

namespace UKControllerPlugin {
    namespace Euroscope {
        class EuroscopePluginLoopbackInterface;
    } // namespace Euroscope
    namespace Flightplan {
        class FlightPlanEventHandlerInterface;
    } // namespace Flightplan
} // namespace UKControllerPlugin

namespace UKControllerPlugin {
    namespace Flightplan {

        class DeferredFlightPlanEvent : public UKControllerPlugin::TimedEvent::DeferredEventRunnerInterface
        {
            public:
            DeferredFlightPlanEvent(
                UKControllerPlugin::Flightplan::FlightPlanEventHandlerInterface& handler,
                UKControllerPlugin::Euroscope::EuroscopePluginLoopbackInterface& plugin,
                std::string callsign);
            void Run(void) override;

            private:
            // The callsign to do the event for
            const std::string callsign;

            // The event handler this is for
            UKControllerPlugin::Flightplan::FlightPlanEventHandlerInterface& handler;

            // The plugin, so we can get flightplans
            UKControllerPlugin::Euroscope::EuroscopePluginLoopbackInterface& plugin;
        };
    } // namespace Flightplan
} // namespace UKControllerPlugin
