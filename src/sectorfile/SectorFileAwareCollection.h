#pragma once
#include "sectorfile/SectorFileAwareInterface.h"
#include "sectorfile/SectorFileProviderInterface.h"

namespace UKControllerPlugin {
    namespace SectorFile {

        /*
            A collection of objects that want to know
            when sector a file is updated or changed.
        */
        class SectorFileAwareCollection
        {
            public:

                void AddHandler(std::shared_ptr<UKControllerPlugin::SectorFile::SectorFileAwareInterface> handler);
                size_t CountHandlers(void) const;
                void SectorFileUpdate(UKControllerPlugin::SectorFile::SectorFileProviderInterface & provider) const;

            private:
                
                std::set<std::shared_ptr<UKControllerPlugin::SectorFile::SectorFileAwareInterface>> handlers;
        };

    }  // namespace Euroscope
}  // namespace UKControllerPlugin
