#include "ApiCurlRequestFactory.h"
#include "ApiRequestData.h"
#include "ApiRequestException.h"
#include "CurlApiRequestPerformer.h"
#include "curl/CurlInterface.h"
#include "curl/CurlRequest.h"

using UKControllerPlugin::Curl::CurlInterface;
using UKControllerPlugin::Curl::CurlRequest;
using UKControllerPlugin::Curl::CurlResponse;
using UKControllerPluginUtils::Http::HttpStatusCode;
using UKControllerPluginUtils::Http::IsSuccessful;

namespace UKControllerPluginUtils::Api {

    CurlApiRequestPerformer::CurlApiRequestPerformer(CurlInterface& curl, const ApiCurlRequestFactory& requestFactory)
        : curl(curl), requestFactory(requestFactory)
    {
    }

    auto CurlApiRequestPerformer::Perform(const ApiRequestData& data) -> Response
    {
        const auto request = requestFactory.BuildCurlRequest(data);
        LogDebug(
            "CurlApiRequestPerformer: Performing cURL request with method " + std::string(data.Method()) + " to " +
            data.Uri() + " with body " + data.Body().dump());

        auto curlResponse = curl.MakeCurlRequest(requestFactory.BuildCurlRequest(data));
        if (!ResponseSuccessful(curlResponse)) {
            LogDebug(
                "CurlApiRequestPerformer: Failed cURL request, status was: " +
                std::to_string(curlResponse.GetStatusCode()) + " response body was " + curlResponse.GetResponse());
            throw ApiRequestException(data.Uri(), static_cast<HttpStatusCode>(curlResponse.GetStatusCode()), false);
        }

        return {static_cast<HttpStatusCode>(curlResponse.GetStatusCode()), ParseResponseBody(data, curlResponse)};
    }

    auto CurlApiRequestPerformer::ResponseSuccessful(const CurlResponse& response) -> bool
    {
        return !response.IsCurlError() && IsSuccessful(static_cast<HttpStatusCode>(response.GetStatusCode()));
    }

    auto CurlApiRequestPerformer::ParseResponseBody(const ApiRequestData& data, const CurlResponse& response)
        -> nlohmann::json
    {
        try {
            return nlohmann::json::parse(response.GetResponse());
        } catch (nlohmann::json::exception&) {
            throw ApiRequestException(data.Uri(), static_cast<HttpStatusCode>(response.GetStatusCode()), true);
        }
    }
} // namespace UKControllerPluginUtils::Api
