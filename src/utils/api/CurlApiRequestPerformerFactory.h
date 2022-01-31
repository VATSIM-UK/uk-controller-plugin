#pragma once
#include "AbstractApiRequestPerformerFactory.h"

namespace UKControllerPlugin::Curl {
    class CurlInterface;
} // namespace UKControllerPlugin::Curl

namespace UKControllerPluginUtils::Api {
    class ApiCurlRequestFactory;
    class ApiHeaderApplicator;
    class ApiUrlBuilder;

    /**
     * Builds API requests during default running, non-mocked.
     */
    class CurlApiRequestPerformerFactory : public AbstractApiRequestPerformerFactory
    {
        public:
        CurlApiRequestPerformerFactory(std::unique_ptr<UKControllerPlugin::Curl::CurlInterface> curl);
        ~CurlApiRequestPerformerFactory();
        [[nodiscard]] auto Make(const ApiSettings& apiSettings)
            -> ApiRequestPerformerInterface& override;

        private:
        // For cURL requests
        std::unique_ptr<UKControllerPlugin::Curl::CurlInterface> curl;
        
        // Applies headers
        std::unique_ptr<ApiHeaderApplicator> headerApplicator;

        // Builds the URLs
        std::unique_ptr<ApiUrlBuilder> urlBuilder;

        // For making cURL requests
        std::unique_ptr<ApiCurlRequestFactory> curlRequestFactory;

        // Performs requests
        std::unique_ptr<ApiRequestPerformerInterface> performer;
    };
} // namespace UKControllerPluginUtils::Api
