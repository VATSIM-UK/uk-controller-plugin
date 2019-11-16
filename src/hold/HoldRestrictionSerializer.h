#pragma once
#include "hold/AbstractHoldLevelRestriction.h"
#include "bootstrap/PersistenceContainer.h"

namespace UKControllerPlugin {
    namespace Hold {
        void hold_restriction_from_json(
            const nlohmann::json & json,
            std::set<std::unique_ptr<AbstractHoldLevelRestriction>> & restrictions,
            const UKControllerPlugin::Bootstrap::PersistenceContainer & container
        );
        bool ValidRestrictionData(const nlohmann::json & json);
    }  // namespace Hold
}  // namespace UKControllerPlugin
