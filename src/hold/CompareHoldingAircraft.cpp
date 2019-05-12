#include "pch/stdafx.h"
#include "hold/CompareHoldingAircraft.h"
#include "hold/HoldingAircraft.h"

using UKControllerPlugin::Hold::HoldingAircraft;

namespace UKControllerPlugin {
    namespace Hold {
        bool CompareHoldingAircraft::operator()(const HoldingAircraft & hold, std::string id) const
        {
            return hold.callsign < id;
        }
        bool CompareHoldingAircraft::operator()(std::string id, const HoldingAircraft & hold) const
        {
            return id < hold.callsign;
        }
        bool CompareHoldingAircraft::operator()(const HoldingAircraft & a, const HoldingAircraft & b) const
        {
            return a.callsign < b.callsign;
        }
    }  // namespace Hold
}  // namespace UKControllerPlugin
