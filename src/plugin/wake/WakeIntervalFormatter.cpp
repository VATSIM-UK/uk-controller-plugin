#include "DepartureWakeInterval.h"
#include "WakeIntervalFormatter.h"
#include "helper/HelperFunctions.h"

namespace UKControllerPlugin::Wake {

    auto FormatInterval(const WakeIntervalInterface& interval) -> std::wstring
    {
        if (interval.Unit() == "s") {
            auto time = static_cast<int>(std::ceil(interval.Value()));
            if (time % 60 == 0) {
                return std::to_wstring(time / 60) + L" mins";
            } else {
                return std::to_wstring(time) + L" secs";
            }
        } else if (interval.Unit() == "nm") {
            char distanceString[25];                             // NOLINT
            sprintf_s(distanceString, "%.1f", interval.Value()); // NOLINT
            return HelperFunctions::ConvertToWideString(distanceString) + L" " + L"nm";
        }

        return L"--";
    }
} // namespace UKControllerPlugin::Wake
