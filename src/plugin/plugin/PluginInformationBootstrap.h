#pragma once
#include "bootstrap/PersistenceContainer.h"
#include "radarscreen/ConfigurableDisplayCollection.h"

namespace UKControllerPlugin {
    namespace Plugin {

        void BootstrapPluginInformationMessage(
            const UKControllerPlugin::Bootstrap::PersistenceContainer& container,
            UKControllerPlugin::RadarScreen::ConfigurableDisplayCollection& configurables);

    } // namespace Plugin
} // namespace UKControllerPlugin
