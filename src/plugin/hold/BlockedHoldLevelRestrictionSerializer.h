#pragma once

namespace UKControllerPlugin {
    namespace Hold {
        class BlockedHoldLevelRestriction;

        void from_json(const nlohmann::json& json, std::unique_ptr<BlockedHoldLevelRestriction>& restriction);
        void to_json(nlohmann::json& json, const std::unique_ptr<BlockedHoldLevelRestriction>& restriction);
        bool ValidateLevelRestrictionData(const nlohmann::json& json);
    } // namespace Hold
} // namespace UKControllerPlugin
