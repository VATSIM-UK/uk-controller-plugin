#pragma once
#include "squawk/ApiSquawkAllocation.h"
#include "srd/SrdSearchParameters.h"

namespace UKControllerPlugin {
    namespace Api {

        /**
         * An abstract class for the web API.
         */
        class ApiInterface
        {
            public:
                ApiInterface() {}
                virtual ~ApiInterface() {}
                virtual UKControllerPlugin::Squawk::ApiSquawkAllocation CreateGeneralSquawkAssignment(
                    std::string callsign,
                    std::string origin,
                    std::string destination
                ) const = 0;
                virtual UKControllerPlugin::Squawk::ApiSquawkAllocation CreateLocalSquawkAssignment(
                    std::string callsign,
                    std::string unit,
                    std::string flightRules
                ) const = 0;
                virtual bool CheckApiAuthorisation(void) const = 0;
                virtual void DeleteSquawkAssignment(std::string callsign) const = 0;
                virtual nlohmann::json GetDependencyList(void) const = 0;
                virtual std::string FetchRemoteFile(std::string uri) const = 0;
                virtual UKControllerPlugin::Squawk::ApiSquawkAllocation GetAssignedSquawk(
                    std::string callsign
                ) const = 0;
                virtual std::string GetApiDomain(void) const = 0;
                virtual std::string GetApiKey(void) const = 0;
                virtual nlohmann::json GetHoldDependency(void) const = 0;
                virtual nlohmann::json GetAssignedHolds(void) const = 0;
                virtual void AssignAircraftToHold(std::string callsign, std::string navaid) const = 0;
                virtual void UnassignAircraftHold(std::string callsign) const = 0;
                virtual nlohmann::json GetMinStackLevels(void) const = 0;
                virtual nlohmann::json GetRegionalPressures(void) const = 0;
                virtual nlohmann::json GetUri(std::string uri) const = 0;
                virtual nlohmann::json SearchSrd(UKControllerPlugin::Srd::SrdSearchParameters params) const = 0;
                virtual nlohmann::json GetAssignedStands(void) const = 0;
                virtual void AssignStandToAircraft(std::string callsign, int standId) const = 0;
                virtual void DeleteStandAssignmentForAircraft(std::string callsign) const = 0;
                virtual void SendEnrouteRelease(
                    std::string aircraftCallsign,
                    std::string sendingController,
                    std::string targetController,
                    int releaseType
                ) const = 0;
                virtual void SendEnrouteReleaseWithReleasePoint(
                    std::string aircraftCallsign,
                    std::string sendingController,
                    std::string targetController,
                    int releaseType,
                    std::string releasePoint
                ) const = 0;
                virtual nlohmann::json GetAllNotifications() const = 0;
                virtual nlohmann::json GetUnreadNotifications() const = 0;
                virtual nlohmann::json SyncPluginEvents() const = 0;
                virtual nlohmann::json GetLatestPluginEvents(int lastEventId) const = 0;
                virtual void AcknowledgeDepartureReleaseRequest(int releaseId, int controllerPositionId) const = 0;
                virtual void RejectDepartureReleaseRequest(int releaseId, int controllerPositionId) const = 0;
                virtual void ApproveDepartureReleaseRequest(
                    int releaseId,
                    int controllerPositionId,
                    std::chrono::system_clock::time_point releasedAt,
                    int expiresInSeconds
                ) const = 0;
                virtual nlohmann::json RequestDepartureRelease(
                    std::string callsign,
                    int requestingControllerId,
                    int targetControllerId,
                    int expiresInSeconds
                ) const = 0;
                virtual void CancelDepartureReleaseRequest(int releaseId) const = 0;
                virtual void ReadNotification(int id) const = 0;
                virtual int UpdateCheck(std::string version) const = 0;
                virtual nlohmann::json GetUpdateDetails() const = 0;
                virtual void SetApiKey(std::string key) = 0;
                virtual void SetApiDomain(std::string domain) = 0;

                // Codes returned after an update check
                static const int UPDATE_UP_TO_DATE = 0;
                static const int UPDATE_VERSION_DISABLED = 1;
                static const int UPDATE_VERSION_NEEDS_UPDATE = 2;
        };
    }  // namespace Api
}  // namespace UKControllerPlugin
