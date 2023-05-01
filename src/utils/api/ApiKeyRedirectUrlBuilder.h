#pragma once

namespace UKControllerPluginUtils::Api {

    class ApiSettings;

    class ApiKeyRedirectUrlBuilder
    {
        public:
        ApiKeyRedirectUrlBuilder(const ApiSettings& settings);
        ~ApiKeyRedirectUrlBuilder();
        [[nodiscard]] auto BuildUrl(const std::string& host, int port) const -> std::string;

        private:
        // The API settings
        const ApiSettings& settings;

        // Curl handle
        CURL* curlHandle;
    };
} // namespace UKControllerPluginUtils::Api
