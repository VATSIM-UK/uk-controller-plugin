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
        [[nodiscard]] auto Passes(
            Euroscope::EuroScopeCFlightPlanInterface& flightplan,
            Euroscope::EuroScopeCRadarTargetInterface& radarTarget) -> bool override;

        private:
        int altitude;
    };
} // namespace UKControllerPlugin::IntentionCode
