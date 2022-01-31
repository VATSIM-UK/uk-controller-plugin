#pragma once

namespace UKControllerPlugin {
    namespace Curl {
        class CurlInterface;
    } // namespace Curl
    namespace Setting {
        class SettingRepository;
    } // namespace Setting
} // namespace UKControllerPlugin

namespace UKControllerPluginUtils::Api {
    class AbstractApiRequestPerformerFactory;
    class ApiRequestFactory;
    class ApiSettings;

    /**
     * Bootstraps and builds the API. Keeps objects alive throughout
     * the plugins lifetime.
     */
    class ApiFactory
    {
        public:
        ApiFactory(std::shared_ptr<AbstractApiRequestPerformerFactory> requestPerformerFactory, bool async);
        ~ApiFactory();
        [[nodiscard]] auto RequestFactory(const UKControllerPlugin::Setting::SettingRepository& settings)
            -> const ApiRequestFactory&;
        [[nodiscard]] auto Settings(const UKControllerPlugin::Setting::SettingRepository& settings) -> ApiSettings&;

        private:
        // Starts performing requests - can be subbed out for a mock.
        std::shared_ptr<AbstractApiRequestPerformerFactory> requestPerformerFactory;

        // Should api requests run async by default. Can be overriden.
        bool async;

        // The API settings
        std::unique_ptr<ApiSettings> apiSettings;

        // Builds API requests
        std::unique_ptr<ApiRequestFactory> requestFactory;
    };
} // namespace UKControllerPluginUtils::Api
