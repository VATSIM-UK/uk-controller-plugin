#include "DepartureWakeInterval.h"

namespace UKControllerPlugin::Wake {
    DepartureWakeInterval::DepartureWakeInterval(
        int subsequentWakeCategoryId, int intervalValue, std::string intervalUnit, bool intervalIsIntermediate)
        : subsequentWakeCategoryId(subsequentWakeCategoryId), intervalValue(intervalValue), intervalUnit(intervalUnit),
          intervalIsIntermediate(intervalIsIntermediate)
    {
    }

    auto DepartureWakeInterval::SubsequentCategory() const -> int
    {
        return subsequentWakeCategoryId;
    }

    auto DepartureWakeInterval::Value() const -> double
    {
        return intervalValue;
    }
    auto DepartureWakeInterval::Unit() const -> const std::string&
    {
        return intervalUnit;
    }
    auto DepartureWakeInterval::Intermediate() const -> bool
    {
        return intervalIsIntermediate;
    }
} // namespace UKControllerPlugin::Wake
