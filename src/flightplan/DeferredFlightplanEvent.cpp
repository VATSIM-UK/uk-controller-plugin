#pragma once
#include "pch/stdafx.h"
#include "flightplan/DeferredFlightplanEvent.h"
#include "euroscope/EuroScopeCFlightPlanInterface.h"
#include "euroscope/EuroScopeCRadarTargetInterface.h"
#include "euroscope/EuroscopePluginLoopbackInterface.h"
#include "flightplan/FlightPlanEventHandlerInterface.h"

using UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface;
using UKControllerPlugin::Euroscope::EuroScopeCRadarTargetInterface;
using UKControllerPlugin::Euroscope::EuroscopePluginLoopbackInterface;
using UKControllerPlugin::Flightplan::FlightPlanEventHandlerInterface;

namespace UKControllerPlugin {
    namespace Flightplan {
        DeferredFlightPlanEvent::DeferredFlightPlanEvent(
            FlightPlanEventHandlerInterface & handler,
            EuroscopePluginLoopbackInterface & plugin,
            std::string callsign
        )
            : handler(handler), plugin(plugin), callsign(callsign)
        {

        }

        /*
            Get the flightplan afresh from ES and run the event
        */
        void DeferredFlightPlanEvent::Run(void)
        {
            try {
                std::shared_ptr<EuroScopeCFlightPlanInterface> currentFlightplan =
                    this->plugin.GetFlightplanForCallsign(callsign);
                std::shared_ptr<EuroScopeCRadarTargetInterface> currentRadarTarget =
                    this->plugin.GetRadarTargetForCallsign(callsign);

                this->handler.FlightPlanEvent(*currentFlightplan, *currentRadarTarget);
            } catch (std::invalid_argument) {
                // If we cant find the ES data, then nothing we can do.
            }
        }

    }  // namespace Flightplan
}  // namespace UKControllerPlugin
