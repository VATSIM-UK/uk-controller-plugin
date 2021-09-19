#include "CurlApi.h"
#include "CurlRequest.h"
#include "update/PluginVersion.h"

using UKControllerPlugin::Curl::CurlRequest;
using UKControllerPlugin::Curl::CurlResponse;

namespace UKControllerPlugin::Curl {

    CurlApi::CurlApi() : userAgent("UK Controller Plugin/" + std::string(Plugin::PluginVersion::version))
    {
    }

    /*
        Performs a CURL request to the specified URL with the specified post params.
    */
    auto CurlApi::MakeCurlRequest(const CurlRequest& request) -> CurlResponse
    {
        struct curl_slist* curlHeaders = nullptr;

        // Set CURL params.
        CURL* curlObject = curl_easy_init();
        curl_easy_setopt(curlObject, CURLOPT_URL, request.GetUri()); // NOLINT(cppcoreguidelines-pro-type-vararg)
        curl_easy_setopt(                                            // NOLINT(cppcoreguidelines-pro-type-vararg)
            curlObject,
            CURLOPT_CUSTOMREQUEST,
            request.GetMethod());

        // Add headers
        if (request.cbegin() != request.cend()) {
            for (auto it = request.cbegin(); it != request.cend(); ++it) {
                curlHeaders = curl_slist_append(curlHeaders, std::string(it->first + ": " + it->second).c_str());
            }
            curl_easy_setopt(curlObject, CURLOPT_HTTPHEADER, curlHeaders); // NOLINT(cppcoreguidelines-pro-type-vararg)
        }

        curl_easy_setopt( // NOLINT(cppcoreguidelines-pro-type-vararg)
            curlObject,
            CURLOPT_POSTFIELDS,
            request.GetBody());

        std::string outBuffer;
        curl_easy_setopt(curlObject, CURLOPT_FOLLOWLOCATION, 1L); // NOLINT(cppcoreguidelines-pro-type-vararg)
        curl_easy_setopt(curlObject, CURLOPT_CONNECTTIMEOUT, 4);  // NOLINT(cppcoreguidelines-pro-type-vararg)
        curl_easy_setopt(                                         // NOLINT(cppcoreguidelines-pro-type-vararg)
            curlObject,
            CURLOPT_TIMEOUT,
            request.GetMaxRequestTime());
        curl_easy_setopt(curlObject, CURLOPT_WRITEDATA, &outBuffer); // NOLINT(cppcoreguidelines-pro-type-vararg)
        curl_easy_setopt(                                            // NOLINT(cppcoreguidelines-pro-type-vararg)
            curlObject,
            CURLOPT_WRITEFUNCTION,
            &CurlApi::WriteFunction);
        curl_easy_setopt(curlObject, CURLOPT_USERAGENT, userAgent.c_str()); // NOLINT(cppcoreguidelines-pro-type-vararg)

        CURLcode result = curl_easy_perform(curlObject);

        // If we get an error, then throw an exception.
        if (result != CURLE_OK) {
            LogError("cURL Error (" + std::to_string(result) + ")");
            return {"", true, 0};
        }

        // Save a copy of the output buffer and clear the buffer, then clean up.
        uint64_t responseCode = 0;
        curl_easy_getinfo( // NOLINT(cppcoreguidelines-pro-type-vararg)
            curlObject,
            CURLINFO_RESPONSE_CODE,
            &responseCode);
        curl_easy_cleanup(curlObject);

        return {outBuffer, false, responseCode};
    }

    /*
        This function is called by Curl once it has received data to
        add a null terminator and store the data in the correct place.
    */
    auto CurlApi::WriteFunction(void* contents, size_t size, size_t nmemb, void* outString) -> size_t
    {
        // For Curl, we should assume that the data is not null terminated, so add a null terminator on the end
        ((std::string*)outString)->append(reinterpret_cast<char*>(contents), size * nmemb); // NOLINT
        return size * nmemb;
    }
} // namespace UKControllerPlugin::Curl
