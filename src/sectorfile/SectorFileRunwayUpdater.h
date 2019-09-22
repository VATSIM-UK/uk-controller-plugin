#pragma once
#include "sectorfile/SectorFileAwareInterface.h"
#include "airfield/RunwayCollection.h"
#include "euroscope/EuroscopeSectorFileElementInterface.h"

namespace UKControllerPlugin {
    namespace SectorFile {

        /*
            Updates the runways every time the sector file is updated.
        */
        class SectorFileRunwayUpdater : public UKControllerPlugin::SectorFile::SectorFileAwareInterface
        {
            public:
                SectorFileRunwayUpdater(UKControllerPlugin::Airfield::RunwayCollection & runways);

                // Inherited via SectorFileAwareInterface
                virtual void SectorFileUpdate(SectorFileProviderInterface & provider) override;

            private:
                
                // The collection of runways that we need to update
                UKControllerPlugin::Airfield::RunwayCollection & runways;
        };
    }  // namespace SectorFile
}  // namespace UKControllerPlugin
