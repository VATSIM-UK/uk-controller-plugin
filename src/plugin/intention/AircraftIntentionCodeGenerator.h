#pragma once

namespace UKControllerPlugin::Euroscope {
    class EuroScopeCFlightPlanInterface;
    class EuroScopeCRadarTargetInterface;
} // namespace UKControllerPlugin::Euroscope

namespace UKControllerPlugin::IntentionCode {
    struct AircraftIntentionCode;

    /*
        Interface for classes that generate an aircrafts intention code
    */
    class AircraftIntentionCodeGenerator
    {
        public:
        virtual ~AircraftIntentionCodeGenerator() = default;
        /*
            Generates the aircrafts FIR exit point data, or nullptr if there are none.
        */
        [[nodiscard]] virtual auto Generate(
            const Euroscope::EuroScopeCFlightPlanInterface& flightplan,
            const Euroscope::EuroScopeCRadarTargetInterface& radarTarget) -> std::shared_ptr<AircraftIntentionCode> = 0;
    };
} // namespace UKControllerPlugin::IntentionCode
