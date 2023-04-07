#pragma once

namespace UKControllerPlugin::Bootstrap {
    struct PersistenceContainer;
} // namespace UKControllerPlugin::Bootstrap

namespace UKControllerPlugin {
    namespace Hold {
        class MinStackHoldLevelRestriction;

        std::unique_ptr<UKControllerPlugin::Hold::MinStackHoldLevelRestriction> MinStackRestrictionFromJson(
            const nlohmann::json& json, const UKControllerPlugin::Bootstrap::PersistenceContainer& container);

        bool RestrictionJsonValid(const nlohmann::json& json);
    } // namespace Hold
} // namespace UKControllerPlugin
