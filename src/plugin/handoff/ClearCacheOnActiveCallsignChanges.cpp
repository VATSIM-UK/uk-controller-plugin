#include "ClearCacheOnActiveCallsignChanges.h"
#include "HandoffCache.h"
#include "ResolvedHandoff.h"
#include "controller/ActiveCallsign.h"
#include "controller/ControllerPosition.h"
#include "controller/ControllerPositionHierarchy.h"

namespace UKControllerPlugin::Handoff {

    ClearCacheOnActiveCallsignChanges::ClearCacheOnActiveCallsignChanges(HandoffCache& cache) : cache(cache)
    {
    }

    /**
     * When new controllers come online, we need to evict from the cache any controller who preceeds them
     * in the hierarchy.
     */
    void ClearCacheOnActiveCallsignChanges::ActiveCallsignAdded(const Controller::ActiveCallsign& callsign)
    {
        this->cache.DeleteWhere([&callsign](const std::shared_ptr<ResolvedHandoff>& handoff) -> bool {
            /*
             * The SID hierarchy has the highest priority. So if the resolved controller is from the SIDs hierarchy,
             * then the controller coming online must preceed it.
             */
            if (handoff->sidHierarchy->PositionInHierarchy(*handoff->resolvedController)) {
                return handoff->sidHierarchy->PositionPreceeds(
                    callsign.GetNormalisedPosition(), *handoff->resolvedController);
            }

            /*
             * If the resolved controller is in the airfield fallback hierarchy, then the controller coming online must
             * preceed it in that.
             */
            if (handoff->airfieldHierarchy->PositionInHierarchy(*handoff->resolvedController)) {
                return handoff->airfieldHierarchy->PositionPreceeds(
                    callsign.GetNormalisedPosition(), *handoff->resolvedController);
            }

            /*
             * Finally, if the resolved controller isn't from a hierarchy, it must be unicom.
             * Therefore, we need to evict from cache if the controller coming online is in either hierarchy.
             */
            return handoff->sidHierarchy->PositionInHierarchy(callsign.GetNormalisedPosition()) ||
                   handoff->airfieldHierarchy->PositionInHierarchy(callsign.GetNormalisedPosition());
        });
    }

    /**
     * If we have a controller that logs off, we only need to evict from the cache instances where
     * this controller is the resolved controller.
     */
    void ClearCacheOnActiveCallsignChanges::ActiveCallsignRemoved(const Controller::ActiveCallsign& callsign)
    {
        this->cache.DeleteWhere([&callsign](const std::shared_ptr<ResolvedHandoff>& handoff) -> bool {
            return *handoff->resolvedController == callsign.GetNormalisedPosition();
        });
    }

    /**
     * If we lose track of who's online alltogether, time to clear the cache.
     */
    void ClearCacheOnActiveCallsignChanges::CallsignsFlushed()
    {
        this->cache.Clear();
    }
} // namespace UKControllerPlugin::Handoff
