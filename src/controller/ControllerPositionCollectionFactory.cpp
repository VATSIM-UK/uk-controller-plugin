#include "pch/stdafx.h"
#include "controller/ControllerPositionCollectionFactory.h"
#include "controller/ControllerPositionCollection.h"
#include "controller/ControllerPosition.h"
#include "dependency/DependencyCache.h"
#include "helper/HelperFunctions.h"

using UKControllerPlugin::Dependency::DependencyCache;
using UKControllerPlugin::HelperFunctions;

namespace UKControllerPlugin {
    namespace Controller {

        // Initilise the required dependency constant
        const std::string ControllerPositionCollectionFactory::requiredDependency = "controller-positions.json";

        std::unique_ptr<ControllerPositionCollection> ControllerPositionCollectionFactory::Create(
            const DependencyCache & dependency
        ) {
            std::unique_ptr<ControllerPositionCollection> collection(new ControllerPositionCollection);
            nlohmann::json controllerPositions;
            try {
                controllerPositions = nlohmann::json::parse(
                    dependency.GetDependency(ControllerPositionCollectionFactory::requiredDependency)
                );
            } catch (...) {
                // If something goes wrong, we cant do anything, return the empty collection.
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
