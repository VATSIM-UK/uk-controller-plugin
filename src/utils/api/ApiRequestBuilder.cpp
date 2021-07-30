#include "pch/pch.h"
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
            Method for querying any API URI. On this method only we disable the
            CURLOPT_TIMEOUT as this method is used to download dependencies and may
            be quite big.
        */
        CurlRequest ApiRequestBuilder::BuildGetUriRequest(std::string uri) const
        {
            CurlRequest request(uri, CurlRequest::METHOD_GET);
            request.SetMaxRequestTime(0L);
            return this->AddCommonHeaders(request);
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

        /*
            Builds a request for querying the SRD
        */
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
            Builds a request for getting all the stand assignments
        */
        CurlRequest ApiRequestBuilder::BuildGetStandAssignmentsRequest(void) const
        {
            return this->AddCommonHeaders(
                CurlRequest(apiDomain + "/stand/assignment", CurlRequest::METHOD_GET)
            );
        }

        /*
            Builds a request for assigning a stand to an aircraft
        */
        CurlRequest ApiRequestBuilder::BuildAssignStandToAircraftRequest(std::string callsign, int standId) const
        {
            CurlRequest request(apiDomain + "/stand/assignment", CurlRequest::METHOD_PUT);
            nlohmann::json body;
            body["callsign"] = callsign;
            body["stand_id"] = standId;
            request.SetBody(body.dump());

            return this->AddCommonHeaders(request);
        }

        /*
            Builds a request for deleting an aircrafts stand assignment
        */
        CurlRequest ApiRequestBuilder::BuildDeleteStandAssignmentForAircraftRequest(std::string callsign) const
        {
            return this->AddCommonHeaders(
                CurlRequest(apiDomain + "/stand/assignment/" + callsign, CurlRequest::METHOD_DELETE)
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

        CurlRequest ApiRequestBuilder::BuildGetAllNotificationsRequest() const
        {
            return this->AddCommonHeaders(CurlRequest(this->apiDomain + "/notifications", CurlRequest::METHOD_GET));
        }

        CurlRequest ApiRequestBuilder::BuildGetUnreadNotificationsRequest() const
        {
            return this->AddCommonHeaders(
                CurlRequest(this->apiDomain + "/notifications/unread", CurlRequest::METHOD_GET)
            );
        }

        CurlRequest ApiRequestBuilder::BuildReadNotificationRequest(int id) const
        {
            return this->AddCommonHeaders(
                CurlRequest(this->apiDomain + "/notifications/read/" + std::to_string(id), CurlRequest::METHOD_PUT)
            );
        }

        CurlRequest ApiRequestBuilder::BuildLatestGithubVersionRequest() const
        {
            return this->AddCommonHeaders(
                CurlRequest(this->apiDomain + "/version/latest", CurlRequest::METHOD_GET)
            );
        }

        CurlRequest ApiRequestBuilder::BuildPluginEventSyncRequest() const
        {
            return this->AddCommonHeaders(
                CurlRequest(this->apiDomain + "/plugin-events/sync", CurlRequest::METHOD_GET)
            );
        }

        CurlRequest ApiRequestBuilder::BuildGetLatestPluginEventsRequest(int lastEventId) const
        {
            return this->AddCommonHeaders(
                CurlRequest(
                    this->apiDomain + "/plugin-events/recent?previous=" + std::to_string(lastEventId),
                    CurlRequest::METHOD_GET
                )
            );
        }

        CurlRequest ApiRequestBuilder::BuildAcknowledgeDepartureReleaseRequest(
            int releaseId,
            int controllerPositionId
        ) const
        {
            nlohmann::json body;
            body["controller_position_id"] = controllerPositionId;

            CurlRequest request(
                this->apiDomain + "/departure/release/request/" + std::to_string(releaseId) + "/acknowledge",
                CurlRequest::METHOD_PATCH
            );
            request.SetBody(body.dump());
            return this->AddCommonHeaders(request);
        }

        CurlRequest ApiRequestBuilder::BuildRejectDepartureReleaseRequest(
            int releaseId,
            int controllerPositionId
        ) const
        {
            nlohmann::json body;
            body["controller_position_id"] = controllerPositionId;

            CurlRequest request(
                this->apiDomain + "/departure/release/request/" + std::to_string(releaseId) + "/reject",
                CurlRequest::METHOD_PATCH
            );
            request.SetBody(body.dump());
            return this->AddCommonHeaders(request);
        }

        /*
         * Build an approve departure release request, pass -1 seconds for never expires.
         */
        CurlRequest ApiRequestBuilder::BuildApproveDepartureReleaseRequest(
            int releaseId,
            int controllerPositionId,
            std::chrono::system_clock::time_point releasedAt,
            int expiresInSeconds
        ) const
        {
            nlohmann::json body;
            body["controller_position_id"] = controllerPositionId;
            body["released_at"] = date::format("%Y-%m-%d %H:%M:%S", date::floor<std::chrono::seconds>(releasedAt));
            if (expiresInSeconds == -1) {
                body["expires_in_seconds"] = nlohmann::json::value_t::null;
            } else {
                body["expires_in_seconds"] = expiresInSeconds;
            }

            CurlRequest request(
                this->apiDomain + "/departure/release/request/" + std::to_string(releaseId) + "/approve",
                CurlRequest::METHOD_PATCH
            );
            request.SetBody(body.dump());
            return this->AddCommonHeaders(request);
        }

        CurlRequest ApiRequestBuilder::BuildDepartureReleaseRequest(
            std::string callsign,
            int requestingControllerId,
            int targetController,
            int expiresInSeconds
        ) const
        {
            CurlRequest request(this->apiDomain + "/departure/release/request", CurlRequest::METHOD_POST);

            nlohmann::json body;
            body["callsign"] = callsign;
            body["requesting_controller_id"] = requestingControllerId;
            body["target_controller_id"] = targetController;
            body["expires_in_seconds"] = expiresInSeconds;
            request.SetBody(body.dump());

            return this->AddCommonHeaders(request);
        }

        CurlRequest ApiRequestBuilder::BuildCancelReleaseRequest(int releaseId) const
        {
            return this->AddCommonHeaders(
                CurlRequest(
                    this->apiDomain + "/departure/release/request/" + std::to_string(releaseId),
                    CurlRequest::METHOD_DELETE
                )
            );
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
