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
            std::shared_ptr<AbstractApiRequestPerformerFactory> requestPerformerFactory,
            bool async);
        ~ApiFactory();
        [[nodiscard]] auto LegacyRequestBuilder() -> const UKControllerPlugin::Api::ApiRequestBuilder&;
        [[nodiscard]] auto RequestFactory() -> const ApiRequestFactory&;
        [[nodiscard]] auto Settings() -> ApiSettings&;
        [[nodiscard]] auto AuthorisationRequestFactory() -> const ApiAuthorisationRequestFactory&;

        private:
        
        //Loads api settings - can be subbed out for a mock.
        std::shared_ptr<ApiSettingsProviderInterface> settingsProvider;

        // Starts performing requests - can be subbed out for a mock.
        std::shared_ptr<AbstractApiRequestPerformerFactory> requestPerformerFactory;

        // Should api requests run async by default. Can be overriden.
        bool async;

        // The API settings
        std::unique_ptr<ApiSettings> apiSettings;

        // Builds API requests
        std::unique_ptr<ApiRequestFactory> requestFactory;

        // Builds API requests, the legacy way
        std::unique_ptr<UKControllerPlugin::Api::ApiRequestBuilder> legacyRequestBuilder;
        
        // Provides a way to check API authorisation
        std::unique_ptr<ApiAuthorisationRequestFactory> authorisationRequestFactory;
    };
} // namespace UKControllerPluginUtils::Api
