#include "AircraftTypeCollection.h"
#include "AircraftTypeCollectionFactory.h"
#include "AircraftTypeFactory.h"

namespace UKControllerPlugin::Aircraft {
    auto CollectionFromDependency(const nlohmann::json& json) -> std::unique_ptr<AircraftTypeCollection>
    {
        auto collection = std::make_unique<AircraftTypeCollection>();
        if (!DependencyValid(json)) {
            LogWarning("Invalid aircraft type dependency");
            return collection;
        }

        for (const auto& type : json) {
            auto aircraftType = FromJson(type);
            if (!aircraftType) {
                continue;
            }

            collection->Add(aircraftType);
        }

        LogInfo("Loaded aircraft type collection with " + std::to_string(collection->Count()) + " types");
        return collection;
    }

    auto DependencyValid(const nlohmann::json& json) -> bool
    {
        return json.is_array();
    }
} // namespace UKControllerPlugin::Aircraft
