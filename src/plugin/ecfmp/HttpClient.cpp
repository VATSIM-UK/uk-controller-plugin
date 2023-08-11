#include "HttpClient.h"
#include "curl/CurlInterface.h"
#include "curl/CurlRequest.h"
#include "curl/CurlResponse.h"

namespace UKControllerPlugin::ECFMP {

    HttpClient::HttpClient(Curl::CurlInterface& curl) : curl(curl)
    {
    }

    auto HttpClient::Get(const std::string& url) -> ::ECFMP::Http::HttpResponse
    {
        const auto response = curl.MakeCurlRequest({url, Curl::CurlRequest::METHOD_GET});

        if (response.IsCurlError()) {
            return {CURL_ERROR_STATUS_CODE, ""};
        }

        return {static_cast<long>(response.GetStatusCode()), response.GetResponse()};
    }
} // namespace UKControllerPlugin::ECFMP
