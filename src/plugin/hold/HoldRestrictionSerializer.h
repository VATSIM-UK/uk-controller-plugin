#pragma once

namespace UKControllerPlugin::Bootstrap {
    struct PersistenceContainer;
} // namespace UKControllerPlugin::Bootstrap

namespace UKControllerPlugin {
    namespace Hold {
        class AbstractHoldLevelRestriction;
        void hold_restriction_from_json(
            const nlohmann::json& json,
            std::set<std::unique_ptr<AbstractHoldLevelRestriction>>& restrictions,
            const UKControllerPlugin::Bootstrap::PersistenceContainer& container);
        bool ValidRestrictionData(const nlohmann::json& json);
    } // namespace Hold
} // namespace UKControllerPlugin
