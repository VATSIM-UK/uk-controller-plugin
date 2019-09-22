#include "pch/stdafx.h"
#include "sectorfile/SectorFileRunwayUpdater.h"
#include "euroscope/EuroscopeSectorFileElementInterface.h"
#include "airfield/Runway.h"

using UKControllerPlugin::Airfield::RunwayCollection;
using UKControllerPlugin::Euroscope::EuroscopeSectorFileElementInterface;
using UKControllerPlugin::Airfield::Runway;

namespace UKControllerPlugin {
    namespace SectorFile {

        SectorFileRunwayUpdater::SectorFileRunwayUpdater(RunwayCollection & runways)
            : runways(runways)
        {
        }

        void SectorFileRunwayUpdater::SectorFileUpdate(SectorFileProviderInterface & provider)
        {
            std::set<std::shared_ptr<EuroscopeSectorFileElementInterface>> runwayElements =
                provider.GetAllElementsByType(EuroScopePlugIn::SECTOR_ELEMENT_RUNWAY);

            runways.Clear();

            for (
                std::set<std::shared_ptr<EuroscopeSectorFileElementInterface>>::const_iterator
                    it = runwayElements.cbegin();
                it != runwayElements.cend();
                ++it
            ) {
                runways.AddRunway(
                    std::make_shared<Runway>(
                        (*it)->Runway1ComponentName(),
                        (*it)->Airport(),
                        (*it)->Runway1Identifier(),
                        (*it)->Runway1Heading(),
                        (*it)->Runway1ActiveForDepartures(),
                        (*it)->Runway1ActiveForArrivals()
                    )
                );

                runways.AddRunway(
                    std::make_shared<Runway>(
                        (*it)->Runway2ComponentName(),
                        (*it)->Airport(),
                        (*it)->Runway2Identifier(),
                        (*it)->Runway2Heading(),
                        (*it)->Runway1ActiveForDepartures(),
                        (*it)->Runway1ActiveForArrivals()
                    )
                );
            }

        }

    }  // namespace SectorFile
}  // namespace UKControllerPlugin
