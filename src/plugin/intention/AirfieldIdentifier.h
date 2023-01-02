#pragma once
#include "intention/CodeGenerator.h"

namespace UKControllerPlugin::IntentionCode {
    /**
     * Returns the last two characters of the airfield identifier.
     */
    class AirfieldIdentifier : public CodeGenerator
    {
        public:
        auto GenerateCode(const Euroscope::EuroScopeCFlightPlanInterface& flightplan) const -> std::string override;
    };
} // namespace UKControllerPlugin::IntentionCode
