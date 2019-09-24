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
            asrEvents.RegisterHandler(
                std::make_shared<RunwayCollection>(*container.plugin)
            );
        }
    }  // namespace SectorFile
}  // namespace UKControllerPlugin
