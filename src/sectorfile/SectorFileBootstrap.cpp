#pragma once
#include "pch/stdafx.h"
#include "sectorfile/SectorFileBootstrap.h"
#include "sectorfile/UserSectorFileTracker.h"
#include "sectorfile/SectorFileRunwayUpdater.h"

namespace UKControllerPlugin {
    namespace SectorFile {

        void BootstrapPlugin(UKControllerPlugin::Bootstrap::PersistenceContainer & container)
        {
            container.timedHandler->RegisterEvent(
                std::make_shared<UserSectorFileTracker>(*container.plugin, *container.sectorFileEventHandlers),
                1
            );

            container.sectorFileEventHandlers->AddHandler(
                std::make_shared<SectorFileRunwayUpdater>(*container.runways)
            );
        }
    }  // namespace SectorFile
}  // namespace UKControllerPlugin
