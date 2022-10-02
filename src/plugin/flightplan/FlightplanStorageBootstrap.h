#pragma once
#include "bootstrap/PersistenceContainer.h"

namespace UKControllerPlugin {
    namespace Flightplan {

        /*
            A class for bootstrapping the storage of flightplans - event handlers.
        */
        class FlightplanStorageBootstrap
        {
            public:
            static void BootstrapPlugin(const UKControllerPlugin::Bootstrap::PersistenceContainer& container);

            // How often the timed event for the handler should be triggered
            static const int timedEventFrequency = 60;
        };
    } // namespace Flightplan
} // namespace UKControllerPlugin
