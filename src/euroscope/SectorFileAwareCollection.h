#pragma once
#include "euroscope/SectorFileAwareInterface.h"
#include "euroscope/SectorFileProviderInterface.h"

namespace UKControllerPlugin {
    namespace Euroscope {

        /*
            A collection of objects that want to know
            when sector a file is updated or changed.
        */
        class SectorFileAwareCollection
        {
            public:

                void AddHandler(std::shared_ptr<UKControllerPlugin::Euroscope::SectorFileAwareInterface> handler);
                size_t CountHandlers(void) const;
                void SectorFileUpdate(UKControllerPlugin::Euroscope::SectorFileProviderInterface & provider) const;

            private:
                
                std::set<std::shared_ptr<UKControllerPlugin::Euroscope::SectorFileAwareInterface>> handlers;
        };

    }  // namespace Euroscope
}  // namespace UKControllerPlugin
