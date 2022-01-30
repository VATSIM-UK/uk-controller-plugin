#pragma once
#include "curl/CurlRequest.h"

namespace UKControllerPluginUtils::Api {
    class ApiRequestData;
    class ApiUrlBuilder;
    class ApiHeaderApplicator;

    /**
     * Creates a cURL request from an ApiRequest
     */
    class ApiCurlRequestFactory
    {
        public:
        ApiCurlRequestFactory(const ApiUrlBuilder& urlBuilder, const ApiHeaderApplicator& headerApplicator);
        [[nodiscard]] auto BuildCurlRequest(const ApiRequestData& data) const -> UKControllerPlugin::Curl::CurlRequest;

        private:
        // Builds URLs
        const ApiUrlBuilder& urlBuilder;

        // Applies headers
        const ApiHeaderApplicator& headerApplicator;
    };
} // namespace UKControllerPluginUtils::Api
