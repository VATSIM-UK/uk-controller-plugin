#include "pch/pch.h"
#include "releases/CompareDepartureReleases.h"
#include "releases/DepartureReleaseRequest.h"


namespace UKControllerPlugin::Releases {
    bool CompareDepartureReleases::operator()(
        const std::shared_ptr<DepartureReleaseRequest>& request,
        int id
    ) const
    {
        return request->Id() < id;
    }

    bool CompareDepartureReleases::operator()(
        int id,
        const std::shared_ptr<DepartureReleaseRequest>& request
    ) const
    {
        return id < request->Id();
    }

    bool CompareDepartureReleases::operator()(
        const std::shared_ptr<DepartureReleaseRequest>& a,
        const std::shared_ptr<DepartureReleaseRequest>& b
    ) const
    {
        return a->Id() < b->Id();
    }
} // namespace UKControllerPlugin::Releases
