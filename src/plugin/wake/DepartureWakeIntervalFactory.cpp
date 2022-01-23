#include "DepartureWakeInterval.h"
#include "DepartureWakeIntervalFactory.h"

namespace UKControllerPlugin::Wake {

    auto IntervalFromJson(const nlohmann::json& json) -> std::shared_ptr<DepartureWakeInterval>
    {
        if (!IntervalValid(json)) {
            LogWarning("Invalid departure wake interval " + json.dump());
            return nullptr;
        }

        return std::make_shared<DepartureWakeInterval>(
            json.at("id").get<int>(),
            json.at("interval").get<int>(),
            json.at("interval_unit").get<std::string>(),
            json.at("intermediate").get<bool>());
    }

    auto IntervalValid(const nlohmann::json& json) -> bool
    {
        return json.is_object() && json.contains("id") && json.at("id").is_number_integer() &&
               json.contains("interval") && json.at("interval").is_number_integer() && json.contains("intermediate") &&
               json.at("intermediate").is_boolean() && json.contains("interval_unit") &&
               json.at("interval_unit").is_string() && IntervalUnitValid(json.at("interval_unit").get<std::string>());
    }

    auto IntervalUnitValid(const std::string& unit) -> bool
    {
        return unit == "nm" || unit == "s";
    }
} // namespace UKControllerPlugin::Wake
