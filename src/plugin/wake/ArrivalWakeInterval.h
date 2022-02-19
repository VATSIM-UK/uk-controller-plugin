#pragma once

namespace UKControllerPlugin::Wake {
    using ArrivalWakeInterval = struct ArrivalWakeInterval
    {
        ArrivalWakeInterval(int subsequentWakeCategoryId, double intervalValue)
            : subsequentWakeCategoryId(subsequentWakeCategoryId), intervalValue(intervalValue)
        {
        }

        // The wake category id of the subsequent arrival aircraft
        int subsequentWakeCategoryId;

        // The value of the interval
        double intervalValue;
    };
} // namespace UKControllerPlugin::Wake
