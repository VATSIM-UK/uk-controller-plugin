#pragma once
#include "sectorfile/SectorFileProviderInterface.h"
#include "sectorfile/SectorFileAwareCollection.h"
#include "timedevent/AbstractTimedEvent.h"

namespace UKControllerPlugin {
    namespace SectorFile {

        class UserSectorFileTracker : public UKControllerPlugin::TimedEvent::AbstractTimedEvent
        {
            public:
                UserSectorFileTracker(
                    UKControllerPlugin::SectorFile::SectorFileProviderInterface & provider,
                    const UKControllerPlugin::SectorFile::SectorFileAwareCollection & handlers
                );
                std::string LoadedSectorFile(void) const;


                // Inherited via AbstractTimedEvent
                void TimedEventTrigger(void) override;

            private:
                
                // Provides the sector file
                UKControllerPlugin::SectorFile::SectorFileProviderInterface & provider;

                // Instances that want to know if the users sector file changes
                const UKControllerPlugin::SectorFile::SectorFileAwareCollection & handlers;

                // The currently loaded sectorfile
                std::string loadedSectorFile = "";
        };
    }  // namespace Euroscope
}  // namespace UKControllerPlugin
