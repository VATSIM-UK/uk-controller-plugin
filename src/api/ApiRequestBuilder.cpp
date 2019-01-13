#include "pch/stdafx.h"
#include "api/ApiRequestBuilder.h"

using UKControllerPlugin::Curl::CurlRequest;

namespace UKControllerPlugin {
    namespace Api {

        ApiRequestBuilder::ApiRequestBuilder(std::string apiDomain, std::string apiKey)
            : apiDomain(apiDomain), apiKey(apiKey)
        {

        }

        /*
            Adds common headers such as the auth headers.
        */
        CurlRequest ApiRequestBuilder::AddCommonHeaders(UKControllerPlugin::Curl::CurlRequest request) const
        {
            request.AddHeader("Authorization", "Bearer " + this->apiKey);
            request.AddHeader("Accept", "application/json");
            return request;
        }

        /*
            Builds a request to hit the root of the API just to check if we can authenticate and reach the API.
        */
        CurlRequest ApiRequestBuilder::BuildAuthCheckRequest(void) const
        {
            return this->AddCommonHeaders(
                CurlRequest(apiDomain + "/", CurlRequest::METHOD_GET)
            );
        }

        /*
            Builds a request to get the dependency manifest file
        */
        CurlRequest ApiRequestBuilder::BuildDependencyListRequest(void) const
        {
            return this->AddCommonHeaders(
                CurlRequest(apiDomain + "/dependency", CurlRequest::METHOD_GET)
            );
        }

        /*
            Builds a request to return a static file stored on the API. This request
            does not need authentication headers.
        */
        CurlRequest ApiRequestBuilder::BuildRemoteFileRequest(std::string uri) const
        {
            return CurlRequest(uri, CurlRequest::METHOD_GET);
        }

        /*
            Builds a request to check the version of the plugin
        */
        CurlRequest ApiRequestBuilder::BuildVersionCheckRequest(std::string versionString) const
        {
            return this->AddCommonHeaders(
                CurlRequest(apiDomain + "/version/" + versionString + "/status", CurlRequest::METHOD_GET)
            );
        }

        /*
            Builds a request to check whether or not the given aircraft has been assigned a squawk.
        */
        CurlRequest ApiRequestBuilder::BuildSquawkAssignmentCheckRequest(std::string callsign) const
        {
            return this->AddCommonHeaders(
                CurlRequest(apiDomain + "/squawk-assignment/" + callsign, CurlRequest::METHOD_GET)
            );
        }

        /*
            Builds a request to delete (deallocate) an assigned squawk
        */
        CurlRequest ApiRequestBuilder::BuildSquawkAssignmentDeletionRequest(std::string callsign) const
        {
            return this->AddCommonHeaders(
                CurlRequest(apiDomain + "/squawk-assignment/" + callsign, CurlRequest::METHOD_DELETE)
            );
        }

        /*
            Builds a request to request a squawk that is local to a particular unit.
        */
        CurlRequest ApiRequestBuilder::BuildLocalSquawkAssignmentRequest(
            std::string callsign,
            std::string unit,
            std::string flightRules
            ) const {
            CurlRequest request(apiDomain + "/squawk-assignment/" + callsign , CurlRequest::METHOD_PUT);

            nlohmann::json body;
            body["type"] = this->localSquawkAssignmentType;
            body["unit"] = unit;
            body["rules"] = flightRules;

            request.SetBody(body.dump());

            return this->AddCommonHeaders(request);
        }

        /*
            Builds a request to request a general use squawk.
        */
        CurlRequest ApiRequestBuilder::BuildGeneralSquawkAssignmentRequest(
            std::string callsign,
            std::string origin,
            std::string destination
        ) const {
            CurlRequest request(apiDomain + "/squawk-assignment/" + callsign, CurlRequest::METHOD_PUT);

            nlohmann::json body;
            body["type"] = this->generalSquawkAssignmentType;
            body["origin"] = origin;
            body["destination"] = destination;

            request.SetBody(body.dump());

            return this->AddCommonHeaders(request);
        }

        /*
            Returns the API Domain that the builder is using
        */
        std::string ApiRequestBuilder::GetApiDomain(void) const
        {
            return this->apiDomain;
        }

        /*
            Returns the API key that is used to authorize requests
        */
        std::string ApiRequestBuilder::GetApiKey(void) const
        {
            return this->apiKey;
        }
    }  // namespace Api
}  // namespace UKControllerPlugin
