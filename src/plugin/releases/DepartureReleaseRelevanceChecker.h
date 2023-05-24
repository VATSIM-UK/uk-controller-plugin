#pragma once

namespace UKControllerPlugin::Releases {

    class DepartureReleaseRequest;

    /**
     * Checks if a release request is relevant to the current user.
     */
    class DepartureReleaseRelevanceChecker
    {
        public:
        virtual ~DepartureReleaseRelevanceChecker() = default;
        [[nodiscard]] virtual bool IsRelevant(const DepartureReleaseRequest& releaseRequest) const = 0;
    };
} // namespace UKControllerPlugin::Releases
