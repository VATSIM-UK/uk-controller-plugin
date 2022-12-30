#include "SingleCode.h"

namespace UKControllerPlugin::IntentionCode {
    SingleCode::SingleCode(std::string code)
        : code(std::move(code))
    {
    }
    
    auto SingleCode::GenerateCode(const Euroscope::EuroScopeCFlightPlanInterface& flightplan) const -> std::string
    {
        return code;
    }
} // namespace UKControllerPlugin::IntentionCode
