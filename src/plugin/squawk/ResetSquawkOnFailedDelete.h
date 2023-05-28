#pragma once
#include "SquawkAssignmentDeleteForConspicuityFailedEvent.h"
#include "euroscope/EuroscopePluginLoopbackInterface.h"
#include "eventhandler/EventHandler.h"

namespace UKControllerPlugin {
    namespace Euroscope {
        class EuroscopePluginLoopbackInterface;
    } // namespace Euroscope
    namespace Flightplan {
        class StoredFlightplanCollection;
    } // namespace Flightplan
} // namespace UKControllerPlugin

namespace UKControllerPlugin::Squawk {

    class ResetSquawkOnFailedDelete
        : public UKControllerPluginUtils::EventHandler::EventHandler<SquawkAssignmentDeleteForConspicuityFailedEvent>
    {
        public:
        ResetSquawkOnFailedDelete(
            Euroscope::EuroscopePluginLoopbackInterface& plugin,
            Flightplan::StoredFlightplanCollection& storedFlightplans);
        void OnEvent(const SquawkAssignmentDeleteForConspicuityFailedEvent& event) override;

        private:
        // For getting the flightplan to reset the squawk
        Euroscope::EuroscopePluginLoopbackInterface& plugin;

        // Stored flightplans
        Flightplan::StoredFlightplanCollection& storedFlightplans;
    };
} // namespace UKControllerPlugin::Squawk
