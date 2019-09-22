#include "pch/stdafx.h"
#include "sectorfile/SectorFileRunwayUpdater.h"

using UKControllerPlugin::Airfield::RunwayCollection;

namespace UKControllerPlugin {
    namespace SectorFile {

        SectorFileRunwayUpdater::SectorFileRunwayUpdater(RunwayCollection & runways)
            : runways(runways)
        {
        }

        void SectorFileRunwayUpdater::SectorFileUpdate(SectorFileProviderInterface & provider)
        {
        }

    }  // namespace SectorFile
}  // namespace UKControllerPlugin
