#include "api/ApiResponseFactory.h"
#include "helper/HelperFunctions.h"

using UKControllerPlugin::HelperFunctions;
using UKControllerPlugin::Curl::CurlResponse;

namespace UKControllerPlugin {
    namespace Api {
        ApiResponse ApiResponseFactory::Create(UKControllerPlugin::Curl::CurlResponse curl)
        {
            // Predeclare the variables
            std::string responseMessage = "No further details were provided.";

            nlohmann::json responseJson;

            // Make sure we can parse the JSON
            try {
                // If 204, we aren't expecting a body
                if (curl.GetStatusCode() != 204L) {
                    responseJson = nlohmann::json::parse(curl.GetResponse());
                }
            } catch (nlohmann::json::exception) {
                LogError("Error processing API response, JSON was invalid");
                responseJson = "{}"_json;
            }

            // Set the API response message if one is given.
            if (responseJson.count("message") != 0 && responseJson["message"].is_string()) {
                responseMessage = HelperFunctions::StripQuotesFromJsonString(responseJson["message"].dump());
            }

            return ApiResponse(curl.IsCurlError(), curl.GetStatusCode(), responseJson, responseMessage);
        }
    } // namespace Api
} // namespace UKControllerPlugin
