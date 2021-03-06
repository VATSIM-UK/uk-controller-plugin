#pragma once
#include "bootstrap/PersistenceContainer.h"
#include "radarscreen/ConfigurableDisplayCollection.h"

namespace UKControllerPlugin {
    namespace Notifications {
        void BootstrapPlugin(Bootstrap::PersistenceContainer& container);
        void BootstrapRadarScreen(RadarScreen::ConfigurableDisplayCollection & displays);
    }  // namespace Notifications
}  // namespace UKControllerPlugin
