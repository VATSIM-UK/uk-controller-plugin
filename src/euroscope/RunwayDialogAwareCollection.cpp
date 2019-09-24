#pragma once
#include "pch/stdafx.h"
#include "euroscope/RunwayDialogAwareCollection.h"

using UKControllerPlugin::Euroscope::RunwayDialogAwareInterface;

namespace UKControllerPlugin {
    namespace Euroscope {

        void RunwayDialogAwareCollection::AddHandler(std::shared_ptr<RunwayDialogAwareInterface> handler)
        {
            if (!this->handlers.insert(handler).second) {
                LogWarning("Attempted to add duplicate sector file handler");
            }
        }

        size_t RunwayDialogAwareCollection::CountHandlers(void) const
        {
            return this->handlers.size();
        }

        void RunwayDialogAwareCollection::RunwayDialogSaved(void) const
        {
            for (
                std::set<std::shared_ptr<RunwayDialogAwareInterface>>::const_iterator it = this->handlers.cbegin();
                it != this->handlers.cend();
                ++it
            ) {
                (*it)->RunwayDialogSaved();
            }
        }
    }  // namespace Euroscope
}  // namespace UKControllerPlugin
