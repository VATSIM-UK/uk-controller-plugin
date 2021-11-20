#pragma once

namespace UKControllerPlugin::FlightRules {
    class FlightRuleCollection;

    [[nodiscard]] auto CreateCollection(const nlohmann::json& json) -> std::unique_ptr<FlightRuleCollection>;
    [[nodiscard]] auto FlightRuleValid(const nlohmann::json& flightRule) -> bool;
} // namespace UKControllerPlugin::FlightRules
