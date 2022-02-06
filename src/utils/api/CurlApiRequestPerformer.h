#pragma once
#include "api/ApiRequestPerformerInterface.h"

namespace UKControllerPlugin::Curl {
    class CurlInterface;
    class CurlResponse;
} // namespace UKControllerPlugin::Curl

namespace UKControllerPluginUtils::Api {
    class ApiCurlRequestFactory;

    /**
     * Performs API requests.
     */
    class CurlApiRequestPerformer : public ApiRequestPerformerInterface
    {
        public:
        CurlApiRequestPerformer(
            UKControllerPlugin::Curl::CurlInterface& curl, const ApiCurlRequestFactory& requestFactory);
        auto Perform(const ApiRequestData& data) -> Response override;

        private:
        [[nodiscard]] static auto ResponseSuccessful(const UKControllerPlugin::Curl::CurlResponse& response) -> bool;
        [[nodiscard]] static auto
        ParseResponseBody(const ApiRequestData& data, const UKControllerPlugin::Curl::CurlResponse& response)
            -> nlohmann::json;

        // For making the cURL requests
        UKControllerPlugin::Curl::CurlInterface& curl;

        // Settings for the API
        const ApiCurlRequestFactory& requestFactory;
    };
} // namespace UKControllerPluginUtils::Api
