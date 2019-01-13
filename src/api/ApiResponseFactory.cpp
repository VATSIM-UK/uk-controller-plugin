#include "pch/stdafx.h"
#include "api/ApiResponseFactory.h"
#include "helper/HelperFunctions.h"

using UKControllerPlugin::Curl::CurlResponse;
using UKControllerPlugin::HelperFunctions;

namespace UKControllerPlugin {
    namespace Api {
        ApiResponse ApiResponseFactory::Create(UKControllerPlugin::Curl::CurlResponse curl)
        {
            // Predeclare the variables
            std::string responseMessage = "No further details were provided.";

            nlohmann::json responseJson;

            // Make sure we can parse the JSON
            try {
                responseJson = nlohmann::json::parse(curl.GetResponse());
            } catch (nlohmann::json::exception) {
                responseJson = "{}"_json;
            }

            // Set the API response message if one is given.
            if (responseJson.count("message") != 0 && responseJson["message"].is_string()) {
                responseMessage = HelperFunctions::StripQuotesFromJsonString(responseJson["message"].dump());
            }

            return ApiResponse(curl.IsCurlError(), curl.GetStatusCode(), responseJson, responseMessage);
        }
    }  // namespace Api
}  // namespace UKControllerPlugin
