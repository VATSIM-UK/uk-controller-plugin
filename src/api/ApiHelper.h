#pragma once
#include "api/ApiResponse.h"
#include "api/ApiRequestBuilder.h"
#include "api/ApiInterface.h"

namespace UKControllerPlugin {
    namespace Curl {
        class CurlInterface;
        class CurlRequest;
    }  // namespace Curl
    namespace Windows {
        class WinApiInterface;
    }  // namespace Windows
}  // namespace UKControllerPlugin

namespace UKControllerPlugin {
    namespace Api {

        /*
            A class for making requests to the UKCP API.
        */
        class ApiHelper : public UKControllerPlugin::Api::ApiInterface
        {
            public:
                ApiHelper(
                    UKControllerPlugin::Curl::CurlInterface & curlApi,
                    ApiRequestBuilder requestBuilder
                );

                UKControllerPlugin::Squawk::ApiSquawkAllocation CreateGeneralSquawkAssignment(
                    std::string callsign,
                    std::string origin,
                    std::string destination
                ) const override;
                UKControllerPlugin::Squawk::ApiSquawkAllocation CreateLocalSquawkAssignment(
                    std::string callsign,
                    std::string unit,
                    std::string flightRules
                ) const override;
                std::string AuthoriseWebsocketChannel(std::string socketId, std::string channel) const override;
                bool CheckApiAuthorisation(void) const override;
                void DeleteSquawkAssignment(std::string callsign) const override;
                std::string FetchRemoteFile(std::string uri) const override;
                UKControllerPlugin::Squawk::ApiSquawkAllocation GetAssignedSquawk(std::string callsign) const override;
                std::string GetApiDomain(void) const override;
                std::string GetApiKey(void) const override;
                nlohmann::json GetDependencyList(void) const override;
                nlohmann::json GetHoldDependency(void) const override;
                nlohmann::json GetAssignedHolds(void) const override;;
                void AssignAircraftToHold(std::string callsign, std::string navaid) const override;
                void UnassignAircraftHold(std::string callsign) const override;
                nlohmann::json GetMinStackLevels(void) const override;
                nlohmann::json GetRegionalPressures(void) const override;
                nlohmann::json GetUri(std::string uri) const;
                nlohmann::json SearchSrd(Srd::SrdSearchParameters params) const override;
                nlohmann::json GetAssignedStands(void) const override;
                void AssignStandToAircraft(std::string callsign, int standId) const override;
                void DeleteStandAssignmentForAircraft(std::string callsign) const override;
                void SendEnrouteRelease(
                    std::string aircraftCallsign,
                    std::string sendingController,
                    std::string targetController,
                    int releaseType
                ) const override;
                void SendEnrouteReleaseWithReleasePoint(
                    std::string aircraftCallsign,
                    std::string sendingController,
                    std::string targetController,
                    int releaseType,
                    std::string releasePoint
                ) const override;
                nlohmann::json GetUpdateDetails() const override;
                nlohmann::json GetAllNotifications() const override;
                nlohmann::json GetUnreadNotifications() const override;
                void ReadNotification(int id) const override;
                int UpdateCheck(std::string version) const override;
                void SetApiKey(std::string key) override;
                void SetApiDomain(std::string domain) override;

                // The HTTP status codes that may be returned by the API
                static const uint64_t STATUS_OK = 200L;
                static const uint64_t STATUS_CREATED = 201L;
                static const uint64_t STATUS_NO_CONTENT = 204L;
                static const uint64_t STATUS_BAD_REQUEST = 400L;
                static const uint64_t STATUS_UNAUTHORISED = 401L;
                static const uint64_t STATUS_FORBIDDEN = 403L;
                static const uint64_t STATUS_NOT_FOUND = 404L;
                static const uint64_t STATUS_TEAPOT = 418L;
                static const uint64_t STATUS_SERVER_ERROR = 500L;
                static const uint64_t STATUS_SERVICE_UNAVAILBLE = 503L;

            private:

                ApiResponse MakeApiRequest(
                    const UKControllerPlugin::Curl::CurlRequest request
                ) const;

                UKControllerPlugin::Squawk::ApiSquawkAllocation ProcessSquawkResponse(
                    const ApiResponse response,
                    std::string callsign
                ) const;

                // The API request builder, that builds our CurlRequests
                UKControllerPlugin::Api::ApiRequestBuilder requestBuilder;

                // An interface to the Curl library.
                UKControllerPlugin::Curl::CurlInterface & curlApi;
        };
    }  // namespace Api
}  // namespace UKControllerPlugin
