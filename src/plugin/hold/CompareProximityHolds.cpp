#include "CompareProximityHolds.h"
#include "ProximityHold.h"

namespace UKControllerPlugin::Hold {
    bool CompareProximityHolds::operator()(const std::shared_ptr<ProximityHold>& proximity, std::string navaid) const
    {
        return proximity->Navaid() < navaid;
    }

    bool CompareProximityHolds::operator()(std::string navaid, const std::shared_ptr<ProximityHold>& proximity) const
    {
        return navaid < proximity->Navaid();
    }

    bool CompareProximityHolds::operator()(
        const std::shared_ptr<ProximityHold>& a, const std::shared_ptr<ProximityHold>& b) const
    {
        return a->Navaid() < b->Navaid();
    }
} // namespace UKControllerPlugin::Hold
