#pragma once
#include "intention/CodeGenerator.h"

namespace UKControllerPlugin::IntentionCode {
    /**
     * Returns a single code, for example "AM", "C2".
     */
    class SingleCode : public CodeGenerator
    {
        public:
        SingleCode(std::string code);
        [[nodiscard]] auto Code() const -> const std::string&;
        auto GenerateCode(const Euroscope::EuroScopeCFlightPlanInterface& flightplan) const -> std::string override;

        private:
        std::string code;
    };
} // namespace UKControllerPlugin::IntentionCode
