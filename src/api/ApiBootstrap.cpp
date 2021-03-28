#include "utils/pch.h"
#include "api/ApiBootstrap.h"
#include "ApiHelper.h"
#include "api/ApiRequestBuilder.h"
#include "curl/CurlApi.h"
#include "setting/SettingRepository.h"

namespace UKControllerPlugin {
    namespace Api {
        std::unique_ptr<ApiInterface> Bootstrap(const Setting::SettingRepository& settings, Curl::CurlInterface& curl)
        {
            ApiRequestBuilder requestBuilder(
                settings.GetSetting("api-url", "https://ukcp.vatsim.uk"),
                settings.GetSetting("api-key")
            );
            return std::make_unique<ApiHelper>(curl, requestBuilder);
        }
    } // namespace Api
} // namespace UKControllerPlugin
