#include "pch.h"
#include "loader/loaderfunctions.h"

bool DoUpdate()
{
    nlohmann::json versionDetails = GetUpdateDetails();

    if (versionDetails.is_null())
    {
        return false;
    }
}

nlohmann::json GetUpdateDetails()
{
    CURL * curlObject;
    CURLcode result;
    struct curl_slist *curlHeaders = NULL;

    // Set CURL params.
    curlObject = curl_easy_init();
    curl_easy_setopt(curlObject, CURLOPT_URL, "https://ukcp.vatsim.uk/version/latest/github");
    curl_easy_setopt(curlObject, CURLOPT_CUSTOMREQUEST, "GET");

    // Add headers
    std::string outBuffer;
    curl_easy_setopt(curlObject, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(curlObject, CURLOPT_CONNECTTIMEOUT, 4);
    curl_easy_setopt(curlObject, CURLOPT_TIMEOUT, 10);
    curl_easy_setopt(curlObject, CURLOPT_WRITEDATA, &outBuffer);
    curl_easy_setopt(curlObject, CURLOPT_WRITEFUNCTION, &CurlWriteFunction);

    result = curl_easy_perform(curlObject);

    // If we get an error, then let them know
    if (result != CURLE_OK) {
        uint64_t responseCode = 0;
        curl_easy_getinfo(curlObject, CURLINFO_RESPONSE_CODE, &responseCode);
        std::wstring message = L"Unable to check for UK Controller Plugin updates, cURL error.\r\n";
        message += L"Response code was: " + std::to_wstring(responseCode) + L".\r\n";
        message += L"Plugin will attempt to load with previously downloaded version.";

        MessageBox(GetActiveWindow(), message.c_str(), L"UKCP Bootstrap Failed", MB_OK | MB_ICONSTOP);
        curl_easy_cleanup(curlObject);
        return nlohmann::json::value_t::null;
    }

    curl_easy_cleanup(curlObject);

    // Parse the json response
    try
    {
        return nlohmann::json::parse(outBuffer);
    } catch (...) {
        std::wstring message = L"Unable to check for UK Controller Plugin updates, invalid JSON was returned.\r\n";
        message += L"Plugin will attempt to load with previously downloaded version.";

        MessageBox(GetActiveWindow(), message.c_str(), L"UKCP Bootstrap Failed", MB_OK | MB_ICONSTOP);
        return nlohmann::json::value_t::null;
    }
}

size_t CurlWriteFunction(void *contents, size_t size, size_t nmemb, void *outString)
{
    // For Curl, we should assume that the data is not null terminated, so add a null terminator on the end
    ((std::string*)outString)->append(reinterpret_cast<char*>(contents) + '\0', size * nmemb);
    return size * nmemb;
}
