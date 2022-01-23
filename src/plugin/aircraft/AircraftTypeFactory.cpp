#include "AircraftType.h"
#include "AircraftTypeFactory.h"

namespace UKControllerPlugin::Aircraft {

    auto FromJson(const nlohmann::json& json) -> std::shared_ptr<AircraftType>
    {
        if (!JsonValid(json)) {
            LogInfo("Invalid aircraft type " + json.dump());
            return nullptr;
        }

        return std::make_shared<AircraftType>(
            json.at("id").get<int>(),
            json.at("icao_code").get<std::string>(),
            json.at("wake_categories").get<std::set<int>>());
    }

    auto JsonValid(const nlohmann::json& json) -> bool
    {
        return json.is_object() && json.contains("id") && json.at("id").is_number_integer() &&
               json.contains("icao_code") && json.at("icao_code").is_string() && json.contains("wake_categories") &&
               json.at("wake_categories").is_array() &&
               std::find_if_not(
                   json.at("wake_categories").cbegin(),
                   json.at("wake_categories").cend(),
                   [](const nlohmann::json& category) -> bool { return category.is_number_integer(); }) ==
                   json.at("wake_categories").cend();
    }
} // namespace UKControllerPlugin::Aircraft
