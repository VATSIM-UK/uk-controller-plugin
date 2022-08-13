#include "ClearCacheOnRunwayDialogSave.h"
#include "HandoffCache.h"

namespace UKControllerPlugin::Handoff {

    ClearCacheOnRunwayDialogSave::ClearCacheOnRunwayDialogSave(std::shared_ptr<HandoffCache> cache) : cache(cache)
    {
        assert(cache && "Handoff cache not set in ctor");
    }

    void ClearCacheOnRunwayDialogSave::RunwayDialogSaved()
    {
        cache->Clear();
    }
} // namespace UKControllerPlugin::Handoff
