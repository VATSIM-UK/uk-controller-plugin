#include "FlightRule.h"
#include "FlightRuleCollection.h"
#include "FlightRuleCollectionFactory.h"

namespace UKControllerPlugin::FlightRules {
    auto CreateCollection(const nlohmann::json& json) -> std::unique_ptr<FlightRuleCollection>
    {
        auto collection = std::make_unique<FlightRuleCollection>();
        if (!json.is_array()) {
            LogError("Flight rule dependency is not array");
            return std::move(collection);
        }

        for (const auto& flightRule : json) {
            if (!FlightRuleValid(flightRule)) {
                LogError("Invalid flight rule: " + flightRule.dump());
                continue;
            }

            collection->Add(std::make_shared<FlightRule>(
                flightRule.at("id").get<int>(),
                flightRule.at("euroscope_key").get<std::string>(),
                flightRule.at("description").get<std::string>()));
        }

        LogInfo("Loaded " + std::to_string(collection->Count()) + " flight rules");
        return std::move(collection);
    }

    auto FlightRuleValid(const nlohmann::json& flightRule) -> bool
    {
        return flightRule.is_object() && flightRule.contains("id") && flightRule.at("id").is_number_integer() &&
               flightRule.contains("euroscope_key") && flightRule.at("euroscope_key").is_string() &&
               flightRule.contains("description") && flightRule.at("description").is_string();
    }
} // namespace UKControllerPlugin::FlightRules
