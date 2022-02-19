#pragma once
#include "WakeIntervalInterface.h"

namespace UKControllerPlugin::Wake {
    class ArrivalWakeInterval : public WakeIntervalInterface
    {
        public:
        ArrivalWakeInterval(int subsequentWakeCategoryId, double intervalValue);

        auto SubsequentCategory() const -> int;
        auto Value() const -> double override;
        auto Unit() const -> const std::string& override;

        private:
        // The wake category id of the subsequent arrival aircraft
        int subsequentWakeCategoryId;

        // The value of the interval
        double intervalValue;

        // The unit
        std::string unit;
    };
} // namespace UKControllerPlugin::Wake
