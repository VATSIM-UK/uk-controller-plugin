#include <utility>

#include "api/ApiRequestBuilder.h"

using UKControllerPlugin::Curl::CurlRequest;
using UKControllerPlugin::Srd::SrdSearchParameters;

namespace UKControllerPlugin::Api {
    ApiRequestBuilder::ApiRequestBuilder(std::string apiDomain, std::string apiKey)
        : apiDomain(std::move(apiDomain)), apiKey(std::move(apiKey))
    {
    }

    /*
        Adds common headers such as the auth headers.
    */
    auto ApiRequestBuilder::AddCommonHeaders(CurlRequest request) const -> CurlRequest
    {
        request.AddHeader("Authorization", "Bearer " + this->apiKey);
        request.AddHeader("Accept", "application/json");
        request.AddHeader("Content-Type", "application/json");
        return request;
    }

    /*
        Builds a request to hit the root of the API just to check if we can authenticate and reach the API.
    */
    auto ApiRequestBuilder::BuildAuthCheckRequest() const -> CurlRequest
    {
        return this->AddCommonHeaders(CurlRequest(apiDomain + "/authorise", CurlRequest::METHOD_GET));
    }

    /*
        Builds a request to get the dependency manifest file
    */
    auto ApiRequestBuilder::BuildDependencyListRequest() const -> CurlRequest
    {
        return this->AddCommonHeaders(CurlRequest(apiDomain + "/dependency", CurlRequest::METHOD_GET));
    }

    /*
        Method for querying any API URI. On this method only we disable the
        CURLOPT_TIMEOUT as this method is used to download dependencies and may
        be quite big.
    */
    auto ApiRequestBuilder::BuildGetUriRequest(std::string uri) const -> CurlRequest
    {
        CurlRequest request(std::move(uri), CurlRequest::METHOD_GET);
        request.SetMaxRequestTime(0L);
        return this->AddCommonHeaders(request);
    }

    /*
        Builds a request to return a static file stored on the API. This request
        does not need authentication headers.
    */
    auto ApiRequestBuilder::BuildRemoteFileRequest(std::string uri) -> CurlRequest
    {
        return {std::move(uri), CurlRequest::METHOD_GET};
    }

    /*
        Builds a request to check the version of the plugin
    */
    auto ApiRequestBuilder::BuildVersionCheckRequest(const std::string& versionString) const -> CurlRequest
    {
        return this->AddCommonHeaders(
            CurlRequest(apiDomain + "/version/" + versionString + "/status", CurlRequest::METHOD_GET));
    }

    /*
        Builds a request for getting minimum stack levels.
    */
    auto ApiRequestBuilder::BuildMinStackLevelRequest() const -> CurlRequest
    {
        return this->AddCommonHeaders(CurlRequest(apiDomain + "/msl", CurlRequest::METHOD_GET));
    }

    /*
        Builds a request for all the regional pressures
    */
    auto ApiRequestBuilder::BuildRegionalPressureRequest() const -> CurlRequest
    {
        return this->AddCommonHeaders(CurlRequest(apiDomain + "/regional-pressure", CurlRequest::METHOD_GET));
    }

    /*
        Builds a request for querying the SRD
    */
    auto ApiRequestBuilder::BuildSrdQueryRequest(const SrdSearchParameters& parameters) const -> CurlRequest
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
    auto ApiRequestBuilder::BuildGetStandAssignmentsRequest() const -> CurlRequest
    {
        return this->AddCommonHeaders(CurlRequest(apiDomain + "/stand/assignment", CurlRequest::METHOD_GET));
    }

    /*
        Builds a request for assigning a stand to an aircraft
    */
    auto ApiRequestBuilder::BuildAssignStandToAircraftRequest(const std::string& callsign, int standId) const
        -> CurlRequest
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
    auto ApiRequestBuilder::BuildDeleteStandAssignmentForAircraftRequest(const std::string& callsign) const
        -> CurlRequest
    {
        return this->AddCommonHeaders(
            CurlRequest(apiDomain + "/stand/assignment/" + callsign, CurlRequest::METHOD_DELETE));
    }

    /*
        Builds a request to check whether or not the given aircraft has been assigned a squawk.
    */
    auto ApiRequestBuilder::BuildSquawkAssignmentCheckRequest(const std::string& callsign) const -> CurlRequest
    {
        return this->AddCommonHeaders(
            CurlRequest(apiDomain + "/squawk-assignment/" + callsign, CurlRequest::METHOD_GET));
    }

    /*
        Builds a request to delete (deallocate) an assigned squawk
    */
    auto ApiRequestBuilder::BuildSquawkAssignmentDeletionRequest(const std::string& callsign) const -> CurlRequest
    {
        return this->AddCommonHeaders(
            CurlRequest(apiDomain + "/squawk-assignment/" + callsign, CurlRequest::METHOD_DELETE));
    }

    /*
        Builds a request to request a squawk that is local to a particular unit.
    */
    auto ApiRequestBuilder::BuildLocalSquawkAssignmentRequest(
        const std::string& callsign, const std::string& unit, const std::string& flightRules) const -> CurlRequest
    {
        CurlRequest request(apiDomain + "/squawk-assignment/" + callsign, CurlRequest::METHOD_PUT);

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
    auto ApiRequestBuilder::BuildGeneralSquawkAssignmentRequest(
        const std::string& callsign, const std::string& origin, const std::string& destination) const -> CurlRequest
    {
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
    auto ApiRequestBuilder::BuildHoldDependencyRequest() const -> CurlRequest
    {
        return this->AddCommonHeaders(CurlRequest(apiDomain + "/hold", CurlRequest::METHOD_GET));
    }

    /*
        Builds a request to get all the currently assigned holds
    */
    auto ApiRequestBuilder::BuildAllAssignedHoldsRequest() const -> CurlRequest
    {
        return this->AddCommonHeaders(CurlRequest(apiDomain + "/hold/assigned", CurlRequest::METHOD_GET));
    }

    /*
        Build request to assign an aircraft to a hold
    */
    auto ApiRequestBuilder::BuildSetAssignedHoldRequest(std::string callsign, std::string navaid) const -> CurlRequest
    {
        CurlRequest request(this->apiDomain + "/hold/assigned", CurlRequest::METHOD_PUT);
        nlohmann::json data{{"callsign", callsign}, {"navaid", navaid}};
        request.SetBody(data.dump());

        return this->AddCommonHeaders(request);
    }

    /*
        Build request to unassign an aircraft from all holds
    */
    auto ApiRequestBuilder::BuildDeleteAssignedHoldRequest(const std::string& callsign) const -> CurlRequest
    {
        return this->AddCommonHeaders(
            CurlRequest(apiDomain + "/hold/assigned/" + callsign, CurlRequest::METHOD_DELETE));
    }

    auto ApiRequestBuilder::BuildEnrouteReleaseRequestWithReleasePoint(
        std::string aircraftCallsign,
        std::string sendingController,
        std::string targetController,
        int releaseType,
        std::string releasePoint) const -> CurlRequest
    {
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

    auto ApiRequestBuilder::BuildGetAllNotificationsRequest() const -> CurlRequest
    {
        return this->AddCommonHeaders(CurlRequest(this->apiDomain + "/notifications", CurlRequest::METHOD_GET));
    }

    auto ApiRequestBuilder::BuildGetUnreadNotificationsRequest() const -> CurlRequest
    {
        return this->AddCommonHeaders(CurlRequest(this->apiDomain + "/notifications/unread", CurlRequest::METHOD_GET));
    }

    auto ApiRequestBuilder::BuildReadNotificationRequest(int id) const -> CurlRequest
    {
        return this->AddCommonHeaders(
            CurlRequest(this->apiDomain + "/notifications/read/" + std::to_string(id), CurlRequest::METHOD_PUT));
    }

    auto ApiRequestBuilder::BuildLatestGithubVersionRequest() const -> CurlRequest
    {
        return this->AddCommonHeaders(CurlRequest(this->apiDomain + "/version/latest", CurlRequest::METHOD_GET));
    }

    auto ApiRequestBuilder::BuildPluginEventSyncRequest() const -> CurlRequest
    {
        return this->AddCommonHeaders(CurlRequest(this->apiDomain + "/plugin-events/sync", CurlRequest::METHOD_GET));
    }

    auto ApiRequestBuilder::BuildGetLatestPluginEventsRequest(int lastEventId) const -> CurlRequest
    {
        return this->AddCommonHeaders(CurlRequest(
            this->apiDomain + "/plugin-events/recent?previous=" + std::to_string(lastEventId),
            CurlRequest::METHOD_GET));
    }

    auto ApiRequestBuilder::BuildAcknowledgeDepartureReleaseRequest(int releaseId, int controllerPositionId) const
        -> CurlRequest
    {
        nlohmann::json body;
        body["controller_position_id"] = controllerPositionId;

        CurlRequest request(
            this->apiDomain + "/departure/release/request/" + std::to_string(releaseId) + "/acknowledge",
            CurlRequest::METHOD_PATCH);
        request.SetBody(body.dump());
        return this->AddCommonHeaders(request);
    }

    auto ApiRequestBuilder::BuildRejectDepartureReleaseRequest(int releaseId, int controllerPositionId) const
        -> CurlRequest
    {
        nlohmann::json body;
        body["controller_position_id"] = controllerPositionId;

        CurlRequest request(
            this->apiDomain + "/departure/release/request/" + std::to_string(releaseId) + "/reject",
            CurlRequest::METHOD_PATCH);
        request.SetBody(body.dump());
        return this->AddCommonHeaders(request);
    }

    /*
     * Build an approve departure release request, pass -1 seconds for never expires.
     */
    auto ApiRequestBuilder::BuildApproveDepartureReleaseRequest(
        int releaseId,
        int controllerPositionId,
        std::chrono::system_clock::time_point releasedAt,
        int expiresInSeconds) const -> CurlRequest
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
            CurlRequest::METHOD_PATCH);
        request.SetBody(body.dump());
        return this->AddCommonHeaders(request);
    }

    auto ApiRequestBuilder::BuildDepartureReleaseRequest(
        const std::string& callsign, int requestingControllerId, int targetController, int expiresInSeconds) const
        -> CurlRequest
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

    auto ApiRequestBuilder::BuildCancelReleaseRequest(int releaseId) const -> CurlRequest
    {
        return this->AddCommonHeaders(CurlRequest(
            this->apiDomain + "/departure/release/request/" + std::to_string(releaseId), CurlRequest::METHOD_DELETE));
    }

    auto ApiRequestBuilder::BuildEnrouteReleaseRequest(
        std::string aircraftCallsign,
        std::string sendingController,
        std::string targetController,
        int releaseType) const -> CurlRequest
    {
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
    auto ApiRequestBuilder::GetApiDomain() const -> std::string
    {
        return this->apiDomain;
    }

    /*
        Returns the API key that is used to authorize requests
    */
    auto ApiRequestBuilder::GetApiKey() const -> std::string
    {
        return this->apiKey;
    }

    /*
        Set the API key
    */
    void ApiRequestBuilder::SetApiDomain(std::string domain)
    {
        this->apiDomain = std::move(domain);
    }

    /*
        Set the API domain
    */
    void ApiRequestBuilder::SetApiKey(std::string key)
    {
        this->apiKey = std::move(key);
    }

    auto ApiRequestBuilder::BuildCreatePrenoteMessageRequest(
        const std::string& callsign,
        const std::string& departureAirfield,
        const std::string& departureSid,
        const std::string& destinationAirfield,
        int requestingController,
        int targetController,
        int requestExpiry) const -> UKControllerPlugin::Curl::CurlRequest
    {
        CurlRequest request(this->apiDomain + "/prenotes/messages", CurlRequest::METHOD_POST);
        nlohmann::json data{
            {"callsign", callsign},
            {"departure_airfield", departureAirfield},
            {"departure_sid",
             departureSid.empty() ? nlohmann::json(nlohmann::json::value_t::null) : nlohmann::json(departureSid)},
            {"destination_airfield",
             destinationAirfield.empty() ? nlohmann::json(nlohmann::json::value_t::null)
                                         : nlohmann::json(destinationAirfield)},
            {"requesting_controller_id", requestingController},
            {"target_controller_id", targetController},
            {"expires_in_seconds", requestExpiry},
        };
        request.SetBody(data.dump());

        return this->AddCommonHeaders(request);
    }

    auto ApiRequestBuilder::BuildAcknowledgePrenoteMessageRequest(int messageId, int controllerId) const
        -> UKControllerPlugin::Curl::CurlRequest
    {
        CurlRequest request(
            this->apiDomain + "/prenotes/messages/" + std::to_string(messageId), CurlRequest::METHOD_PATCH);
        nlohmann::json data{
            {"controller_position_id", controllerId},
        };
        request.SetBody(data.dump());

        return this->AddCommonHeaders(request);
    }
    auto ApiRequestBuilder::BuildDeletePrenoteMessageRequest(int messageId) const
        -> UKControllerPlugin::Curl::CurlRequest
    {
        return this->AddCommonHeaders(
            {this->apiDomain + "/prenotes/messages/" + std::to_string(messageId), CurlRequest::METHOD_DELETE});
    }
} // namespace UKControllerPlugin::Api
