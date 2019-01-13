#include "pch/pch.h"
#include "helper/ApiRequestHelperFunctions.h"

using UKControllerPlugin::Curl::CurlRequest;
using UKControllerPlugin::Api::ApiRequestBuilder;

const std::string mockApiUrl = "http://uckp.test.com";
const std::string mockApiKey = "areallyniceapikey";

/*
    Builds an expected cURL request
*/
CurlRequest GetApiCurlRequest(
    std::string route,
    std::string method,
    nlohmann::json body
) {
    CurlRequest request(mockApiUrl + route, method);
    request.SetBody(body.dump());
    request.AddHeader("Authorization", "Bearer " + mockApiKey);
    request.AddHeader("Accept", "application/json");
    return request;
}

/*
    Builds an expected cURL request with no body
*/
CurlRequest GetApiCurlRequest(
    std::string route,
    std::string method
) {
    CurlRequest request(mockApiUrl + route, method);
    request.AddHeader("Authorization", "Bearer " + mockApiKey);
    request.AddHeader("Accept", "application/json");
    return request;
}

/*
    Returns an API Request Builder
*/
ApiRequestBuilder GetApiRequestBuilder(void)
{
    return ApiRequestBuilder(mockApiUrl, mockApiKey);
}
