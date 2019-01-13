#pragma once
#include "timedevent/AbstractTimedEvent.h"
#include "flightplan/FlightPlanEventHandlerInterface.h"

namespace UKControllerPlugin {
    namespace Euroscope {
        class EuroScopeCFlightPlanInterface;
        class EuroScopeCRadarTargetInterface;
    }  // namespace Euroscope
    namespace Flightplan {
        class StoredFlightplanCollection;
    }  // namespace Flightplan
}  // namespace UKControllerPlugin

namespace UKControllerPlugin {
    namespace Flightplan {

        /*
            A class for processing events in relations to flightplan details
            that have been stored in the plugin.
        */
        class StoredFlightplanEventHandler : public UKControllerPlugin::Flightplan::FlightPlanEventHandlerInterface,
            public UKControllerPlugin::TimedEvent::AbstractTimedEvent
        {
            public:
                explicit StoredFlightplanEventHandler(
                    UKControllerPlugin::Flightplan::StoredFlightplanCollection & storedFlightplans
                );
                void ControllerFlightPlanDataEvent(
                    UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface & flightPlan,
                    int dataType
                );
                void FlightPlanEvent(
                    UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface & euroscopeFlightplan,
                    UKControllerPlugin::Euroscope::EuroScopeCRadarTargetInterface & radarTarget
                );
                void FlightPlanDisconnectEvent(
                    UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface & euroscopeFlightplan
                );
                void TimedEventTrigger(void);

            private:

                // Stored flightplans
                UKControllerPlugin::Flightplan::StoredFlightplanCollection & storedFlightplans;

                // The timeout value we set for flightplan timeout on disconnect - 10 minutes.
                const int flightplanTimeout = 600;
        };
    }  // namespace Flightplan
}  // namespace UKControllerPlugin
