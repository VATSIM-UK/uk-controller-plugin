#include "utils/pch.h"
#include "api/ApiBootstrap.h"
#include "api/ApiHelper.h"
#include "api/ApiRequestBuilder.h"
#include "curl/CurlApi.h"
#include "setting/SettingRepository.h"

namespace UKControllerPlugin {
    namespace Api {
        std::unique_ptr<ApiInterface> Bootstrap(const Setting::SettingRepository& settings, Curl::CurlInterface& curl)
        {
            std::string apiUrl = settings.GetSetting("api-url", "https://ukcp.vatsim.uk");
            LogInfo("API bootstrapped with URL " + apiUrl);
            ApiRequestBuilder requestBuilder(
                apiUrl,
                settings.GetSetting("api-key")
            );
            return std::make_unique<ApiHelper>(curl, requestBuilder);
        }
    } // namespace Api
} // namespace UKControllerPlugin
