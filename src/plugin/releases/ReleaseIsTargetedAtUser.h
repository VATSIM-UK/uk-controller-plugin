#pragma once
#include "releases/DepartureReleaseRelevanceChecker.h"

namespace UKControllerPlugin::Controller {
    class ActiveCallsignCollection;
} // namespace UKControllerPlugin::Controller

namespace UKControllerPlugin::Releases {

    class ReleaseIsTargetedAtUser : public DepartureReleaseRelevanceChecker
    {
        public:
        explicit ReleaseIsTargetedAtUser(std::shared_ptr<const Controller::ActiveCallsignCollection> activeCallsigns);
        [[nodiscard]] bool IsRelevant(const DepartureReleaseRequest& releaseRequest) const override;

        private:
        // The active callsigns
        const std::shared_ptr<const Controller::ActiveCallsignCollection> activeCallsigns;
    };

} // namespace UKControllerPlugin::Releases
