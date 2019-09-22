#pragma once
#include "pch/stdafx.h"
#include "sectorfile/SectorFileAwareInterface.h"
#include "sectorfile/SectorFileProviderInterface.h"
#include "SectorFileAwareCollection.h"

namespace UKControllerPlugin {
    namespace SectorFile {

        void SectorFileAwareCollection::AddHandler(std::shared_ptr<SectorFileAwareInterface> handler)
        {
            if (!this->handlers.insert(handler).second) {
                LogWarning("Attempted to add duplicate sector file handler");
            }
        }

        size_t SectorFileAwareCollection::CountHandlers(void) const
        {
            return this->handlers.size();
        }

        void SectorFileAwareCollection::SectorFileUpdate(SectorFileProviderInterface & provider) const
        {
            for (
                std::set<std::shared_ptr<SectorFileAwareInterface>>::const_iterator it = this->handlers.cbegin();
                it != this->handlers.cend();
                ++it
            ) {
                (*it)->SectorFileUpdate(provider);
            }
        }
    }  // namespace Euroscope
}  // namespace UKControllerPlugin
