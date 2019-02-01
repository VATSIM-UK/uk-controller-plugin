#include "pch/stdafx.h"
#include "hold/HoldEventHandler.h"
#include "euroscope/EuroscopePluginLoopbackInterface.h"
#include "euroscope/EuroScopeCFlightPlanInterface.h"
#include "euroscope//EuroScopeCRadarTargetInterface.h"
#include "hold/HoldManager.h"

using UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface;
using UKControllerPlugin::Euroscope::EuroScopeCRadarTargetInterface;
using UKControllerPlugin::Euroscope::EuroscopePluginLoopbackInterface;
using UKControllerPlugin::TimedEvent::AbstractTimedEvent;
using UKControllerPlugin::Hold::HoldManager;

namespace UKControllerPlugin {
    namespace Hold {

        HoldEventHandler::HoldEventHandler(HoldManager & holdManager, EuroscopePluginLoopbackInterface & plugin)
            : holdManager(holdManager), plugin(plugin)
        {

        }

        void HoldEventHandler::FlightPlanEvent(
            EuroScopeCFlightPlanInterface & flightPlan,
            EuroScopeCRadarTargetInterface & radarTarget
        ) {
            // Nothing to do here
        }

        /*
            When a flightplan disconnects, remove that aircraft from the hold.
        */
        void HoldEventHandler::FlightPlanDisconnectEvent(EuroScopeCFlightPlanInterface & flightPlan)
        {
            this->holdManager.RemoveAircraftFromAnyHold(flightPlan.GetCallsign());
        }

        void HoldEventHandler::ControllerFlightPlanDataEvent(
            EuroScopeCFlightPlanInterface & flightPlan,
            int dataType
        ) {
            // Nothing to do here
        }

        /*
            When the timed event is called, we should update the holding aircrafts details
        */
        void HoldEventHandler::TimedEventTrigger(void)
        {
            this->holdManager.UpdateHoldingAircraft(this->plugin);
        }
    }  // namespace Hold
}  // namespace UKControllerPlugin
