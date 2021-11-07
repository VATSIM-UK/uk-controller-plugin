#include "CompareMissedApproaches.h"
#include "MissedApproach.h"

namespace UKControllerPlugin::MissedApproach {
    auto CompareMissedApproaches::operator()(const std::shared_ptr<MissedApproach>& missed, int id) const -> bool
    {
        return missed->Id() < id;
    }

    auto CompareMissedApproaches::operator()(int id, const std::shared_ptr<MissedApproach>& missed) const -> bool
    {
        return id < missed->Id();
    }

    auto CompareMissedApproaches::operator()(
        const std::shared_ptr<MissedApproach>& a, const std::shared_ptr<MissedApproach>& b) const -> bool
    {
        return a->Id() < b->Id();
    }
} // namespace UKControllerPlugin::MissedApproach
