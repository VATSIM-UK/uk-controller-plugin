#pragma once
#include "Condition.h"

namespace UKControllerPlugin::IntentionCode {

    /**
     * A condition that only passes if the aircraft is arriving at one of the listed
     * airfields.
     */
    class ArrivalAirfields : public Condition
    {
        public:
        ArrivalAirfields(std::set<std::string> airfields);
        [[nodiscard]] auto Passes(
            Euroscope::EuroScopeCFlightPlanInterface& flightplan,
            Euroscope::EuroScopeCRadarTargetInterface& radarTarget) -> bool override;

        private:
        std::set<std::string> airfields;
    };
} // namespace UKControllerPlugin::IntentionCode
