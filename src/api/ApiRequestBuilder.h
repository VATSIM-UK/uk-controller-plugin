#pragma once
#include "curl/CurlRequest.h"
#include "srd/SrdSearchParameters.h"

namespace UKControllerPlugin {
    namespace Api {

        /*
            Responsible for converting a set of input data into a cURL request
            that can sent to the API.
        */
        class ApiRequestBuilder
        {
            public:
                ApiRequestBuilder(std::string apiDomain, std::string apiKey);
                UKControllerPlugin::Curl::CurlRequest BuildAuthCheckRequest(void) const;
                UKControllerPlugin::Curl::CurlRequest BuildDependencyListRequest(void) const;
                UKControllerPlugin::Curl::CurlRequest BuildGetUriRequest(
                    std::string uri
                ) const;
                UKControllerPlugin::Curl::CurlRequest BuildSquawkAssignmentCheckRequest(std::string callsign) const;
                UKControllerPlugin::Curl::CurlRequest BuildSquawkAssignmentDeletionRequest(std::string callsign) const;
                UKControllerPlugin::Curl::CurlRequest BuildLocalSquawkAssignmentRequest(
                    std::string callsign,
                    std::string unit,
                    std::string flightRules
                ) const;
                UKControllerPlugin::Curl::CurlRequest BuildGeneralSquawkAssignmentRequest(
                    std::string callsign,
                    std::string origin,
                    std::string destination
                ) const;
                UKControllerPlugin::Curl::CurlRequest BuildHoldDependencyRequest(void) const;
                UKControllerPlugin::Curl::CurlRequest BuildAllAssignedHoldsRequest(void) const;
                UKControllerPlugin::Curl::CurlRequest BuildSetAssignedHoldRequest(
                    std::string callsign,
                    std::string navaid
                ) const;
                UKControllerPlugin::Curl::CurlRequest BuildDeleteAssignedHoldRequest(std::string callsign) const;
                UKControllerPlugin::Curl::CurlRequest BuildRemoteFileRequest(std::string uri) const;
                UKControllerPlugin::Curl::CurlRequest BuildVersionCheckRequest(std::string versionString) const;
                Curl::CurlRequest BuildMinStackLevelRequest(void) const;
                UKControllerPlugin::Curl::CurlRequest BuildRegionalPressureRequest(void) const;
                UKControllerPlugin::Curl::CurlRequest BuildSrdQueryRequest(
                    UKControllerPlugin::Srd::SrdSearchParameters parameters
                ) const;
                UKControllerPlugin::Curl::CurlRequest BuildGetStandAssignmentsRequest(void) const;
                UKControllerPlugin::Curl::CurlRequest BuildAssignStandToAircraftRequest(
                    std::string callsign,
                    int standId
                ) const;
                UKControllerPlugin::Curl::CurlRequest BuildDeleteStandAssignmentForAircraftRequest(
                    std::string callsign
                ) const;
                UKControllerPlugin::Curl::CurlRequest BuildEnrouteReleaseRequest(
                    std::string aircraftCallsign,
                    std::string sendingController,
                    std::string targetController,
                    int releaseType
                ) const;

                UKControllerPlugin::Curl::CurlRequest BuildEnrouteReleaseRequestWithReleasePoint(
                    std::string aircraftCallsign,
                    std::string sendingController,
                    std::string targetController,
                    int releaseType,
                    std::string releasePoint
                ) const;
                Curl::CurlRequest BuildGetAllNotificationsRequest() const;
                Curl::CurlRequest BuildGetUnreadNotificationsRequest() const;
                Curl::CurlRequest BuildReadNotificationRequest(int id) const;
                Curl::CurlRequest BuildLatestGithubVersionRequest() const;
                Curl::CurlRequest BuildPluginEventSyncRequest() const;
                Curl::CurlRequest BuildGetLatestPluginEventsRequest(int lastEventId) const;
                Curl::CurlRequest
                BuildAcknowledgeDepartureReleaseRequest(int releaseId, int controllerPositionId) const;
                Curl::CurlRequest BuildRejectDepartureReleaseRequest(int releaseId, int controllerPositionId) const;
                Curl::CurlRequest BuildApproveDepartureReleaseRequest(
                    int releaseId,
                    int controllerPositionId,
                    std::chrono::system_clock::time_point releasedAt,
                    int expiresInSeconds
                ) const;
                Curl::CurlRequest BuildDepartureReleaseRequest(
                    std::string callsign,
                    int requestingControllerId,
                    int targetControllerId,
                    int expiresInSeconds
                ) const;
                Curl::CurlRequest BuildCancelReleaseRequest(int releaseId) const;
                std::string GetApiDomain(void) const;
                std::string GetApiKey(void) const;
                void SetApiDomain(std::string domain);
                void SetApiKey(std::string key);

                // The type string to send in the payload if we want a general squawk
                const std::string generalSquawkAssignmentType = "general";

                // The type string to send in the payload if we want a local squawk
                const std::string localSquawkAssignmentType = "local";

            private:

                UKControllerPlugin::Curl::CurlRequest AddCommonHeaders(
                    UKControllerPlugin::Curl::CurlRequest request
                ) const;

                // The base URL of the API
                std::string apiDomain;

                // Our API key
                std::string apiKey;
        };
    }  // namespace Api
}  // namespace UKControllerPlugin
