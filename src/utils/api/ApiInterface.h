#pragma once
#include "squawk/ApiSquawkAllocation.h"
#include "srd/SrdSearchParameters.h"

namespace UKControllerPlugin::Api {

    /**
     * An abstract class for the web API.
     */
    class ApiInterface
    {
        public:
        ApiInterface() = default;
        ApiInterface(const ApiInterface&) = default;
        ApiInterface(ApiInterface&&) = default;
        auto operator=(const ApiInterface&) -> ApiInterface& = default;
        auto operator=(ApiInterface&&) -> ApiInterface& = default;
        virtual ~ApiInterface() = default;
        [[nodiscard]] virtual auto
        CreateGeneralSquawkAssignment(std::string callsign, std::string origin, std::string destination) const
            -> UKControllerPlugin::Squawk::ApiSquawkAllocation = 0;
        [[nodiscard]] virtual auto
        CreateLocalSquawkAssignment(std::string callsign, std::string unit, std::string flightRules) const
            -> UKControllerPlugin::Squawk::ApiSquawkAllocation = 0;
        [[nodiscard]] virtual auto CheckApiAuthorisation() const -> bool = 0;
        virtual void DeleteSquawkAssignment(std::string callsign) const = 0;
        [[nodiscard]] virtual auto GetDependencyList() const -> nlohmann::json = 0;
        [[nodiscard]] virtual auto FetchRemoteFile(std::string uri) const -> std::string = 0;
        [[nodiscard]] virtual auto GetAssignedSquawk(std::string callsign) const
            -> UKControllerPlugin::Squawk::ApiSquawkAllocation = 0;
        [[nodiscard]] virtual auto GetApiDomain() const -> std::string = 0;
        [[nodiscard]] virtual auto GetApiKey() const -> std::string = 0;
        [[nodiscard]] virtual auto GetHoldDependency() const -> nlohmann::json = 0;
        [[nodiscard]] virtual auto GetAssignedHolds() const -> nlohmann::json = 0;
        virtual void AssignAircraftToHold(std::string callsign, std::string navaid) const = 0;
        virtual void UnassignAircraftHold(std::string callsign) const = 0;
        [[nodiscard]] virtual auto GetMinStackLevels() const -> nlohmann::json = 0;
        [[nodiscard]] virtual auto GetRegionalPressures() const -> nlohmann::json = 0;
        [[nodiscard]] virtual auto GetUri(std::string uri) const -> nlohmann::json = 0;
        [[nodiscard]] virtual auto SearchSrd(UKControllerPlugin::Srd::SrdSearchParameters params) const
            -> nlohmann::json = 0;
        [[nodiscard]] virtual auto GetAssignedStands() const -> nlohmann::json = 0;
        virtual void AssignStandToAircraft(std::string callsign, int standId) const = 0;
        virtual void DeleteStandAssignmentForAircraft(std::string callsign) const = 0;
        virtual void SendEnrouteRelease(
            std::string aircraftCallsign,
            std::string sendingController,
            std::string targetController,
            int releaseType) const = 0;
        virtual void SendEnrouteReleaseWithReleasePoint(
            std::string aircraftCallsign,
            std::string sendingController,
            std::string targetController,
            int releaseType,
            std::string releasePoint) const = 0;
        [[nodiscard]] virtual auto GetAllNotifications() const -> nlohmann::json = 0;
        [[nodiscard]] virtual auto GetUnreadNotifications() const -> nlohmann::json = 0;
        [[nodiscard]] virtual auto SyncPluginEvents() const -> nlohmann::json = 0;
        [[nodiscard]] virtual auto GetLatestPluginEvents(int lastEventId) const -> nlohmann::json = 0;
        virtual void AcknowledgeDepartureReleaseRequest(int releaseId, int controllerPositionId) const = 0;
        virtual void
        RejectDepartureReleaseRequest(int releaseId, int controllerPositionId, const std::string& remarks) const = 0;
        virtual void ApproveDepartureReleaseRequest(
            int releaseId,
            int controllerPositionId,
            std::chrono::system_clock::time_point releasedAt,
            int expiresInSeconds,
            const std::string& remarks) const = 0;
        [[nodiscard]] virtual auto RequestDepartureRelease(
            std::string callsign, int requestingControllerId, int targetControllerId, int expiresInSeconds) const
            -> nlohmann::json = 0;
        virtual void CancelDepartureReleaseRequest(int releaseId) const = 0;
        virtual void ReadNotification(int id) const = 0;
        [[nodiscard]] virtual auto GetUpdateDetails() const -> nlohmann::json = 0;

        [[nodiscard]] virtual auto CreatePrenoteMessage(
            const std::string& callsign,
            const std::string& departureAirfield,
            const std::string& departureSid,
            const std::string& destinationAirfield,
            int requestingController,
            int targetController,
            int requestExpiry) const -> nlohmann::json = 0;
        virtual void AcknowledgePrenoteMessage(int messageId, int controllerId) const = 0;
        virtual void DeletePrenoteMessage(int messageId) const = 0;
        [[nodiscard]] virtual auto CreateMissedApproach(const std::string& callsign) const -> nlohmann::json = 0;
        virtual void AcknowledgeMissedApproach(int id, const std::string& remarks) const = 0;

        virtual void SetApiKey(std::string key) = 0;
        virtual void SetApiDomain(std::string domain) = 0;
    };
} // namespace UKControllerPlugin::Api
