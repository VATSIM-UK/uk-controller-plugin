#pragma once
#include "hold/MinStackHoldLevelRestriction.h"
#include "bootstrap/PersistenceContainer.h"

namespace UKControllerPlugin {
    namespace Bootstrap {
        struct PersistenceContainer;
    } // namespace Bootstrap
} // namespace UKControllerPlugin

namespace UKControllerPlugin {
    namespace Hold {
        class MinStackHoldLevelRestriction;

        std::unique_ptr<UKControllerPlugin::Hold::MinStackHoldLevelRestriction> MinStackRestrictionFromJson(
            const nlohmann::json& json, const UKControllerPlugin::Bootstrap::PersistenceContainer& container);

        bool RestrictionJsonValid(const nlohmann::json& json);
    } // namespace Hold
} // namespace UKControllerPlugin
