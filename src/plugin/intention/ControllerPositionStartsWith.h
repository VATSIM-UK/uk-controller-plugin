#pragma once
#include "Condition.h"

namespace UKControllerPlugin::Controller {
    class ActiveCallsignCollection;
} // namespace UKControllerPlugin::Controller

namespace UKControllerPlugin::IntentionCode {

    /**
     * A condition that only passes if the controller position callsign starts with a given slug
     */
    class ControllerPositionStartsWith : public Condition
    {
        public:
        ControllerPositionStartsWith(
            std::shared_ptr<const Controller::ActiveCallsignCollection> activeControllers, std::string pattern);
        [[nodiscard]] auto Pattern() const -> const std::string&;
        [[nodiscard]] auto Passes(
            const Euroscope::EuroScopeCFlightPlanInterface& flightplan,
            const Euroscope::EuroScopeCRadarTargetInterface& radarTarget) -> bool override;

        private:
        // Active controllers
        std::shared_ptr<const Controller::ActiveCallsignCollection> activeControllers;

        // The airfield pattern to match
        std::string pattern;
    };
} // namespace UKControllerPlugin::IntentionCode
