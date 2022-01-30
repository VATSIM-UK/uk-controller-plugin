#include "ApiCurlRequestFactory.h"
#include "ApiException.h"
#include "ApiRequestPerformer.h"
#include "curl/CurlInterface.h"
#include "curl/CurlRequest.h"

using UKControllerPlugin::Api::ApiException;
using UKControllerPlugin::Curl::CurlInterface;
using UKControllerPlugin::Curl::CurlRequest;
using UKControllerPlugin::Curl::CurlResponse;
using UKControllerPluginUtils::Http::HttpStatusCode;

namespace UKControllerPluginUtils::Api {

    ApiRequestPerformer::ApiRequestPerformer(CurlInterface& curl, const ApiCurlRequestFactory& requestFactory)
        : curl(curl), requestFactory(requestFactory)
    {
    }

    auto ApiRequestPerformer::Perform(const ApiRequestData& data) -> Response
    {
        auto curlResponse = curl.MakeCurlRequest(requestFactory.BuildCurlRequest(data));
        if (!ResponseSuccessful(curlResponse)) {
            throw ApiException("Api returned status code: " + std::to_string(curlResponse.GetStatusCode()));
        }

        return {static_cast<HttpStatusCode>(curlResponse.GetStatusCode()), ParseResponseBody(curlResponse)};
    }

    auto ApiRequestPerformer::ResponseSuccessful(const CurlResponse& response) -> bool
    {
        return !response.IsCurlError() &&
               (response.GetStatusCode() == HttpStatusCode::Ok || response.GetStatusCode() == HttpStatusCode::Created ||
                response.GetStatusCode() == HttpStatusCode::NoContent);
    }

    auto ApiRequestPerformer::ParseResponseBody(const CurlResponse& response) -> nlohmann::json
    {
        try {
            return nlohmann::json::parse(response.GetResponse());
        } catch (nlohmann::json::exception&) {
            throw ApiException("Invalid API response body");
        }
    }
} // namespace UKControllerPluginUtils::Api