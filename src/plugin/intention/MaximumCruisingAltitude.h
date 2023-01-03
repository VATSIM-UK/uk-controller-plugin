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
            const Euroscope::EuroScopeCFlightPlanInterface& flightplan,
            const Euroscope::EuroScopeCRadarTargetInterface& radarTarget) -> bool override;

        private:
        int altitude;
    };
} // namespace UKControllerPlugin::IntentionCode
