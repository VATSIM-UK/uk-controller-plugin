#pragma once
#include "euroscope/SectorFileProviderInterface.h"
#include "euroscope/SectorFileAwareCollection.h"
#include "timedevent/AbstractTimedEvent.h"

namespace UKControllerPlugin {
    namespace Euroscope {

        class UserSectorFileTracker : public UKControllerPlugin::TimedEvent::AbstractTimedEvent
        {
            public:
                UserSectorFileTracker(
                    UKControllerPlugin::Euroscope::SectorFileProviderInterface & provider,
                    const UKControllerPlugin::Euroscope::SectorFileAwareCollection & handlers
                );
                std::string LoadedSectorFile(void) const;


                // Inherited via AbstractTimedEvent
                void TimedEventTrigger(void) override;

            private:
                
                // Provides the sector file
                UKControllerPlugin::Euroscope::SectorFileProviderInterface & provider;

                // Instances that want to know if the users sector file changes
                const UKControllerPlugin::Euroscope::SectorFileAwareCollection & handlers;

                // The currently loaded sectorfile
                std::string loadedSectorFile = "";
        };
    }  // namespace Euroscope
}  // namespace UKControllerPlugin
