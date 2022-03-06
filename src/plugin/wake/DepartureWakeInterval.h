#pragma once
#include "WakeIntervalInterface.h"

namespace UKControllerPlugin::Wake {
    class DepartureWakeInterval : public WakeIntervalInterface
    {
        public:
        DepartureWakeInterval(
            int subsequentWakeCategoryId, int intervalValue, std::string intervalUnit, bool intervalIsIntermediate);
        auto SubsequentCategory() const -> int;
        auto Value() const -> double override;
        auto Unit() const -> const std::string& override;
        auto Intermediate() const -> bool;

        private:
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
