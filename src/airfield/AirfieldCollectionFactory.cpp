#include "pch/stdafx.h"
#include "airfield/AirfieldCollectionFactory.h"
#include "airfield/AirfieldCollection.h"
#include "airfield/Airfield.h"
#include "dependency/DependencyCache.h"

using UKControllerPlugin::Dependency::DependencyCache;
using UKControllerPlugin::Airfield::Airfield;

namespace UKControllerPlugin {
    namespace Airfield {

        // Initialise the required dependency string
        const std::string AirfieldCollectionFactory::requiredDependency = "airfield-ownership.json";

        std::unique_ptr<const AirfieldCollection> AirfieldCollectionFactory::Create(
            const UKControllerPlugin::Dependency::DependencyCache & dependency
        ) {
            std::unique_ptr<AirfieldCollection> collection(new AirfieldCollection);
            nlohmann::json dependencyData;

            try {
                dependencyData = nlohmann::json::parse(
                    dependency.GetDependency(AirfieldCollectionFactory::requiredDependency)
                );
            } catch (...) {
                // If something goes wrong, just return the empty collection.
                LogError("Failed to load airfields, dependency data was invalid");
                return std::move(collection);
            }

            // Loop through the dependency data to get the airfields and then add them to the collection.
            for (nlohmann::json::iterator it = dependencyData.begin(); it != dependencyData.end(); ++it) {

                // We can't do anything if it's not an array.
                if (!it.value().is_array()) {
                    continue;
                }

                try {
                    collection->AddAirfield(std::unique_ptr<Airfield>(new Airfield(it.key(), it.value())));
                } catch (std::invalid_argument) {
                    // We've tried to add a duplicate or invalid airfield.
                }
            }

            LogInfo("Loaded " + std::to_string(collection->GetSize()) + " airfields into airfield collection");
            return std::move(collection);
        }
    }  // namespace Airfield
}  // namespace UKControllerPlugin
