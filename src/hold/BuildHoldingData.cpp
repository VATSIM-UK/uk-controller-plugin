#include "pch/stdafx.h"
#include "hold/BuildHoldingData.h"
#include "hold/ManagedHold.h"
#include "hold/HoldingDataSerializer.h"
using UKControllerPlugin::Hold::HoldManager;

namespace UKControllerPlugin {
    namespace Hold {

        /*
            Convert JSON data into a collection of holds.
        */
        std::unique_ptr<HoldManager> BuildHoldingData(nlohmann::json data)
        {
            std::unique_ptr<HoldManager> holdManager = std::make_unique<HoldManager>();

            // If not object, nothing to do
            if (!data.is_array()) {
                LogWarning("Holding data is invalid");
                return holdManager;
            }

            // Check valid and add
            for (nlohmann::json::const_iterator it = data.cbegin(); it != data.cend(); ++it) {
                HoldingData data = it->get<HoldingData>();
                if (data == holdSerializerInvalid) {
                    LogWarning("Invalid hold data detected: " + it->dump());
                    continue;
                }

                holdManager->AddHold(ManagedHold(data));
            }

            LogInfo("Added holding data for " + std::to_string(holdManager->CountHolds()) + " holds");
            return holdManager;
        }
    }  // namespace Hold
}  // namespace UKControllerPlugin
