#include "pch/stdafx.h"
#include "hold/HoldDependency.h"
#include "api/ApiInterface.h"
#include "windows/WinApi.h"
#include "api/ApiException.h"

using UKControllerPlugin::Api::ApiInterface;
using UKControllerPlugin::Windows::WinApiInterface;
using UKControllerPlugin::Api::ApiException;

namespace UKControllerPlugin {
    namespace Hold {

        const std::string dependencyLocation = "dependencies/holds.json";

        const std::string defaultValue = "[]";

        /*
            Updates the hold data on disk with new data from the API.
        */
        void UpdateHoldDependency(
            const ApiInterface & api,
            WinApiInterface & winApi
        ) {
            if (!winApi.FileExists(dependencyLocation)) {
                winApi.WriteToFile(dependencyLocation, defaultValue, true);
            }

            try {
                winApi.WriteToFile(dependencyLocation, api.GetHoldDependency().dump(), true);
            } catch (ApiException exception) {
                LogError("ApiException when updating hold data: " + std::string(exception.what()));
                return;
            }

            LogInfo("Updated holding data");
        }

        /*
            Returns the hold data from disk
        */
        nlohmann::json GetLocalHoldData(WinApiInterface & winApi)
        {
            if (!winApi.FileExists(dependencyLocation)) {
                return nlohmann::json::parse(defaultValue);
            }

            try {
                return nlohmann::json::parse(winApi.ReadFromFile(dependencyLocation));
            } catch (nlohmann::json::exception) {
                LogError("Unable to read hold data from local storage, invalid JSON");
            }

            return nlohmann::json::parse(defaultValue);
        }

    }  // namespace Hold
}  // namespace UKControllerPlugin
