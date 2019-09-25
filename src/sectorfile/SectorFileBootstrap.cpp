#pragma once
#include "pch/stdafx.h"
#include "sectorfile/SectorFileBootstrap.h"
#include "sectorfile/RunwayCollection.h"

using UKControllerPlugin::Bootstrap::PersistenceContainer;
using UKControllerPlugin::Euroscope::AsrEventHandlerCollection;

namespace UKControllerPlugin {
    namespace SectorFile {

        void BootstrapPlugin(PersistenceContainer & container)
        {
            container.runways = std::make_shared<RunwayCollection>(*container.plugin);
            container.runwayDialogEventHandlers->AddHandler(container.runways);
        }

        void BootstrapRadarScreen(
            const PersistenceContainer & container,
            AsrEventHandlerCollection & asrEvents

        ) {
            // Register the runway collection to be updated when the ASR loads
            asrEvents.RegisterHandler(container.runways);
        }
    }  // namespace SectorFile
}  // namespace UKControllerPlugin
