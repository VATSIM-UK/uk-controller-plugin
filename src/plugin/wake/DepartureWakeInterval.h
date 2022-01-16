#pragma once

namespace UKControllerPlugin::Wake {
    using DepartureWakeInterval = struct DepartureWakeInterval
    {
        DepartureWakeInterval(
            int subsequentWakeCategoryId, int intervalValue, std::string intervalUnit, bool intervalIsIntermediate)
            : subsequentWakeCategoryId(subsequentWakeCategoryId), intervalValue(intervalValue),
              intervalUnit(intervalUnit), intervalIsIntermediate(intervalIsIntermediate)
        {
        }

        // The wake category id of the subsequent departing aircraft
        int subsequentWakeCategoryId;

        // The value of the interval
        int intervalValue;

        // The unit for the interval
        std::string intervalUnit;

        // Is the interval assuming an intermediate point on the runway
        bool intervalIsIntermediate;
    };
} // namespace UKControllerPlugin::Wake
