#pragma once
#include "Condition.h"

namespace UKControllerPlugin::IntentionCode {

    class AircraftFirExitGenerator;

    /**
     * A condition that passes if an aircraft is exiting the FIR at a given point.
     */
    class ExitingFirAtPoint : public Condition
    {
        public:
        ExitingFirAtPoint(std::shared_ptr<AircraftFirExitGenerator> firExitGenerator, int firExitPointId);
        [[nodiscard]] auto ExitPoint() const -> int;
        [[nodiscard]] auto Passes(
            const Euroscope::EuroScopeCFlightPlanInterface& flightplan,
            const Euroscope::EuroScopeCRadarTargetInterface& radarTarget) -> bool override;

        private:
        // Generates the FIR exit point for an aircraft
        std::shared_ptr<AircraftFirExitGenerator> firExitGenerator;

        // The FIR exit point applying to this rule
        int firExitPointId;
    };
} // namespace UKControllerPlugin::IntentionCode
