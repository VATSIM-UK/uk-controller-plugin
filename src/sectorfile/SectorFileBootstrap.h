#pragma once
#include "bootstrap/PersistenceContainer.h"
#include "euroscope/AsrEventHandlerCollection.h"

namespace UKControllerPlugin {
    namespace SectorFile {

        void BootstrapRadarScreen(
            const UKControllerPlugin::Bootstrap::PersistenceContainer & container,
            UKControllerPlugin::Euroscope::AsrEventHandlerCollection & asrEvents
        );

    }  // namespace SectorFile
}  // namespace UKControllerPlugin
