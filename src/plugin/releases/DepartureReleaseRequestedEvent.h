#pragma once

namespace UKControllerPlugin::Releases {

    class DepartureReleaseRequest;

    using DepartureReleaseRequestedEvent = struct DepartureReleaseRequestedEvent
    {
        std::shared_ptr<DepartureReleaseRequest> releaseRequest;
    };
} // namespace UKControllerPlugin::Releases
