#pragma once
#include "bootstrap/PersistenceContainer.h"
#include "radarscreen/ConfigurableDisplayCollection.h"

namespace UKControllerPlugin {
    namespace Srd {

        extern const int srdDialogTagFunctionId;

        void BootstrapPlugin(UKControllerPlugin::Bootstrap::PersistenceContainer& container);

        void BootstrapRadarScreen(
            const UKControllerPlugin::Bootstrap::PersistenceContainer& container,
            UKControllerPlugin::RadarScreen::ConfigurableDisplayCollection & configurables
        );
    }  // namespace Srd
}  // UKControllerPlugin
