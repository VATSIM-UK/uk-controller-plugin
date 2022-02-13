#pragma once

namespace UKControllerPlugin {
    namespace Api {
        class ApiRequestBuilder;
    } // namespace Api
    namespace Curl {
        class CurlInterface;
    } // namespace Curl
} // namespace UKControllerPlugin

namespace UKControllerPluginUtils::Api {
    class AbstractApiRequestPerformerFactory;
    class ApiRequestFactory;
    class ApiSettings;
    class ApiSettingsProviderInterface;
    class ApiAuthorisationRequestFactory;

    /**
     * Bootstraps and builds the API. Keeps objects alive throughout
     * the plugins lifetime.
     */
    class ApiFactory
    {
        public:
        ApiFactory(
            std::shared_ptr<ApiSettingsProviderInterface> settingsProvider,
            std::shared_ptr<AbstractApiRequestPerformerFactory> requestPerformerFactory);
        ~ApiFactory();
        [[nodiscard]] auto LegacyRequestBuilder() -> const UKControllerPlugin::Api::ApiRequestBuilder&;
        [[nodiscard]] auto RequestFactory() -> ApiRequestFactory&;
        [[nodiscard]] auto SettingsProvider() -> const std::shared_ptr<ApiSettingsProviderInterface>;

        private:
        // Loads api settings - can be subbed out for a mock.
        std::shared_ptr<ApiSettingsProviderInterface> settingsProvider;

        // Starts performing requests - can be subbed out for a mock.
        std::shared_ptr<AbstractApiRequestPerformerFactory> requestPerformerFactory;

        // Builds API requests
        std::unique_ptr<ApiRequestFactory> requestFactory;

        // Builds API requests, the legacy way
        std::unique_ptr<UKControllerPlugin::Api::ApiRequestBuilder> legacyRequestBuilder;
    };
} // namespace UKControllerPluginUtils::Api
