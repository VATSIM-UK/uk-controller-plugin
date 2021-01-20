#include "pch/stdafx.h"
#include "hold/PublishedHoldCollectionFactory.h"
#include "hold/HoldingDataSerializer.h"

using UKControllerPlugin::Bootstrap::PersistenceContainer;

namespace UKControllerPlugin {
    namespace Hold {

        /*
            Create a hold manager from JSON.
        */
        std::unique_ptr<PublishedHoldCollection> CreatePublishedHoldCollection(
            nlohmann::json data,
            const PersistenceContainer & container
        ) {
            std::unique_ptr<PublishedHoldCollection> collection = std::make_unique<PublishedHoldCollection>();

            // If not object, nothing to do
            if (!data.is_array()) {
                LogWarning("Holding data is invalid");
                return collection;
            }

            // Check valid and add
            for (nlohmann::json::const_iterator it = data.cbegin(); it != data.cend(); ++it) {
                HoldingData holdingData;
                from_json_with_restrictions(*it, holdingData, container);
                if (holdingData == holdSerializerInvalid) {
                    LogWarning("Invalid hold data detected when building hold manager: " + it->dump());
                    continue;
                }

                collection->Add(std::move(holdingData));
            }

            LogInfo("Created Hold Manager with " + std::to_string(collection->Count()) + " holds");
            return collection;
        }
    }  // namespace Hold
}  // namespace UKControllerPlugin
