#pragma once
#include "pch/stdafx.h"
#include "sectorfile/SectorFileBootstrap.h"
#include "sectorfile/RunwayCollection.h"

using UKControllerPlugin::Bootstrap::PersistenceContainer;
using UKControllerPlugin::Euroscope::AsrEventHandlerCollection;

namespace UKControllerPlugin {
    namespace SectorFile {

        void BootstrapRadarScreen(
            const PersistenceContainer & container,
            AsrEventHandlerCollection & asrEvents

        ) {
            // Set up the runway tracking in the sectorfile
            std::shared_ptr<RunwayCollection> runways = std::make_shared<RunwayCollection>(*container.plugin);
            asrEvents.RegisterHandler(runways);
            container.runwayDialogEventHandlers->AddHandler(runways);
        }
    }  // namespace SectorFile
}  // namespace UKControllerPlugin
