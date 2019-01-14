#include "pch/stdafx.h"
#include "hold/CompareHolds.h"
#include "hold/HoldingData.h"

using UKControllerPlugin::Hold::HoldingData;

namespace UKControllerPlugin {
    namespace Hold {
        bool CompareHolds::operator()(const HoldingData & initial, std::string id) const
        {
            return initial.identifier < id;
        }
        bool CompareHolds::operator()(std::string id, const HoldingData & initial) const
        {
            return id < initial.identifier;
        }
        bool CompareHolds::operator()(const HoldingData & a, const HoldingData & b) const
        {
            return a.identifier < b.identifier;
        }
    }  // namespace Hold
}  // namespace UKControllerPlugin
