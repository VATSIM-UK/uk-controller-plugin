#pragma once
#include "pch/stdafx.h"
#include "sectorfile/UserSectorFileTracker.h"

namespace UKControllerPlugin {
    namespace SectorFile {

        UserSectorFileTracker::UserSectorFileTracker(
            SectorFileProviderInterface & provider,
            const SectorFileAwareCollection & handlers
        )
            : provider(provider), handlers(handlers)
        {

        }

        std::string UserSectorFileTracker::LoadedSectorFile(void) const
        {
            return this->loadedSectorFile;
        }

        /*
            Check if the users sectorfile has updated
        */
        void UserSectorFileTracker::TimedEventTrigger(void)
        {
            if (this->loadedSectorFile != this->provider.GetSectorFileName()) {
                this->handlers.SectorFileUpdate(provider);
                this->loadedSectorFile = this->provider.GetSectorFileName();
            }
        }
    }  // namespace Euroscope
}  // namespace UKControllerPlugin
