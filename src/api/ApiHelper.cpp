#include "pch/stdafx.h"
#include "api/ApiHelper.h"
#include "curl/CurlInterface.h"
#include "api/ApiException.h"
#include "api/ApiNotFoundException.h"
#include "api/ApiNotAuthorisedException.h"
#include "api/ApiResponseFactory.h"
#include "curl/CurlRequest.h"
#include "curl/CurlResponse.h"
#include "squawk/SquawkValidator.h"
#include "windows/WinApiInterface.h"

using UKControllerPlugin::Api::ApiException;
using UKControllerPlugin::Curl::CurlResponse;
using UKControllerPlugin::Curl::CurlRequest;
using UKControllerPlugin::Curl::CurlInterface;
using UKControllerPlugin::Api::ApiResponseFactory;
using UKControllerPlugin::Api::ApiRequestBuilder;
using UKControllerPlugin::Api::ApiNotFoundException;
using UKControllerPlugin::Api::ApiNotAuthorisedException;
using UKControllerPlugin::Squawk::SquawkValidator;
using UKControllerPlugin::Windows::WinApiInterface;
using UKControllerPlugin::Squawk::ApiSquawkAllocation;

namespace UKControllerPlugin {
    namespace Api {

        ApiHelper::ApiHelper(
            CurlInterface & curlApi,
            ApiRequestBuilder requestBuilder,
            WinApiInterface & winApi
        ) : curlApi(curlApi), requestBuilder(requestBuilder), winApi(winApi)
        {

        }

        /*
            Makes a request to the API.
        */
        ApiResponse ApiHelper::MakeApiRequest(const CurlRequest request) const
        {
            CurlResponse response = this->curlApi.MakeCurlRequest(request);

            if (response.IsCurlError()) {
                LogError("cURL error when making API request, route: " + std::string(request.GetUri()));
                throw ApiException("ApiException when calling " + std::string(request.GetUri()));
            }

            if (response.GetStatusCode() == this->STATUS_SERVER_ERROR ||
                response.GetStatusCode() == this->STATUS_SERVICE_UNAVAILBLE) {
                LogError("Internal server error when calling " + std::string(request.GetUri()));
                throw ApiException("ApiException, internal server error");
            }

            if (response.GetStatusCode() == this->STATUS_UNAUTHORISED ||
                response.GetStatusCode() == this->STATUS_FORBIDDEN) {
                LogError("The API returned unauthorised when calling " + std::string(request.GetUri()));
                throw ApiNotAuthorisedException("The API returned 401 or 403");
            }

            if (response.GetStatusCode() == this->STATUS_BAD_REQUEST) {
                LogError("The API responed with bad request when calling " + std::string(request.GetUri()));
                throw ApiException("The API returned 400");
            }

            if (response.GetStatusCode() == this->STATUS_NOT_FOUND) {
                throw ApiNotFoundException("The API returned 404 for " + std::string(request.GetUri()));
            }

            // These are the only codes the API should be sending on success
            if (
                response.GetStatusCode() != this->STATUS_CREATED &&
                response.GetStatusCode() != this->STATUS_NO_CONTENT &&
                response.GetStatusCode() != this->STATUS_OK
            ) {
                LogError("Unknown API response occured, HTTP status was " + std::to_string(response.GetStatusCode()));
                throw ApiException("Unknown response");
            }

            return ApiResponseFactory::Create(response);
        }

        ApiSquawkAllocation ApiHelper::ProcessSquawkResponse(const ApiResponse response, std::string callsign) const
        {
            nlohmann::json responseJson = response.GetRawData();

            if (responseJson.count("squawk") != 1 || !responseJson["squawk"].is_string()) {
                LogError("No squawk in API response for " + callsign);
                throw ApiException("Invalid response returned from API");
            }

            if (!SquawkValidator::ValidSquawk(responseJson["squawk"])) {
                LogError("Invalid API response when requesting squawk assignment for " + callsign);
                throw ApiException("Invalid response returned from API");
            }

            // We should return false here, because if the API has a bad squawk assigned, we should try replacing it
            if (!SquawkValidator::AllowedSquawk(responseJson["squawk"])) {
                LogError("API returned invalid squawk when requesting squawk assignment for " + callsign);
                throw ApiException("Invalid squawk returned from API");
            }

            return ApiSquawkAllocation{ callsign, responseJson["squawk"] };
        }

        /*
            Creates or updates a general squawk assignment (generates a new squawk for the aircraft).
        */
        ApiSquawkAllocation ApiHelper::CreateGeneralSquawkAssignment(
            std::string callsign,
            std::string origin,
            std::string destination
        ) const {

            return this->ProcessSquawkResponse(
                this->MakeApiRequest(
                    this->requestBuilder.BuildGeneralSquawkAssignmentRequest(callsign, origin, destination)
                ),
                callsign
            );
        }

        /*
            Creates or updates a local squawk assignment (generates a new squawk for the aircraft).
        */
        ApiSquawkAllocation ApiHelper::CreateLocalSquawkAssignment(
            std::string callsign,
            std::string unit,
            std::string flightRules
        ) const {
            return this->ProcessSquawkResponse(
                this->MakeApiRequest(
                    this->requestBuilder.BuildLocalSquawkAssignmentRequest(callsign, unit, flightRules)
                ),
                callsign
            );
        }

        /*
            Authorise connection to a websocket channel and return the authorisation code.
        */
        std::string ApiHelper::AuthoriseWebsocketChannel(std::string socketId, std::string channel) const
        {
            nlohmann::json response = this->MakeApiRequest(
                this->requestBuilder.BuildWebsocketChannelAuthRequest(socketId, channel)
            ).GetRawData();

            if (!response.count("auth") || !response.at("auth").is_string()) {
                throw ApiException("Authorsation response missing valid token");
            }

            return response.at("auth");
        }

        /*
            Hits the API root to find out whether we're allowed in.
        */
        bool ApiHelper::CheckApiAuthorisation(void) const
        {
            return this->MakeApiRequest(this->requestBuilder.BuildAuthCheckRequest()).GetStatusCode() ==
                this->STATUS_OK;
        }

        /*
            De allocate a squawk from a given aircraft.
        */
        void ApiHelper::DeleteSquawkAssignment(std::string callsign) const
        {
            this->MakeApiRequest(this->requestBuilder.BuildSquawkAssignmentDeletionRequest(callsign));
        }

        /*
            Fetches the file at the given URI.
        */
        std::string ApiHelper::FetchRemoteFile(std::string uri) const
        {
            return this->MakeApiRequest(this->requestBuilder.BuildRemoteFileRequest(uri)).GetRawData().dump();
        }

        /*
            Get any currently assigned squawk for the aircraft
        */
        ApiSquawkAllocation ApiHelper::GetAssignedSquawk(std::string callsign) const
        {
            return this->ProcessSquawkResponse(
                this->MakeApiRequest(
                    this->requestBuilder.BuildSquawkAssignmentCheckRequest(callsign)
                ),
                callsign
            );
        }

        /*
            Returns the API domain being used by the request builder
        */
        std::string ApiHelper::GetApiDomain(void) const
        {
            return this->requestBuilder.GetApiDomain();
        }

        /*
            Returns the API key being used to authenticate requests
        */
        std::string ApiHelper::GetApiKey(void) const
        {
            return this->requestBuilder.GetApiKey();
        }

        nlohmann::json ApiHelper::GetDependencyList(void) const
        {
            return this->MakeApiRequest(this->requestBuilder.BuildDependencyListRequest()).GetRawData();
        }

        /*
            Returns the hold data dependency
        */
        nlohmann::json ApiHelper::GetHoldDependency(void) const
        {
            return this->MakeApiRequest(this->requestBuilder.BuildHoldDependencyRequest()).GetRawData();
        }

        /*
            Download the generic hold profiles JSON
        */
        nlohmann::json ApiHelper::GetGenericHoldProfiles(void) const
        {
            return this->MakeApiRequest(this->requestBuilder.BuildUserHoldProfilesRequest()).GetRawData();
        }

        /*
            Download the user hold profiles JSON
        */
        nlohmann::json ApiHelper::GetUserHoldProfiles(void) const
        {
            return this->MakeApiRequest(this->requestBuilder.BuildUserHoldProfilesRequest()).GetRawData();
        }

        /*
            Request all the min stack levels
        */
        nlohmann::json ApiHelper::GetMinStackLevels(void) const
        {
            return this->MakeApiRequest(this->requestBuilder.BuildMinStackLevelRequest()).GetRawData();
        }

        nlohmann::json ApiHelper::GetRegionalPressures(void) const
        {
            return this->MakeApiRequest(this->requestBuilder.BuildRegionalPressureRequest()).GetRawData();
        }

        nlohmann::json ApiHelper::GetUri(std::string uri) const
        {
            if (uri.find(this->GetApiDomain()) == std::string::npos)
            {
                LogCritical("Attempted to get URI on non-ukcp route");
                throw ApiException("Attempted to get URI on non-ukcp route");
            }

            return this->MakeApiRequest(this->requestBuilder.BuildGetUriRequest(uri)).GetRawData();
        }

        /*
            Delete the given user hold profile
        */
        void ApiHelper::DeleteUserHoldProfile(unsigned int profileId) const
        {
            this->MakeApiRequest(this->requestBuilder.BuildDeleteUserHoldProfileRequest(profileId));
        }

        /*
            Create a user hold profile
        */
        unsigned int ApiHelper::CreateUserHoldProfile(std::string name, std::set<unsigned int> holds) const
        {
            nlohmann::json response = this->MakeApiRequest(
                this->requestBuilder.BuildCreateUserHoldProfileRequest(name, holds)
            ).GetRawData();

            if (!response.count("id") || !response.at("id").is_number_integer()) {
                throw ApiException("Invalid API response when creating a hold profile");
            }

            return response.at("id");
        }

        /*
            Update a user hold profile
        */
        void ApiHelper::UpdateUserHoldProfile(unsigned int id, std::string name, std::set<unsigned int> holds) const
        {
            this->MakeApiRequest(this->requestBuilder.BuildUpdateUserHoldProfileRequest(id, name, holds));
        }

        /*
            Runs an update check.
        */
        int ApiHelper::UpdateCheck(std::string version) const
        {
            nlohmann::json apiResponseJson = this->MakeApiRequest(
                this->requestBuilder.BuildVersionCheckRequest(version)
            ).GetRawData();

            if (apiResponseJson["version_disabled"]) {
                return this->UPDATE_VERSION_DISABLED;
            }

            if (apiResponseJson["update_available"]) {
                return this->UPDATE_VERSION_NEEDS_UPDATE;
            }

            return this->UPDATE_UP_TO_DATE;
        }

        /*
            Set api key on the request builder
        */
        void ApiHelper::SetApiKey(std::string key)
        {
            this->requestBuilder.SetApiKey(key);
        }

        /*
            Set api domain on the request builder
        */
        void ApiHelper::SetApiDomain(std::string domain)
        {
            this->requestBuilder.SetApiDomain(domain);
        }
    }  // namespace Api
}  // namespace UKControllerPlugin
