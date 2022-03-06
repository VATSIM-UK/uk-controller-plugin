#include "api/ApiSettings.h"
#include "helper/ApiRequestHelperFunctions.h"

using UKControllerPlugin::Api::ApiRequestBuilder;
using UKControllerPlugin::Curl::CurlRequest;
using UKControllerPluginUtils::Api::ApiSettings;

const std::string mockApiUrl = "http://ukcp.test.com";
const std::string mockApiKey = "areallyniceapikey";
std::shared_ptr<ApiSettings> settings;

/*
    Builds an expected cURL request
*/
CurlRequest GetApiCurlRequest(std::string route, std::string method, nlohmann::json body)
{
    CurlRequest request(mockApiUrl + route, method);
    request.SetBody(body.dump());
    request.AddHeader("Authorization", "Bearer " + mockApiKey);
    request.AddHeader("Accept", "application/json");
    request.AddHeader("Content-Type", "application/json");
    return request;
}

/*
    Builds an expected cURL request with no body
*/
CurlRequest GetApiCurlRequest(std::string route, std::string method)
{
    CurlRequest request(mockApiUrl + route, method);
    request.AddHeader("Authorization", "Bearer " + mockApiKey);
    request.AddHeader("Accept", "application/json");
    request.AddHeader("Content-Type", "application/json");
    return request;
}

/*
    Builds an expected cURL request with no body
*/
CurlRequest GetApiGetUriCurlRequest(std::string route, std::string method)
{
    CurlRequest request(route, method);
    request.AddHeader("Authorization", "Bearer " + mockApiKey);
    request.AddHeader("Accept", "application/json");
    request.AddHeader("Content-Type", "application/json");
    return request;
}

/*
    Returns an API Request Builder
*/
ApiRequestBuilder GetApiRequestBuilder()
{
    if (!settings) {
        settings = std::make_shared<ApiSettings>(mockApiUrl, mockApiKey);
    }

    return ApiRequestBuilder(*settings);
}
