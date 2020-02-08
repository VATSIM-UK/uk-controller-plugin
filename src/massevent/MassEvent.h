#pragma once
#include "squawk/SquawkEventHandler.h"

namespace UKControllerPlugin {
    namespace Flightplan {
        class FlightplanCollection;
    }  // namespace Flightplan
    namespace InitialAltitude {
        class InitialAltitudeEventHandler;
    }  // namespace InitialAltitude
    namespace Euroscope {
        class EuroscopePluginLoopbackInterface;
    }  // namespace Euroscope
}  // namespace UKControllerPlugin

namespace UKControllerPlugin {
    namespace EventHandler {

        /*
            A class for running events in bulk across entire collections, for example
            assigning squawks in bulk.
        */
        class MassEvent
        {
            public:
                MassEvent(
                    UKControllerPlugin::Euroscope::EuroscopePluginLoopbackInterface & pluginInterface,
                    std::shared_ptr<UKControllerPlugin::InitialAltitude::InitialAltitudeEventHandler> initialAltitudes,
                    const UKControllerPlugin::Flightplan::StoredFlightplanCollection & flightplans
                );

                void SetAllInitialAltitudes(void);

            private:
                // A way to get things we need from Euroscope.
                UKControllerPlugin::Euroscope::EuroscopePluginLoopbackInterface & pluginInterface;

                // Event handler for initial altitudes
                std::shared_ptr<UKControllerPlugin::InitialAltitude::InitialAltitudeEventHandler> initialAltitudes;

                // The flightplan repository so we can iterate it.
                const UKControllerPlugin::Flightplan::StoredFlightplanCollection & flightplans;

        };
    }  // namespace EventHandler
}  // namespace UKControllerPlugin
