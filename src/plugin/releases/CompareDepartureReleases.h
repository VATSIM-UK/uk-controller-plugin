#pragma once

namespace UKControllerPlugin::Releases {

    class DepartureReleaseRequest;

    /*
        Used for comparing departure releases
    */
    using CompareDepartureReleases = struct CompareDepartureReleases
    {
        using is_transparent = int;

        bool operator()(const std::shared_ptr<DepartureReleaseRequest>& request, int id) const;
        bool operator()(int, const std::shared_ptr<DepartureReleaseRequest>& request) const;
        bool operator()(
            const std::shared_ptr<DepartureReleaseRequest>& a, const std::shared_ptr<DepartureReleaseRequest>& b) const;
    };
} // namespace UKControllerPlugin::Releases
