#pragma once

namespace UKControllerPlugin::Wake {
    class WakeCategory;

    [[nodiscard]] auto WakeCategoryFromJson(const nlohmann::json& json) -> std::shared_ptr<WakeCategory>;
    [[nodiscard]] auto CategoryValid(const nlohmann::json& json) -> bool;
} // namespace UKControllerPlugin::Wake
