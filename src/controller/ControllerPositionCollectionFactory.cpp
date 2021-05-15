#include "pch/stdafx.h"
#include "controller/ControllerPositionCollectionFactory.h"
#include "controller/ControllerPositionCollection.h"
#include "controller/ControllerPosition.h"
#include "dependency/DependencyLoaderInterface.h"
#include "helper/HelperFunctions.h"

using UKControllerPlugin::Dependency::DependencyLoaderInterface;
using UKControllerPlugin::HelperFunctions;

namespace UKControllerPlugin {
    namespace Controller {

        // Initialise the required dependency constant
        const std::string ControllerPositionCollectionFactory::requiredDependency =
            "DEPENDENCY_CONTROLLER_POSITIONS_V2";

        std::unique_ptr<ControllerPositionCollection> ControllerPositionCollectionFactory::Create(
            DependencyLoaderInterface& dependency
        ) {
            std::unique_ptr<ControllerPositionCollection> collection(new ControllerPositionCollection);
            nlohmann::json controllerPositions = dependency.LoadDependency(
                requiredDependency,
                nlohmann::json::array()
            );

            if (!DependencyDataValid(controllerPositions)) {
                LogError("Unable to load controller positions, V2 dependency data is invalid");
                return collection;
            }


            for (auto controllerPosition : controllerPositions) {
                collection->AddPosition(
                    std::make_shared<ControllerPosition>(
                        controllerPosition.at("id").get<int>(),
                        controllerPosition.at("callsign").get<std::string>(),
                        controllerPosition.at("frequency").get<double>(),
                        controllerPosition.at("top_down").get<std::vector<std::string>>(),
                        controllerPosition.at("requests_departure_releases").get<bool>(),
                        controllerPosition.at("receives_departure_releases").get<bool>()
                    )
                );
            }

            LogInfo(
                "Loaded " + std::to_string(collection->GetSize()) + " controllers into controller position collection"
            );
            return collection;
        }

        bool ControllerPositionCollectionFactory::DependencyDataValid(nlohmann::json& data)
        {
            return data.is_array() &&
                std::find_if_not(
                    data.cbegin(),
                    data.cend(),
                    [](const nlohmann::json& position) -> bool
                    {
                        return position.is_object() &&
                            position.contains("id") &&
                            position.at("id").is_number_integer() &&
                            position.contains("callsign") &&
                            position.at("callsign").is_string() &&
                            position.contains("frequency") &&
                            position.at("frequency").is_number_float() &&
                            position.contains("top_down") &&
                            position.at("top_down").is_array() &&
                            std::find_if_not(
                                position.at("top_down").cbegin(),
                                position.at("top_down").cend(),
                                [](const nlohmann::json& topdown) -> bool
                                {
                                    return topdown.is_string();
                                }
                            ) == position.at("top_down").cend() &&
                            position.contains("requests_departure_releases") &&
                            position.at("requests_departure_releases").is_boolean() &&
                            position.contains("receives_departure_releases") &&
                            position.at("receives_departure_releases").is_boolean();
                    }
                ) == data.cend();
        }
    }  // namespace Controller
}  // namespace UKControllerPlugin
