#include "pch/stdafx.h"
#include "api/ApiRequestBuilder.h"

using UKControllerPlugin::Curl::CurlRequest;
using UKControllerPlugin::Srd::SrdSearchParameters;

namespace UKControllerPlugin {
    namespace Api {

        ApiRequestBuilder::ApiRequestBuilder(std::string apiDomain, std::string apiKey)
            : apiDomain(apiDomain), apiKey(apiKey)
        {

        }

        /*
            Adds common headers such as the auth headers.
        */
        CurlRequest ApiRequestBuilder::AddCommonHeaders(CurlRequest request) const
        {
            request.AddHeader("Authorization", "Bearer " + this->apiKey);
            request.AddHeader("Accept", "application/json");
            request.AddHeader("Content-Type", "application/json");
            return request;
        }

        /*
            Builds a request to hit the root of the API just to check if we can authenticate and reach the API.
        */
        CurlRequest ApiRequestBuilder::BuildAuthCheckRequest(void) const
        {
            return this->AddCommonHeaders(
                CurlRequest(apiDomain + "/authorise", CurlRequest::METHOD_GET)
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
            Method for querying any API URI
        */
        CurlRequest ApiRequestBuilder::BuildGetUriRequest(std::string uri) const
        {
            return this->AddCommonHeaders(
                CurlRequest(uri, CurlRequest::METHOD_GET)
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
            Build a request to authorise a websocket connection to a channel.
        */
        CurlRequest ApiRequestBuilder::BuildWebsocketChannelAuthRequest(
            std::string socketId,
            std::string channel
        ) const {
            CurlRequest request(apiDomain + "/broadcasting/auth", CurlRequest::METHOD_POST);

            nlohmann::json body;
            body["socket_id"] = socketId;
            body["channel_name"] = channel;

            request.SetBody(body.dump());

            return this->AddCommonHeaders(request);
        }

        /*
            Builds a request for getting minimum stack levels.
        */
        CurlRequest ApiRequestBuilder::BuildMinStackLevelRequest(void) const
        {
            return this->AddCommonHeaders(CurlRequest(apiDomain + "/msl", CurlRequest::METHOD_GET));
        }

        /*
            Builds a request for all the regional pressures
        */
        CurlRequest ApiRequestBuilder::BuildRegionalPressureRequest(void) const
        {
            return this->AddCommonHeaders(
                CurlRequest(apiDomain + "/regional-pressure", CurlRequest::METHOD_GET)
            );
        }

        CurlRequest ApiRequestBuilder::BuildSrdQueryRequest(SrdSearchParameters parameters) const
        {
            std::string uri = apiDomain + "/srd/route/search?";
            uri += "origin=" + parameters.origin;
            uri += "&destination=" + parameters.destination;

            if (parameters.requestedLevel != NULL) {
                uri += "&requestedLevel=" + std::to_string(parameters.requestedLevel);
            }

            return this->AddCommonHeaders(CurlRequest(uri, CurlRequest::METHOD_GET));
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
            Builds a request to download the hold data dependency
        */
        CurlRequest ApiRequestBuilder::BuildHoldDependencyRequest(void) const
        {
            return this->AddCommonHeaders(CurlRequest(apiDomain + "/hold", CurlRequest::METHOD_GET));
        }

        /*
            Builds a request to get all the currently assigned holds
        */
        CurlRequest ApiRequestBuilder::BuildAllAssignedHoldsRequest(void) const
        {
            return this->AddCommonHeaders(CurlRequest(apiDomain + "/hold/assigned", CurlRequest::METHOD_GET));
        }

        /*
            Build request to assign an aircraft to a hold
        */
        CurlRequest ApiRequestBuilder::BuildSetAssignedHoldRequest(std::string callsign, std::string navaid) const
        {
            CurlRequest request(this->apiDomain + "/hold/assigned", CurlRequest::METHOD_PUT);
            nlohmann::json data{
                {"callsign", callsign},
                {"navaid", navaid}
            };
            request.SetBody(data.dump());

            return this->AddCommonHeaders(request);
        }

        /*
            Build request to unassign an aircraft from all holds
        */
        CurlRequest ApiRequestBuilder::BuildDeleteAssignedHoldRequest(std::string callsign) const
        {
            return this->AddCommonHeaders(
                CurlRequest(apiDomain + "/hold/assigned/" + callsign, CurlRequest::METHOD_DELETE)
            );
        }

        CurlRequest ApiRequestBuilder::BuildEnrouteReleaseRequestWithReleasePoint(
            std::string aircraftCallsign,
            std::string sendingController,
            std::string targetController,
            int releaseType,
            std::string releasePoint
        ) const {
            CurlRequest request(this->apiDomain + "/release/enroute", CurlRequest::METHOD_POST);
            nlohmann::json data{
                {"callsign", aircraftCallsign},
                {"type", releaseType},
                {"initiating_controller", sendingController},
                {"target_controller", targetController},
                {"release_point", releasePoint},
            };
            request.SetBody(data.dump());

            return this->AddCommonHeaders(request);
        }

        CurlRequest ApiRequestBuilder::BuildEnrouteReleaseRequest(
            std::string aircraftCallsign,
            std::string sendingController,
            std::string targetController,
            int releaseType
        ) const {
            CurlRequest request(this->apiDomain + "/release/enroute", CurlRequest::METHOD_POST);
            nlohmann::json data{
                {"callsign", aircraftCallsign},
                {"type", releaseType},
                {"initiating_controller", sendingController},
                {"target_controller", targetController},
            };
            request.SetBody(data.dump());

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

        /*
            Set the API key
        */
        void ApiRequestBuilder::SetApiDomain(std::string domain)
        {
            this->apiDomain = domain;
        }

        /*
            Set the API domain
        */
        void ApiRequestBuilder::SetApiKey(std::string key)
        {
            this->apiKey = key;
        }
    }  // namespace Api
}  // namespace UKControllerPlugin
