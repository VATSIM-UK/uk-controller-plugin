#pragma once
#include "Condition.h"

namespace UKControllerPlugin::IntentionCode {

    /**
     * A condition that only passes if the aircraft is cruising at or below a given altitude
     */
    class MaximumCruisingAltitude : public Condition
    {
        public:
        MaximumCruisingAltitude(int altitude);
        [[nodiscard]] auto Altitude() const -> int;
        [[nodiscard]] auto Passes(
            Euroscope::EuroScopeCFlightPlanInterface& flightplan,
            Euroscope::EuroScopeCRadarTargetInterface& radarTarget) -> bool override;

        private:
        int altitude;
    };
} // namespace UKControllerPlugin::IntentionCode
