#include "pch/stdafx.h"
#include "hold/HoldProfileManagerFactory.h"
#include "hold/HoldProfile.h"
#include "hold/HoldProfileSerializer.h"
#include "api/ApiInterface.h"

using UKControllerPlugin::Hold::HoldProfileManager;
using UKControllerPlugin::Api::ApiInterface;

namespace UKControllerPlugin {
    namespace Hold {

        /*
            Create a hold profile manager from JSON.
        */
        std::unique_ptr<HoldProfileManager> CreateHoldProfileManager(nlohmann::json data, const ApiInterface & api)
        {
            std::unique_ptr<HoldProfileManager> profileManager = std::make_unique<HoldProfileManager>(api);

            // If not object, nothing to do
            if (!data.is_array()) {
                LogWarning("Hold profile data is invalid");
                return profileManager;
            }

            // Check valid and add
            for (nlohmann::json::const_iterator it = data.cbegin(); it != data.cend(); ++it) {
                HoldProfile profile = it->get<HoldProfile>();
                if (profile == holdProfileSerializerInvalid) {
                    LogWarning("Invalid hold profile detected when building hold profile manager: " + it->dump());
                    continue;
                }

                profileManager->AddProfile(profile);
            }

            LogInfo("Loaded " + std::to_string(profileManager->CountProfiles()) + " hold profiles");
            return profileManager;
        }
    }  // namespace Hold
}  // namespace UKControllerPlugin
