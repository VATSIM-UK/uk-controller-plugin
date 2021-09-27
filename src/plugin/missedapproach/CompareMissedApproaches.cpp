#include "CompareMissedApproaches.h"
#include "MissedApproach.h"

namespace UKControllerPlugin::MissedApproach {
    auto CompareMissedApproaches::operator()(
        const std::shared_ptr<MissedApproach>& missed, const std::string& callsign) const -> bool
    {
        return missed->Callsign() < callsign;
    }

    auto CompareMissedApproaches::operator()(
        const std::string& callsign, const std::shared_ptr<MissedApproach>& missed) const -> bool
    {
        return callsign < missed->Callsign();
    }

    auto CompareMissedApproaches::operator()(
        const std::shared_ptr<MissedApproach>& a, const std::shared_ptr<MissedApproach>& b) const -> bool
    {
        return a->Callsign() < b->Callsign();
    }
} // namespace UKControllerPlugin::MissedApproach
