#include "DepartureWakeInterval.h"
#include "WakeIntervalFormatter.h"

namespace UKControllerPlugin::Wake {

    auto FormatInterval(const DepartureWakeInterval& interval) -> std::wstring
    {
        if (interval.intervalUnit == "s") {
            if (interval.intervalValue % 60 == 0) {
                return std::to_wstring(interval.intervalValue / 60) + L" mins";
            } else {
                return std::to_wstring(interval.intervalValue) + L" secs";
            }
        } else if (interval.intervalUnit == "nm") {
            return std::to_wstring(interval.intervalValue) + L" " + L"nm";
        }

        return L"--";
    }
} // namespace UKControllerPlugin::Wake
