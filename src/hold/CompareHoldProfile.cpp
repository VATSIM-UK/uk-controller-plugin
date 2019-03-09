#include "pch/stdafx.h"
#include "hold/CompareHoldProfile.h"
#include "hold/HoldProfile.h"

using UKControllerPlugin::Hold::HoldProfile;

namespace UKControllerPlugin {
    namespace Hold {
        bool CompareHoldProfile::operator()(const HoldProfile & hold, unsigned int id) const
        {
            return hold.id < id;
        }
        bool CompareHoldProfile::operator()(unsigned int id, const HoldProfile & hold) const
        {
            return id < hold.id;
        }
        bool CompareHoldProfile::operator()(const HoldProfile & a, const HoldProfile & b) const
        {
            return a.id < b.id;
        }
    }  // namespace Hold
}  // namespace UKControllerPlugin
