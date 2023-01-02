#pragma once
#include "Condition.h"

namespace UKControllerPlugin::IntentionCode {

    /**
     * A condition that only passes if the aircraft is cruising above a given level
     */
    class CruisingLevelAbove : public Condition
    {
        public:
        CruisingLevelAbove(int altitude);
        [[nodiscard]] auto Level() const -> int;
        [[nodiscard]] auto Passes(
            const Euroscope::EuroScopeCFlightPlanInterface& flightplan,
            const Euroscope::EuroScopeCRadarTargetInterface& radarTarget) -> bool override;

        private:
        int altitude;
    };
} // namespace UKControllerPlugin::IntentionCode
