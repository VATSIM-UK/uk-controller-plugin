#include "ArrivalWakeInterval.h"

namespace UKControllerPlugin::Wake {
    ArrivalWakeInterval::ArrivalWakeInterval(int subsequentWakeCategoryId, double intervalValue)
        : subsequentWakeCategoryId(subsequentWakeCategoryId), intervalValue(intervalValue), unit("nm")
    {
    }

    auto ArrivalWakeInterval::Value() const -> double
    {
        return intervalValue;
    }

    auto ArrivalWakeInterval::Unit() const -> const std::string&
    {
        return unit;
    }

    auto ArrivalWakeInterval::SubsequentCategory() const -> int
    {
        return subsequentWakeCategoryId;
    }
} // namespace UKControllerPlugin::Wake
