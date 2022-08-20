#pragma once
#include "euroscope/RunwayDialogAwareInterface.h"

namespace UKControllerPlugin::Handoff {

    class HandoffCache;

    class ClearCacheOnRunwayDialogSave : public Euroscope::RunwayDialogAwareInterface
    {
        public:
        ClearCacheOnRunwayDialogSave(std::shared_ptr<HandoffCache> cache);
        void RunwayDialogSaved() override;

        private:
        // The cache
        const std::shared_ptr<HandoffCache> cache;
    };
} // namespace UKControllerPlugin::Handoff
