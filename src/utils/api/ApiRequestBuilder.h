#pragma once
#include "curl/CurlRequest.h"
#include "srd/SrdSearchParameters.h"

namespace UKControllerPlugin::Api {

    /*
        Responsible for converting a set of input data into a cURL request
        that can sent to the API.
    */
    class ApiRequestBuilder
    {
        public:
        ApiRequestBuilder(std::string apiDomain, std::string apiKey);
        [[nodiscard]] auto BuildAuthCheckRequest() const -> UKControllerPlugin::Curl::CurlRequest;
        [[nodiscard]] auto BuildDependencyListRequest() const -> UKControllerPlugin::Curl::CurlRequest;
        [[nodiscard]] auto BuildGetUriRequest(std::string uri) const -> UKControllerPlugin::Curl::CurlRequest;
        [[nodiscard]] auto BuildSquawkAssignmentCheckRequest(const std::string& callsign) const
            -> UKControllerPlugin::Curl::CurlRequest;
        [[nodiscard]] auto BuildSquawkAssignmentDeletionRequest(const std::string& callsign) const
            -> UKControllerPlugin::Curl::CurlRequest;
        [[nodiscard]] auto BuildLocalSquawkAssignmentRequest(
            const std::string& callsign, const std::string& unit, const std::string& flightRules) const
            -> UKControllerPlugin::Curl::CurlRequest;
        [[nodiscard]] auto BuildGeneralSquawkAssignmentRequest(
            const std::string& callsign, const std::string& origin, const std::string& destination) const
            -> UKControllerPlugin::Curl::CurlRequest;
        [[nodiscard]] auto BuildHoldDependencyRequest() const -> UKControllerPlugin::Curl::CurlRequest;
        [[nodiscard]] auto BuildAllAssignedHoldsRequest() const -> UKControllerPlugin::Curl::CurlRequest;
        [[nodiscard]] auto BuildSetAssignedHoldRequest(std::string callsign, std::string navaid) const
            -> UKControllerPlugin::Curl::CurlRequest;
        [[nodiscard]] auto BuildDeleteAssignedHoldRequest(const std::string& callsign) const
            -> UKControllerPlugin::Curl::CurlRequest;
        [[nodiscard]] static auto BuildRemoteFileRequest(std::string uri) -> UKControllerPlugin::Curl::CurlRequest;
        [[nodiscard]] auto BuildMinStackLevelRequest() const -> UKControllerPlugin::Curl::CurlRequest;
        [[nodiscard]] auto BuildRegionalPressureRequest() const -> UKControllerPlugin::Curl::CurlRequest;
        [[nodiscard]] auto BuildSrdQueryRequest(const UKControllerPlugin::Srd::SrdSearchParameters& parameters) const
            -> UKControllerPlugin::Curl::CurlRequest;
        [[nodiscard]] auto BuildGetStandAssignmentsRequest() const -> UKControllerPlugin::Curl::CurlRequest;
        [[nodiscard]] auto BuildAssignStandToAircraftRequest(const std::string& callsign, int standId) const
            -> UKControllerPlugin::Curl::CurlRequest;
        [[nodiscard]] auto BuildDeleteStandAssignmentForAircraftRequest(const std::string& callsign) const
            -> UKControllerPlugin::Curl::CurlRequest;
        [[nodiscard]] auto BuildEnrouteReleaseRequest(
            std::string aircraftCallsign,
            std::string sendingController,
            std::string targetController,
            int releaseType) const -> UKControllerPlugin::Curl::CurlRequest;

        [[nodiscard]] auto BuildEnrouteReleaseRequestWithReleasePoint(
            std::string aircraftCallsign,
            std::string sendingController,
            std::string targetController,
            int releaseType,
            std::string releasePoint) const -> UKControllerPlugin::Curl::CurlRequest;
        [[nodiscard]] auto BuildGetAllNotificationsRequest() const -> UKControllerPlugin::Curl::CurlRequest;
        [[nodiscard]] auto BuildGetUnreadNotificationsRequest() const -> UKControllerPlugin::Curl::CurlRequest;
        [[nodiscard]] auto BuildReadNotificationRequest(int id) const -> UKControllerPlugin::Curl::CurlRequest;
        [[nodiscard]] auto BuildLatestGithubVersionRequest(const std::string& releaseChannel) const
            -> UKControllerPlugin::Curl::CurlRequest;
        [[nodiscard]] auto BuildPluginEventSyncRequest() const -> UKControllerPlugin::Curl::CurlRequest;
        [[nodiscard]] auto BuildGetLatestPluginEventsRequest(int lastEventId) const
            -> UKControllerPlugin::Curl::CurlRequest;
        [[nodiscard]] auto BuildAcknowledgeDepartureReleaseRequest(int releaseId, int controllerPositionId) const
            -> UKControllerPlugin::Curl::CurlRequest;
        [[nodiscard]] auto BuildRejectDepartureReleaseRequest(int releaseId, int controllerPositionId) const
            -> UKControllerPlugin::Curl::CurlRequest;
        [[nodiscard]] auto BuildApproveDepartureReleaseRequest(
            int releaseId,
            int controllerPositionId,
            std::chrono::system_clock::time_point releasedAt,
            int expiresInSeconds) const -> UKControllerPlugin::Curl::CurlRequest;
        [[nodiscard]] auto BuildDepartureReleaseRequest(
            const std::string& callsign, int requestingControllerId, int targetControllerId, int expiresInSeconds) const
            -> UKControllerPlugin::Curl::CurlRequest;
        [[nodiscard]] auto BuildCancelReleaseRequest(int releaseId) const -> UKControllerPlugin::Curl::CurlRequest;
        [[nodiscard]] auto BuildCreatePrenoteMessageRequest(
            const std::string& callsign,
            const std::string& departureAirfield,
            const std::string& departureSid,
            const std::string& destinationAirfield,
            int requestingController,
            int targetController,
            int requestExpiry) const -> UKControllerPlugin::Curl::CurlRequest;
        [[nodiscard]] auto BuildAcknowledgePrenoteMessageRequest(int messageId, int controllerId) const
            -> UKControllerPlugin::Curl::CurlRequest;
        [[nodiscard]] auto BuildDeletePrenoteMessageRequest(int messageId) const
            -> UKControllerPlugin::Curl::CurlRequest;
        [[nodiscard]] auto BuildMissedApproachMessage(const std::string& callsign) const
            -> UKControllerPlugin::Curl::CurlRequest;
        [[nodiscard]] auto BuildMissedApproachAcknowledgeMessage(int id, const std::string& remarks) const
            -> UKControllerPlugin::Curl::CurlRequest;

        [[nodiscard]] auto GetApiDomain() const -> std::string;
        [[nodiscard]] auto GetApiKey() const -> std::string;
        void SetApiDomain(std::string domain);
        void SetApiKey(std::string key);

        private:
        [[nodiscard]] auto AddCommonHeaders(UKControllerPlugin::Curl::CurlRequest request) const
            -> UKControllerPlugin::Curl::CurlRequest;

        // The type string to send in the payload if we want a general squawk
        const std::string generalSquawkAssignmentType = "general";

        // The type string to send in the payload if we want a local squawk
        const std::string localSquawkAssignmentType = "local";

        // The base URL of the API
        std::string apiDomain;

        // Our API key
        std::string apiKey;
    };
} // namespace UKControllerPlugin::Api
