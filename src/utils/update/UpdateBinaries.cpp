#include "UpdateBinaries.h"
#include "api/ApiException.h"
#include "api/ApiInterface.h"
#include "curl/CurlInterface.h"
#include "curl/CurlRequest.h"
#include "data/PluginDataLocations.h"
#include "helper/HelperFunctions.h"
#include "windows/WinApi.h"

using UKControllerPlugin::HelperFunctions;
using UKControllerPlugin::Api::ApiException;
using UKControllerPlugin::Curl::CurlRequest;
using UKControllerPlugin::Curl::CurlResponse;

namespace UKControllerPlugin {

    /*
     * Download and update the Updater binary. Do this without a cURL time limit.
     */
    auto DownloadUpdater(nlohmann::json updateData, Windows::WinApiInterface& windows, Curl::CurlInterface& curl)
        -> bool
    {
        LogInfo("Downloading updater library");
        CurlRequest updaterRequest(updateData.at("updater_download_url").get<std::string>(), CurlRequest::METHOD_GET);
        updaterRequest.SetMaxRequestTime(0);
        return UpdateBinary(curl, updaterRequest, windows, GetUpdaterBinaryRelativePath());
    }

    /*
     * Download and update the Core binary. Do this without a cURL time limit.
     */
    auto DownloadCoreLibrary(nlohmann::json updateData, Windows::WinApiInterface& windows, Curl::CurlInterface& curl)
        -> bool
    {
        LogInfo("Downloading core library");
        CurlRequest coreRequest(updateData.at("core_download_url").get<std::string>(), CurlRequest::METHOD_GET);
        coreRequest.SetMaxRequestTime(0);
        return UpdateBinary(curl, coreRequest, windows, GetCoreBinaryRelativePath());
    }

    auto GetUpdateData(const Api::ApiInterface& api) -> nlohmann::json
    {
        try {
            nlohmann::json updateData = api.GetUpdateDetails();
            if (!UpdateDataValid(updateData)) {
                LogError("Unable to get update data, update data was invalid.");
                throw std::exception("Update data response invalid");
            }
            return updateData;
        } catch (ApiException& apiException) {
            LogError("Unable to download update for binaries: " + std::string(apiException.what()));
            throw(apiException);
        }
    }

    auto UpdateBinary(
        Curl::CurlInterface& curl, CurlRequest& request, Windows::WinApiInterface& windows, std::wstring targetFile)
        -> bool
    {
        CurlResponse response = curl.MakeCurlRequest(request);

        if (response.IsCurlError() || !response.StatusOk()) {
            LogError("Error when downloading binary");
            return false;
        }
        
        if (response.GetResponse().empty()) {
            LogError("Error when downloading binary, was empty");
            return false;
        }

        windows.WriteToFile(std::move(targetFile), response.GetResponse(), true, true);
        LogInfo("Binary updated successfully");
        return true;
    }

    auto UpdateDataValid(const nlohmann::json& updateData) -> bool
    {
        return updateData.is_object() && updateData.contains("version") && updateData.at("version").is_string() &&
               updateData.contains("updater_download_url") && updateData.at("updater_download_url").is_string() &&
               updateData.contains("core_download_url") && updateData.at("core_download_url").is_string() &&
               updateData.contains("loader_download_url") && updateData.at("loader_download_url").is_string();
    }
} // namespace UKControllerPlugin
