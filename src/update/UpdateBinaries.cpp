#include "utils/pch.h"
#include "update/UpdateBinaries.h"
#include "api/ApiInterface.h"
#include "windows/WinApi.h"
#include "curl/CurlInterface.h"
#include "api/ApiException.h"
#include "curl/CurlResponse.h"
#include "curl/CurlRequest.h"
#include "data/PluginDataLocations.h"
#include "helper/HelperFunctions.h"

using UKControllerPlugin::Api::ApiException;
using UKControllerPlugin::Curl::CurlResponse;
using UKControllerPlugin::Curl::CurlRequest;
using UKControllerPlugin::HelperFunctions;

namespace UKControllerPlugin {

    bool UKControllerPlugin::UpdateBinaries(
        nlohmann::json updateData,
        Windows::WinApiInterface& windows,
        Curl::CurlInterface& curl
    )
    {
        if (!UpdateDataValid(updateData)) {
            LogError("Unable to update binaries, update data was invalid.");
            return false;
        }

        CurlRequest coreRequest(
            updateData.at("core_download_url").get<std::string>(),
            CurlRequest::METHOD_GET
        );
        bool coreUpdated = UpdateBinary(curl, coreRequest, windows, GetCoreBinaryRelativePath());

        CurlRequest updaterRequest(
            updateData.at("updater_download_url").get<std::string>(),
            CurlRequest::METHOD_GET
        );
        bool updaterUpdated = UpdateBinary(curl, coreRequest, windows, GetCoreBinaryRelativePath());

        return coreUpdated && updaterUpdated;
    }

    nlohmann::json GetUpdateData(const Api::ApiInterface& api)
    {
        try {
            return api.GetUpdateDetails();
        } catch (ApiException apiException) {
            LogError("Unable to download update for binaries: " + std::string(apiException.what()));
            return nlohmann::json::object();
        }
    }

    bool UpdateBinary(
        Curl::CurlInterface& curl,
        CurlRequest& request,
        Windows::WinApiInterface& windows,
        std::wstring targetFile
    )
    {
        CurlResponse response = curl.MakeCurlRequest(request);

        if (response.IsCurlError() || response.GetStatusCode() != 200L) {
            return false;
        }

        windows.WriteToFile(targetFile, response.GetResponse(), true);
        return true;
    }

    bool UpdateDataValid(const nlohmann::json& updateData)
    {
        return updateData.is_object() &&
            updateData.contains("version") &&
            updateData.at("version").is_string() &&
            updateData.contains("updater_download_url") &&
            updateData.at("updater_download_url").is_string() &&
            updateData.contains("core_download_url") &&
            updateData.at("core_download_url").is_string();
    }
} // namespace UKControllerPlugin
