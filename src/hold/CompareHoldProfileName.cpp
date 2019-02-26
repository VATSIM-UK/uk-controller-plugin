#include "pch/stdafx.h"
#include "hold/CompareHoldProfileName.h"
#include "hold/HoldProfile.h"

using UKControllerPlugin::Hold::HoldProfile;

namespace UKControllerPlugin {
    namespace Hold {
        bool CompareHoldProfileName::operator()(const HoldProfile & hold, std::string name) const
        {
            return hold.name < name;
        }
        bool CompareHoldProfileName::operator()(std::string name, const HoldProfile & hold) const
        {
            return name < hold.name;
        }
        bool CompareHoldProfileName::operator()(const HoldProfile & a, const HoldProfile & b) const
        {
            return a.name < b.name;
        }
    }  // namespace Hold
}  // namespace UKControllerPlugin
