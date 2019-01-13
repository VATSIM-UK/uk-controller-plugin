#include "pch/stdafx.h"
#include "curl/CurlApi.h"
#include "curl/CurlRequest.h"
#include "curl/curl.h"

using UKControllerPlugin::Curl::CurlRequest;
using UKControllerPlugin::Curl::CurlResponse;

namespace UKControllerPlugin {
    namespace Curl {

        /*
            Performs a CURL request to the specified URL with the specified post params.
        */
        UKControllerPlugin::Curl::CurlResponse CurlApi::MakeCurlRequest(const CurlRequest & request) {
            CURL * curlObject;
            CURLcode result;
            struct curl_slist *curlHeaders = NULL;

            // Set CURL params.
            curlObject = curl_easy_init();
            curl_easy_setopt(curlObject, CURLOPT_URL, request.GetUri());
            curl_easy_setopt(curlObject, CURLOPT_CUSTOMREQUEST, request.GetMethod());

            // Add headers
            if (request.cbegin() != request.cend()) {
                for (CurlRequest::HttpHeaders::const_iterator it = request.cbegin(); it != request.cend(); ++it) {
                    curlHeaders = curl_slist_append(curlHeaders, std::string(it->first + ": " + it->second).c_str());
                }
                curl_easy_setopt(curlObject, CURLOPT_HTTPHEADER, curlHeaders);
            }

            curl_easy_setopt(curlObject, CURLOPT_POSTFIELDS, request.GetBody());

            std::string outBuffer;
            curl_easy_setopt(curlObject, CURLOPT_FOLLOWLOCATION, 1L);
            curl_easy_setopt(curlObject, CURLOPT_TIMEOUT, 3);
            curl_easy_setopt(curlObject, CURLOPT_WRITEDATA, &outBuffer);
            curl_easy_setopt(curlObject, CURLOPT_WRITEFUNCTION, &CurlApi::WriteFunction);

            result = curl_easy_perform(curlObject);

            // If we get an error, then throw an exception.
            if (result != CURLE_OK) {
                return CurlResponse("", true, -1);
            }

            // Save a copy of the output buffer and clear the buffer, then clean up.
            uint64_t responseCode = 0;
            curl_easy_getinfo(curlObject, CURLINFO_RESPONSE_CODE, &responseCode);
            curl_easy_cleanup(curlObject);

            return CurlResponse(outBuffer, false, responseCode);
        }

        /*
            This function is called by Curl once it has received data to
            add a null terminator and store the data in the correct place.
        */
        size_t CurlApi::WriteFunction(void *contents, size_t size, size_t nmemb, void *outString)
        {
            // For Curl, we should assume that the data is not null terminated, so add a null terminator on the end
            ((std::string*)outString)->append(reinterpret_cast<char*>(contents) + '\0', size * nmemb);
            return size * nmemb;
        }
    }  // namespace Curl
}  // namespace UKControllerPlugin
