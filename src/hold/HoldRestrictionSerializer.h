#pragma once
#include "hold/AbstractHoldLevelRestriction.h"

namespace UKControllerPlugin {
    namespace Hold {
        void from_json(
            const nlohmann::json & json,
            std::set<std::unique_ptr<AbstractHoldLevelRestriction>> & restrictions
        );
        bool ValidRestrictionData(const nlohmann::json & json);
    }  // namespace Hold
}  // namespace UKControllerPlugin
