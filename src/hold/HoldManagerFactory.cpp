#include "pch/stdafx.h"
#include "hold/HoldManagerFactory.h"
#include "hold/ManagedHold.h"
#include "hold/HoldingDataSerializer.h"
using UKControllerPlugin::Hold::HoldManager;
using UKControllerPlugin::Bootstrap::PersistenceContainer;

namespace UKControllerPlugin {
    namespace Hold {

        /*
            Create a hold manager from JSON.
        */
        std::unique_ptr<HoldManager> CreateHoldManager(nlohmann::json data, const PersistenceContainer & container)
        {
            std::unique_ptr<HoldManager> holdManager = std::make_unique<HoldManager>();

            // If not object, nothing to do
            if (!data.is_array()) {
                LogWarning("Holding data is invalid");
                return holdManager;
            }

            // Check valid and add
            for (nlohmann::json::const_iterator it = data.cbegin(); it != data.cend(); ++it) {
                HoldingData holdingData;
                from_json_with_restrictions(*it, holdingData, container);
                if (holdingData == holdSerializerInvalid) {
                    LogWarning("Invalid hold data detected when building hold manager: " + it->dump());
                    continue;
                }

                holdManager->AddHold(ManagedHold(std::move(holdingData)));
            }

            LogInfo("Created Hold Manager with " + std::to_string(holdManager->CountHolds()) + " holds");
            return holdManager;
        }
    }  // namespace Hold
}  // namespace UKControllerPlugin
