#include "WakeSchemeCollection.h"
#include "WakeSchemeCollectionFactory.h"
#include "WakeSchemeFactory.h"

namespace UKControllerPlugin::Wake {

    auto CollectionFromDependency(const nlohmann::json& json) -> std::unique_ptr<WakeSchemeCollection>
    {
        auto collection = std::make_unique<WakeSchemeCollection>();
        if (!DependencyValid(json)) {
            LogWarning("Invalid wake dependency json");
            return collection;
        }

        for (const auto& schemeJson : json) {
            auto scheme = WakeSchemeFromJson(schemeJson);
            if (!scheme) {
                continue;
            }

            collection->Add(scheme);
        }

        LogInfo("Created wake scheme collection with " + std::to_string(collection->Count()) + " schemes");
        return collection;
    }

    auto DependencyValid(const nlohmann::json& json) -> bool
    {
        return json.is_array();
    }
} // namespace UKControllerPlugin::Wake
