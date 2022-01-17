#pragma once

namespace UKControllerPlugin::Wake {
    class WakeScheme;

    [[nodiscard]] auto WakeSchemeFromJson(const nlohmann::json& json) -> std::shared_ptr<WakeScheme>;
    [[nodiscard]] auto SchemeValid(const nlohmann::json& json) -> bool;
} // namespace UKControllerPlugin::Wake
