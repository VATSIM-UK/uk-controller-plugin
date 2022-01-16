#pragma once

namespace UKControllerPlugin::Aircraft {
    class AircraftType;
    [[nodiscard]] auto FromJson(const nlohmann::json& json) -> std::shared_ptr<AircraftType>;
    [[nodiscard]] auto JsonValid(const nlohmann::json& json) -> bool;
} // namespace UKControllerPlugin::Aircraft
