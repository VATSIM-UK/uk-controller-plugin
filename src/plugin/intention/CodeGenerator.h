#pragma once

namespace UKControllerPlugin::Euroscope {
    class EuroScopeCFlightPlanInterface;
} //

namespace UKControllerPlugin::IntentionCode {
    /**
     * An interface for classes that generate the actual intention code codes
     */
    class CodeGenerator
    {
        public:
        virtual ~CodeGenerator() = default;
        [[nodiscard]] virtual auto GenerateCode(const Euroscope::EuroScopeCFlightPlanInterface& flightplan) const ->
            std::string = 0;
    };
} // namespace UKControllerPlugin::IntentionCode
