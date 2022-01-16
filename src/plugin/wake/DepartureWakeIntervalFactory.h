#pragma once

namespace UKControllerPlugin::Wake {
    struct DepartureWakeInterval;

    [[nodiscard]] auto IntervalFromJson(const nlohmann::json& json) -> std::shared_ptr<DepartureWakeInterval>;
    [[nodiscard]] auto IntervalValid(const nlohmann::json& json) -> bool;
    [[nodiscard]] auto IntervalUnitValid(const std::string& unit) -> bool;
} // namespace UKControllerPlugin::Wake
