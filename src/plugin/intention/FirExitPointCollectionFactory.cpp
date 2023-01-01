#include "ExitCone.h"
#include "FirExitPoint.h"
#include "FirExitPointCollection.h"
#include "FirExitPointCollectionFactory.h"

namespace UKControllerPlugin::IntentionCode {

    auto MakeFirExitPointCollection(const nlohmann::json& exitPointData) -> std::shared_ptr<FirExitPointCollection>
    {
        auto collection = std::make_shared<FirExitPointCollection>();
        if (!exitPointData.is_array()) {
            return collection;
        }

        for (const auto& exitPoint : exitPointData) {
            if (!ExitPointValid(exitPoint)) {
                LogError("Invalid FIR exit point: " + exitPoint.dump());
                continue;
            }

            collection->Add(std::make_shared<FirExitPoint>(
                exitPoint.at("id").get<int>(),
                exitPoint.at("exit_point").get<std::string>(),
                exitPoint.at("internal").get<bool>(),
                std::make_shared<ExitCone>(
                    exitPoint.at("exit_direction_start").get<int>(), exitPoint.at("exit_direction_end").get<int>())));
        }

        LogInfo("Loaded " + std::to_string(collection->CountPoints()) + " FIR Exit Points");
        return collection;
    }

    auto ExitPointValid(const nlohmann::json& exitPointData) -> bool
    {
        return exitPointData.is_object() && exitPointData.contains("id") &&
               exitPointData.at("id").is_number_integer() && exitPointData.contains("exit_point") &&
               exitPointData.at("exit_point").is_string() && exitPointData.contains("internal") &&
               exitPointData.at("internal").is_boolean() && HeadingValid("exit_direction_start", exitPointData) &&
               HeadingValid("exit_direction_end", exitPointData);
    }

    auto HeadingValid(const std::string& key, const nlohmann::json& exitPointData) -> bool
    {
        if (!exitPointData.contains(key)) {
            return false;
        }

        if (!exitPointData.at(key).is_number_integer()) {
            return false;
        }

        const auto heading = exitPointData.at(key).get<int>();
        return heading >= 0 && heading <= 360;
    }
} // namespace UKControllerPlugin::IntentionCode
