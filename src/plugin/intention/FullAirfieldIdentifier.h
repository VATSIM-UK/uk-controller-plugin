#pragma once
#include "intention/CodeGenerator.h"

namespace UKControllerPlugin::IntentionCode {
    /**
     * Returns the full airfield identifier.
     */
    class FullAirfieldIdentifier : public CodeGenerator
    {
        public:
        auto GenerateCode(const Euroscope::EuroScopeCFlightPlanInterface& flightplan) const -> std::string override;
    };
} // namespace UKControllerPlugin::IntentionCode
