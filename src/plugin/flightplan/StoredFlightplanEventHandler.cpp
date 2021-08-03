#include "pch/pch.h"
#include "flightplan/StoredFlightplanEventHandler.h"
#include "flightplan/StoredFlightplan.h"
#include "euroscope/EuroScopeCFlightPlanInterface.h"
#include "euroscope/EuroScopeCRadarTargetInterface.h"
#include "flightplan/StoredFlightplanCollection.h"


using UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface;
using UKControllerPlugin::Euroscope::EuroScopeCRadarTargetInterface;
using UKControllerPlugin::Flightplan::StoredFlightplan;

namespace UKControllerPlugin {
    namespace Flightplan {

        StoredFlightplanEventHandler::StoredFlightplanEventHandler(StoredFlightplanCollection & storedFlightplans)
            : storedFlightplans(storedFlightplans)
        {

        }
        /*
            Called if controller assigned data is changed.
        */
        void StoredFlightplanEventHandler::ControllerFlightPlanDataEvent(
            EuroScopeCFlightPlanInterface & euroscopeFlightplan,
            int dataType
        ) {
            // Nothing to do here
        }

        void StoredFlightplanEventHandler::FlightPlanEvent(
            EuroScopeCFlightPlanInterface & euroscopeFlightplan,
            EuroScopeCRadarTargetInterface & radarTarget
        ) {
            this->storedFlightplans.UpdatePlan(StoredFlightplan(euroscopeFlightplan));

            // Reset anything to do with timeout.
            this->storedFlightplans.GetFlightplanForCallsign(euroscopeFlightplan.GetCallsign()).ResetTimeout();
        }

        /*
            Called when a flightplan disconnects.
        */
        void StoredFlightplanEventHandler::FlightPlanDisconnectEvent(
            EuroScopeCFlightPlanInterface & euroscopeFlightplan
        ) {
            if (!this->storedFlightplans.HasFlightplanForCallsign(euroscopeFlightplan.GetCallsign())) {
                return;
            }

            this->storedFlightplans.GetFlightplanForCallsign(
                euroscopeFlightplan.GetCallsign()
            ).SetTimeout(this->flightplanTimeout);
        }

        /*
            When the timed event triggers, remove any flightplans that are deemed to have timed out from
            storage.
        */
        void StoredFlightplanEventHandler::TimedEventTrigger(void)
        {
            this->storedFlightplans.RemoveTimedOutPlans();
        }
    }  // namespace Flightplan
}  // namespace UKControllerPlugin
