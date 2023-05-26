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
        struct Impl;
        std::unique_ptr<Impl> impl;
    };
} // namespace UKControllerPluginUtils::Api
