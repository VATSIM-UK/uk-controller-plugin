#include "ApiKeyRedirectUrlBuilder.h"
#include "ApiSettings.h"

namespace UKControllerPluginUtils::Api {

    struct ApiKeyRedirectUrlBuilder::Impl
    {
        Impl(const ApiSettings& settings) : settings(settings), curlHandle(curl_easy_init())
        {
        }

        ~Impl()
        {
            curl_easy_cleanup(curlHandle);
        }

        // The API settings
        const ApiSettings& settings;

        // Curl handle
        CURL* curlHandle;
    };

    ApiKeyRedirectUrlBuilder::ApiKeyRedirectUrlBuilder(const ApiSettings& settings)
        : impl(std::make_unique<Impl>(settings))
    {
    }

    ApiKeyRedirectUrlBuilder::~ApiKeyRedirectUrlBuilder() = default;

    auto ApiKeyRedirectUrlBuilder::BuildUrl(const std::string& host, int port) const -> std::string
    {
        const auto redirectUrl = "http://" + host + ":" + std::to_string(port);
        return impl->settings.Url() + "/user-create-api-key/?redirect=" +
               curl_easy_escape(impl->curlHandle, redirectUrl.c_str(), static_cast<int>(redirectUrl.size()));
    }
} // namespace UKControllerPluginUtils::Api
