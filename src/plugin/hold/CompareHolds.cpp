#include "pch/stdafx.h"
#include "hold/CompareHolds.h"
#include "hold/HoldingData.h"

using UKControllerPlugin::Hold::HoldingData;

namespace UKControllerPlugin {
    namespace Hold {
        bool CompareHolds::operator()(const HoldingData & hold, unsigned int id) const
        {
            return hold.identifier < id;
        }
        bool CompareHolds::operator()(unsigned int id, const HoldingData & hold) const
        {
            return id < hold.identifier;
        }
        bool CompareHolds::operator()(const HoldingData & a, const HoldingData & b) const
        {
            return a.identifier < b.identifier;
        }
    }  // namespace Hold
}  // namespace UKControllerPlugin
