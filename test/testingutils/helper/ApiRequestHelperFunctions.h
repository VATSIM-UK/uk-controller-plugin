#pragma once
#include "curl/CurlRequest.h"
#include "api/ApiRequestBuilder.h"

extern const std::string mockApiUrl;
extern const std::string mockApiKey;

/*
    Builds an expected cURL request
*/
UKControllerPlugin::Curl::CurlRequest GetApiCurlRequest(std::string route, std::string method);
UKControllerPlugin::Curl::CurlRequest GetApiCurlRequest(std::string route, std::string method, nlohmann::json body);
UKControllerPlugin::Curl::CurlRequest GetApiGetUriCurlRequest(std::string route, std::string method);

/*
    Returns an API Request Builder
*/
UKControllerPlugin::Api::ApiRequestBuilder GetApiRequestBuilder();
