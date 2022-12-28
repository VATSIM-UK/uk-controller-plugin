#pragma once

namespace UKControllerPlugin::IntentionCode {

    class FirExitPointCollection;

    [[nodiscard]] auto MakeFirExitPointCollection(const nlohmann::json& exitPointData)
        -> std::shared_ptr<FirExitPointCollection>;
    [[nodiscard]] auto ExitPointValid(const nlohmann::json& exitPointData) -> bool;
    [[nodiscard]] auto HeadingValid(const std::string& key, const nlohmann::json& exitPointData) -> bool;
} // namespace UKControllerPlugin::IntentionCode
