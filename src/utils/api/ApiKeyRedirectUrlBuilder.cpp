#include "ApiKeyRedirectUrlBuilder.h"
#include "ApiSettings.h"

namespace UKControllerPluginUtils::Api {

    ApiKeyRedirectUrlBuilder::ApiKeyRedirectUrlBuilder(const ApiSettings& settings)
        : settings(settings), curlHandle(curl_easy_init())
    {
    }

    ApiKeyRedirectUrlBuilder::~ApiKeyRedirectUrlBuilder()
    {
        curl_easy_cleanup(curlHandle);
    };

    auto ApiKeyRedirectUrlBuilder::BuildUrl(const std::string& host, int port) const -> std::string
    {
        const auto redirectUrl = host + ":" + std::to_string(port);
        return settings.Url() + "?redirect=" + curl_easy_escape(curlHandle, redirectUrl.c_str(), redirectUrl.size());
    }
} // namespace UKControllerPluginUtils::Api
