#include "pch/pch.h"
#include "hold/CompareHoldsDescription.h"
#include "hold/HoldingData.h"

using UKControllerPlugin::Hold::HoldingData;

namespace UKControllerPlugin {
    namespace Hold {
        bool CompareHoldsDescription::operator()(const HoldingData & hold, std::string description) const
        {
            return hold.description < description;
        }
        bool CompareHoldsDescription::operator()(std::string description, const HoldingData & hold) const
        {
            return description < hold.description;
        }
        bool CompareHoldsDescription::operator()(const HoldingData & a, const HoldingData & b) const
        {
            return a.description < b.description;
        }
    }  // namespace Hold
}  // namespace UKControllerPlugin
