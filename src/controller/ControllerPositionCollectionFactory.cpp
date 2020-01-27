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

        // Initilise the required dependency constant
        const std::string ControllerPositionCollectionFactory::requiredDependency = "DEPENDENCY_CONTROLLER_POSITIONS";

        std::unique_ptr<ControllerPositionCollection> ControllerPositionCollectionFactory::Create(
            DependencyLoaderInterface& dependency
        ) {
            std::unique_ptr<ControllerPositionCollection> collection(new ControllerPositionCollection);
            nlohmann::json controllerPositions = dependency.LoadDependency(
                requiredDependency,
                nlohmann::json::object()
            );

            if (!controllerPositions.is_object()) {
                LogError("Unable to load controller positions, dependency data invalid");
                return collection;
            }

            for (nlohmann::json::iterator posIt = controllerPositions.begin();
                posIt != controllerPositions.end();
                posIt++
            ) {

                // Missing data, skip this position.
                if (!posIt.value()["frequency"].is_number_float() || !posIt.value()["top-down"].is_array()) {
                    continue;
                }

                std::string callsign = posIt.key();
                std::string posType = callsign.substr(callsign.size() - 3, callsign.size());
                double frequency = posIt.value()["frequency"];

                try {
                    collection->AddPosition(std::unique_ptr<ControllerPosition>(
                        new ControllerPosition(callsign, frequency, posType, posIt.value()["top-down"])
                    ));
                } catch (std::invalid_argument) {
                    LogError("Failed adding position " + callsign + " to controller collection");
                    // Do nothing if something goes wrong.
                }
            }

            LogInfo(
                "Loaded " + std::to_string(collection->GetSize()) + " controllers into controller position collection"
            );
            return collection;
        }
    }  // namespace Controller
}  // namespace UKControllerPlugin
