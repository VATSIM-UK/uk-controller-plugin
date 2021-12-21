#pragma once
#include "api/ApiInterface.h"
#include "api/ApiRequestBuilder.h"
#include "api/ApiResponse.h"

namespace UKControllerPlugin::Curl {
    class CurlInterface;
    class CurlRequest;
} // namespace UKControllerPlugin::Curl

namespace UKControllerPlugin::Api {

    /*
        A class for making requests to the UKCP API.
    */
    class ApiHelper : public UKControllerPlugin::Api::ApiInterface
    {
        public:
        ApiHelper(UKControllerPlugin::Curl::CurlInterface& curlApi, ApiRequestBuilder requestBuilder);

        [[nodiscard]] auto
        CreateGeneralSquawkAssignment(std::string callsign, std::string origin, std::string destination) const
            -> UKControllerPlugin::Squawk::ApiSquawkAllocation override;
        [[nodiscard]] auto
        CreateLocalSquawkAssignment(std::string callsign, std::string unit, std::string flightRules) const
            -> UKControllerPlugin::Squawk::ApiSquawkAllocation override;
        [[nodiscard]] auto CheckApiAuthorisation() const -> bool override;
        void DeleteSquawkAssignment(std::string callsign) const override;
        [[nodiscard]] auto FetchRemoteFile(std::string uri) const -> std::string override;
        [[nodiscard]] auto GetAssignedSquawk(std::string callsign) const
            -> UKControllerPlugin::Squawk::ApiSquawkAllocation override;
        [[nodiscard]] auto GetApiDomain() const -> std::string override;
        [[nodiscard]] auto GetApiKey() const -> std::string override;
        [[nodiscard]] auto GetDependencyList() const -> nlohmann::json override;
        [[nodiscard]] auto GetHoldDependency() const -> nlohmann::json override;
        [[nodiscard]] auto GetAssignedHolds() const -> nlohmann::json override;
        ;
        void AssignAircraftToHold(std::string callsign, std::string navaid) const override;
        void UnassignAircraftHold(std::string callsign) const override;
        [[nodiscard]] auto GetMinStackLevels() const -> nlohmann::json override;
        [[nodiscard]] auto GetRegionalPressures() const -> nlohmann::json override;
        [[nodiscard]] auto GetUri(std::string uri) const -> nlohmann::json override;
        [[nodiscard]] auto SearchSrd(Srd::SrdSearchParameters params) const -> nlohmann::json override;
        [[nodiscard]] auto GetAssignedStands() const -> nlohmann::json override;
        void AssignStandToAircraft(std::string callsign, int standId) const override;
        void DeleteStandAssignmentForAircraft(std::string callsign) const override;
        void SendEnrouteRelease(
            std::string aircraftCallsign,
            std::string sendingController,
            std::string targetController,
            int releaseType) const override;
        void SendEnrouteReleaseWithReleasePoint(
            std::string aircraftCallsign,
            std::string sendingController,
            std::string targetController,
            int releaseType,
            std::string releasePoint) const override;
        [[nodiscard]] auto GetUpdateDetails() const -> nlohmann::json override;
        [[nodiscard]] auto GetAllNotifications() const -> nlohmann::json override;
        [[nodiscard]] auto GetUnreadNotifications() const -> nlohmann::json override;
        [[nodiscard]] auto SyncPluginEvents() const -> nlohmann::json override;
        [[nodiscard]] auto GetLatestPluginEvents(int lastEventId) const -> nlohmann::json override;
        void AcknowledgeDepartureReleaseRequest(int releaseId, int controllerPositionId) const override;
        void RejectDepartureReleaseRequest(int releaseId, int controllerPositionId) const override;
        void ApproveDepartureReleaseRequest(
            int releaseId,
            int controllerPositionId,
            std::chrono::system_clock::time_point releasedAt,
            int expiresInSeconds) const override;
        [[nodiscard]] auto RequestDepartureRelease(
            std::string callsign, int requestingControllerId, int targetControllerId, int expiresInSeconds) const
            -> nlohmann::json override;
        void CancelDepartureReleaseRequest(int releaseId) const override;
        void ReadNotification(int id) const override;
        void SetApiKey(std::string key) override;
        void SetApiDomain(std::string domain) override;
        [[nodiscard]] auto CreatePrenoteMessage(
            const std::string& callsign,
            const std::string& departureAirfield,
            const std::string& departureSid,
            const std::string& destinationAirfield,
            int requestingController,
            int targetController,
            int requestExpiry) const -> nlohmann::json override;
        void AcknowledgePrenoteMessage(int messageId, int controllerId) const override;
        void DeletePrenoteMessage(int messageId) const override;
        [[nodiscard]] auto CreateMissedApproach(const std::string& callsign) const -> nlohmann::json override;
        void AcknowledgeMissedApproach(int id, const std::string& remarks) const override;

        // The HTTP status codes that may be returned by the API
        static const uint64_t STATUS_OK = 200L;
        static const uint64_t STATUS_CREATED = 201L;
        static const uint64_t STATUS_NO_CONTENT = 204L;
        static const uint64_t STATUS_BAD_REQUEST = 400L;
        static const uint64_t STATUS_UNAUTHORISED = 401L;
        static const uint64_t STATUS_FORBIDDEN = 403L;
        static const uint64_t STATUS_NOT_FOUND = 404L;
        static const uint64_t STATUS_SERVER_ERROR = 500L;
        static const uint64_t STATUS_SERVICE_UNAVAILBLE = 503L;

        private:
        [[nodiscard]] auto MakeApiRequest(const UKControllerPlugin::Curl::CurlRequest& request) const -> ApiResponse;
        [[nodiscard]] static auto ProcessSquawkResponse(const ApiResponse& response, const std::string& callsign)
            -> UKControllerPlugin::Squawk::ApiSquawkAllocation;

        // The API request builder, that builds our CurlRequests
        UKControllerPlugin::Api::ApiRequestBuilder requestBuilder;

        // An interface to the Curl library.
        UKControllerPlugin::Curl::CurlInterface& curlApi;
    };
} // namespace UKControllerPlugin::Api
