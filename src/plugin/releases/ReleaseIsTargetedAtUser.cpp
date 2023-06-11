#include "ReleaseIsTargetedAtUser.h"
#include "controller/ActiveCallsignCollection.h"
#include "controller/ActiveCallsign.h"
#include "controller/ControllerPosition.h"
#include "releases/DepartureReleaseRequest.h"

namespace UKControllerPlugin::Releases {

    ReleaseIsTargetedAtUser::ReleaseIsTargetedAtUser(
        std::shared_ptr<const Controller::ActiveCallsignCollection> activeCallsigns)
        : activeCallsigns(std::move(activeCallsigns))
    {
        assert(this->activeCallsigns && "activeCallsigns must not be null");
    }

    /**
     * Check that the user has a callsign, and that the callsign has the same normalised controller position
     * as the target of the release request.
     */
    bool ReleaseIsTargetedAtUser::IsRelevant(const DepartureReleaseRequest& releaseRequest) const
    {
        return activeCallsigns->UserHasCallsign() &&
               activeCallsigns->GetUserCallsign().GetNormalisedPosition().GetId() == releaseRequest.TargetController();
    }
} // namespace UKControllerPlugin::Releases
