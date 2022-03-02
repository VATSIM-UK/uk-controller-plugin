#include "ArrivalWakeInterval.h"
#include "ArrivalWakeIntervalFactory.h"

namespace UKControllerPlugin::Wake {
    auto ArrivalIntervalFromJson(const nlohmann::json& json) -> std::shared_ptr<ArrivalWakeInterval>
    {
        if (!ArrivalIntervalValid(json)) {
            LogWarning("Invalid arrival wake interval JSON " + json.dump());
            return nullptr;
        }

        return std::make_shared<ArrivalWakeInterval>(json.at("id").get<int>(), json.at("interval").get<double>());
    }

    auto ArrivalIntervalValid(const nlohmann::json& json) -> bool
    {
        return json.is_object() && json.contains("id") && json.at("id").is_number_integer() &&
               json.contains("interval") && json.at("interval").is_number();
    }
} // namespace UKControllerPlugin::Wake
