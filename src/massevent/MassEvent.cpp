#include "pch/stdafx.h"
#include "massevent/MassEvent.h"
#include "flightplan/StoredFlightplanCollection.h"
#include "flightplan/StoredFlightplan.h"
#include "euroscope/EuroscopePluginLoopbackInterface.h"
#include "initialaltitude/InitialAltitudeEventHandler.h"

using UKControllerPlugin::Flightplan::StoredFlightplanCollection;
using UKControllerPlugin::Euroscope::EuroscopePluginLoopbackInterface;
using UKControllerPlugin::InitialAltitude::InitialAltitudeEventHandler;
using UKControllerPlugin::Squawk::SquawkEventHandler;

namespace UKControllerPlugin {
    namespace EventHandler {
        MassEvent::MassEvent(
            EuroscopePluginLoopbackInterface & pluginInterface,
            std::shared_ptr<InitialAltitudeEventHandler> initialAltitudes,
            const StoredFlightplanCollection & flightplans,
            std::shared_ptr<SquawkEventHandler> squawks
        )
            : pluginInterface(pluginInterface), flightplans(flightplans),
            initialAltitudes(initialAltitudes), squawks(squawks)
        {

        }

        /*
            Loops through every flightplan in the collection and triggers an "update" event.
        */
        void MassEvent::SetAllInitialAltitudes(void)
        {
            if (!this->initialAltitudes) {
                return;
            }

            LogInfo("Mass assigning initial altitudes");

            for (
                StoredFlightplanCollection::const_iterator it = this->flightplans.cbegin();
                it != this->flightplans.cend();
                ++it
            ) {
                try {

                    this->initialAltitudes->FlightPlanEvent(
                        *this->pluginInterface.GetFlightplanForCallsign(it->second->GetCallsign()),
                        *this->pluginInterface.GetRadarTargetForCallsign(it->second->GetCallsign())
                    );

                } catch (std::invalid_argument) {
                    continue;
                }
            }
        }

        /*
            Loops through all the flightplans and triggers an update event.
        */
        void MassEvent::SetAllSquawks(void)
        {
            if (!this->squawks) {
                return;
            }

            LogInfo("Mass assigning squawks");

            for (
                StoredFlightplanCollection::const_iterator it = this->flightplans.cbegin();
                it != this->flightplans.cend();
                ++it
            ) {
                try {

                    this->squawks->FlightPlanEvent(
                        *this->pluginInterface.GetFlightplanForCallsign(it->second->GetCallsign()),
                        *this->pluginInterface.GetRadarTargetForCallsign(it->second->GetCallsign())
                    );

                }
                catch (std::invalid_argument) {
                    continue;
                }
            }
        }
    }  // namespace EventHandler
}  // namespace UKControllerPlugin
