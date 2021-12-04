#include "api/ApiException.h"
#include "api/ApiHelper.h"
#include "api/ApiNotAuthorisedException.h"
#include "api/ApiNotFoundException.h"
#include "api/ApiResponseFactory.h"
#include "curl/CurlInterface.h"
#include "squawk/SquawkValidator.h"

using UKControllerPlugin::Api::ApiException;
using UKControllerPlugin::Api::ApiNotAuthorisedException;
using UKControllerPlugin::Api::ApiNotFoundException;
using UKControllerPlugin::Api::ApiRequestBuilder;
using UKControllerPlugin::Api::ApiResponseFactory;
using UKControllerPlugin::Curl::CurlInterface;
using UKControllerPlugin::Curl::CurlRequest;
using UKControllerPlugin::Curl::CurlResponse;
using UKControllerPlugin::Squawk::ApiSquawkAllocation;
using UKControllerPlugin::Squawk::SquawkValidator;
using UKControllerPlugin::Srd::SrdSearchParameters;

namespace UKControllerPlugin::Api {

    ApiHelper::ApiHelper(CurlInterface& curlApi, ApiRequestBuilder requestBuilder)
        : requestBuilder(std::move(requestBuilder)), curlApi(curlApi)
    {
    }

    /*
        Makes a request to the API.
    */
    auto ApiHelper::MakeApiRequest(const CurlRequest& request) const -> ApiResponse
    {
        CurlResponse response = this->curlApi.MakeCurlRequest(request);

        if (response.IsCurlError()) {
            LogError("cURL error when making API request, route: " + std::string(request.GetUri()));
            throw ApiException("ApiException when calling " + std::string(request.GetUri()));
        }

        if (response.GetStatusCode() == STATUS_SERVER_ERROR || response.GetStatusCode() == STATUS_SERVICE_UNAVAILBLE) {
            LogError("Internal server error when calling " + std::string(request.GetUri()));
            throw ApiException("ApiException, internal server error");
        }

        if (response.GetStatusCode() == STATUS_UNAUTHORISED || response.GetStatusCode() == STATUS_FORBIDDEN) {
            LogError("The API returned unauthorised when calling " + std::string(request.GetUri()));
            throw ApiNotAuthorisedException("The API returned 401 or 403");
        }

        if (response.GetStatusCode() == STATUS_BAD_REQUEST) {
            LogError("The API responed with bad request when calling " + std::string(request.GetUri()));
            throw ApiException("The API returned 400");
        }

        if (response.GetStatusCode() == STATUS_NOT_FOUND) {
            throw ApiNotFoundException("The API returned 404 for " + std::string(request.GetUri()));
        }

        // These are the only codes the API should be sending on success
        if (response.GetStatusCode() != STATUS_CREATED && response.GetStatusCode() != STATUS_NO_CONTENT &&
            response.GetStatusCode() != STATUS_OK) {
            LogError("Unknown API response occured, HTTP status was " + std::to_string(response.GetStatusCode()));
            throw ApiException("Unknown response");
        }

        return ApiResponseFactory::Create(response);
    }

    auto ApiHelper::ProcessSquawkResponse(const ApiResponse& response, const std::string& callsign)
        -> ApiSquawkAllocation
    {
        nlohmann::json responseJson = response.GetRawData();

        if (responseJson.count("squawk") != 1 || !responseJson["squawk"].is_string()) {
            LogError("No squawk in API response for " + callsign);
            throw ApiException("Invalid response returned from API");
        }

        if (!SquawkValidator::ValidSquawk(responseJson["squawk"])) {
            LogError("Invalid API response when requesting squawk assignment for " + callsign);
            throw ApiException("Invalid response returned from API");
        }

        // We should return false here, because if the API has a bad squawk assigned, we should try replacing it
        if (!SquawkValidator::AllowedSquawk(responseJson["squawk"])) {
            LogError("API returned invalid squawk when requesting squawk assignment for " + callsign);
            throw ApiException("Invalid squawk returned from API");
        }

        return ApiSquawkAllocation{callsign, responseJson["squawk"]};
    }

    /*
        Creates or updates a general squawk assignment (generates a new squawk for the aircraft).
    */
    auto
    ApiHelper::CreateGeneralSquawkAssignment(std::string callsign, std::string origin, std::string destination) const
        -> ApiSquawkAllocation
    {

        return this->ProcessSquawkResponse(
            this->MakeApiRequest(
                this->requestBuilder.BuildGeneralSquawkAssignmentRequest(callsign, origin, destination)),
            callsign);
    }

    /*
        Creates or updates a local squawk assignment (generates a new squawk for the aircraft).
    */
    auto ApiHelper::CreateLocalSquawkAssignment(std::string callsign, std::string unit, std::string flightRules) const
        -> ApiSquawkAllocation
    {
        return this->ProcessSquawkResponse(
            this->MakeApiRequest(this->requestBuilder.BuildLocalSquawkAssignmentRequest(callsign, unit, flightRules)),
            callsign);
    }

    /*
        Hits the API root to find out whether we're allowed in.
    */
    auto ApiHelper::CheckApiAuthorisation() const -> bool
    {
        return this->MakeApiRequest(this->requestBuilder.BuildAuthCheckRequest()).GetStatusCode() == STATUS_OK;
    }

    /*
        De allocate a squawk from a given aircraft.
    */
    void ApiHelper::DeleteSquawkAssignment(std::string callsign) const
    {
        static_cast<void>(this->MakeApiRequest(this->requestBuilder.BuildSquawkAssignmentDeletionRequest(callsign)));
    }

    /*
        Fetches the file at the given URI.
    */
    auto ApiHelper::FetchRemoteFile(std::string uri) const -> std::string
    {
        return this->MakeApiRequest(ApiRequestBuilder::BuildRemoteFileRequest(uri)).GetRawData().dump();
    }

    /*
        Get any currently assigned squawk for the aircraft
    */
    auto ApiHelper::GetAssignedSquawk(std::string callsign) const -> ApiSquawkAllocation
    {
        return this->ProcessSquawkResponse(
            this->MakeApiRequest(this->requestBuilder.BuildSquawkAssignmentCheckRequest(callsign)), callsign);
    }

    /*
        Returns the API domain being used by the request builder
    */
    auto ApiHelper::GetApiDomain() const -> std::string
    {
        return this->requestBuilder.GetApiDomain();
    }

    /*
        Returns the API key being used to authenticate requests
    */
    auto ApiHelper::GetApiKey() const -> std::string
    {
        return this->requestBuilder.GetApiKey();
    }

    auto ApiHelper::GetDependencyList() const -> nlohmann::json
    {
        return this->MakeApiRequest(this->requestBuilder.BuildDependencyListRequest()).GetRawData();
    }

    /*
        Returns the hold data dependency
    */
    auto ApiHelper::GetHoldDependency() const -> nlohmann::json
    {
        return this->MakeApiRequest(this->requestBuilder.BuildHoldDependencyRequest()).GetRawData();
    }

    auto ApiHelper::GetAssignedHolds() const -> nlohmann::json
    {
        return this->MakeApiRequest(this->requestBuilder.BuildAllAssignedHoldsRequest()).GetRawData();
    }

    void ApiHelper::AssignAircraftToHold(std::string callsign, std::string navaid) const
    {
        static_cast<void>(this->MakeApiRequest(this->requestBuilder.BuildSetAssignedHoldRequest(callsign, navaid)));
    }

    void ApiHelper::UnassignAircraftHold(std::string callsign) const
    {
        static_cast<void>(this->MakeApiRequest(this->requestBuilder.BuildDeleteAssignedHoldRequest(callsign)));
    }

    /*
        Request all the min stack levels
    */
    auto ApiHelper::GetMinStackLevels() const -> nlohmann::json
    {
        return this->MakeApiRequest(this->requestBuilder.BuildMinStackLevelRequest()).GetRawData();
    }

    auto ApiHelper::GetRegionalPressures() const -> nlohmann::json
    {
        return this->MakeApiRequest(this->requestBuilder.BuildRegionalPressureRequest()).GetRawData();
    }

    auto ApiHelper::GetUri(std::string uri) const -> nlohmann::json
    {
        if (uri.find(this->GetApiDomain()) == std::string::npos) {
            LogCritical("Attempted to get URI on non-ukcp route");
            throw ApiException("Attempted to get URI on non-ukcp route");
        }

        return this->MakeApiRequest(this->requestBuilder.BuildGetUriRequest(uri)).GetRawData();
    }

    auto ApiHelper::SearchSrd(SrdSearchParameters params) const -> nlohmann::json
    {
        return this->MakeApiRequest(this->requestBuilder.BuildSrdQueryRequest(params)).GetRawData();
    }

    auto ApiHelper::GetAssignedStands() const -> nlohmann::json
    {
        return this->MakeApiRequest(this->requestBuilder.BuildGetStandAssignmentsRequest()).GetRawData();
    }

    void ApiHelper::AssignStandToAircraft(std::string callsign, int standId) const
    {
        static_cast<void>(
            this->MakeApiRequest(this->requestBuilder.BuildAssignStandToAircraftRequest(callsign, standId)));
    }

    void ApiHelper::DeleteStandAssignmentForAircraft(std::string callsign) const
    {
        static_cast<void>(
            this->MakeApiRequest(this->requestBuilder.BuildDeleteStandAssignmentForAircraftRequest(callsign)));
    }

    void ApiHelper::SendEnrouteRelease(
        std::string aircraftCallsign,
        std::string sendingController,
        std::string targetController,
        int releaseType) const
    {
        static_cast<void>(this->MakeApiRequest(this->requestBuilder.BuildEnrouteReleaseRequest(
            aircraftCallsign, sendingController, targetController, releaseType)));
    }

    void ApiHelper::SendEnrouteReleaseWithReleasePoint(
        std::string aircraftCallsign,
        std::string sendingController,
        std::string targetController,
        int releaseType,
        std::string releasePoint) const
    {
        static_cast<void>(this->MakeApiRequest(this->requestBuilder.BuildEnrouteReleaseRequestWithReleasePoint(
            aircraftCallsign, sendingController, targetController, releaseType, releasePoint)));
    }

    auto ApiHelper::GetUpdateDetails() const -> nlohmann::json
    {
        return this->MakeApiRequest(this->requestBuilder.BuildLatestGithubVersionRequest()).GetRawData();
    }

    auto ApiHelper::GetAllNotifications() const -> nlohmann::json
    {
        return this->MakeApiRequest(this->requestBuilder.BuildGetAllNotificationsRequest()).GetRawData();
    }

    auto ApiHelper::GetUnreadNotifications() const -> nlohmann::json
    {
        return this->MakeApiRequest(this->requestBuilder.BuildGetUnreadNotificationsRequest()).GetRawData();
    }

    auto ApiHelper::SyncPluginEvents() const -> nlohmann::json
    {
        return this->MakeApiRequest(this->requestBuilder.BuildPluginEventSyncRequest()).GetRawData();
    }

    auto ApiHelper::GetLatestPluginEvents(int lastEventId) const -> nlohmann::json
    {
        return this->MakeApiRequest(this->requestBuilder.BuildGetLatestPluginEventsRequest(lastEventId)).GetRawData();
    }

    void ApiHelper::AcknowledgeDepartureReleaseRequest(int releaseId, int controllerPositionId) const
    {
        static_cast<void>(this->MakeApiRequest(
            this->requestBuilder.BuildAcknowledgeDepartureReleaseRequest(releaseId, controllerPositionId)));
    }

    void ApiHelper::RejectDepartureReleaseRequest(int releaseId, int controllerPositionId) const
    {
        static_cast<void>(this->MakeApiRequest(
            this->requestBuilder.BuildRejectDepartureReleaseRequest(releaseId, controllerPositionId)));
    }

    void ApiHelper::ApproveDepartureReleaseRequest(
        int releaseId,
        int controllerPositionId,
        std::chrono::system_clock::time_point releasedAt,
        int expiresInSeconds) const
    {
        static_cast<void>(this->MakeApiRequest(this->requestBuilder.BuildApproveDepartureReleaseRequest(
            releaseId, controllerPositionId, releasedAt, expiresInSeconds)));
    }

    auto ApiHelper::RequestDepartureRelease(
        std::string callsign, int requestingControllerId, int targetControllerId, int expiresInSeconds) const
        -> nlohmann::json
    {
        return this
            ->MakeApiRequest(this->requestBuilder.BuildDepartureReleaseRequest(
                callsign, requestingControllerId, targetControllerId, expiresInSeconds))
            .GetRawData();
    }

    void ApiHelper::CancelDepartureReleaseRequest(int releaseId) const
    {
        static_cast<void>(this->MakeApiRequest(this->requestBuilder.BuildCancelReleaseRequest(releaseId)));
    }

    void ApiHelper::ReadNotification(int id) const
    {
        static_cast<void>(this->MakeApiRequest(this->requestBuilder.BuildReadNotificationRequest(id)));
    }

    /*
        Set api key on the request builder
    */
    void ApiHelper::SetApiKey(std::string key)
    {
        this->requestBuilder.SetApiKey(key);
    }

    /*
        Set api domain on the request builder
    */
    void ApiHelper::SetApiDomain(std::string domain)
    {
        this->requestBuilder.SetApiDomain(domain);
    }

    auto ApiHelper::CreatePrenoteMessage(
        const std::string& callsign,
        const std::string& departureAirfield,
        const std::string& departureSid,
        const std::string& destinationAirfield,
        int requestingController,
        int targetController,
        int requestExpiry) const -> nlohmann::json
    {
        return this
            ->MakeApiRequest(this->requestBuilder.BuildCreatePrenoteMessageRequest(
                callsign,
                departureAirfield,
                departureSid,
                destinationAirfield,
                requestingController,
                targetController,
                requestExpiry))
            .GetRawData();
    }

    void ApiHelper::AcknowledgePrenoteMessage(int messageId, int controllerId) const
    {
        static_cast<void>(
            this->MakeApiRequest(this->requestBuilder.BuildAcknowledgePrenoteMessageRequest(messageId, controllerId)));
    }

    void ApiHelper::DeletePrenoteMessage(int messageId) const
    {
        static_cast<void>(this->MakeApiRequest(this->requestBuilder.BuildDeletePrenoteMessageRequest(messageId)));
    }

    auto ApiHelper::CreateMissedApproach(const std::string& callsign) const -> nlohmann::json
    {
        return this->MakeApiRequest(this->requestBuilder.BuildMissedApproachMessage(callsign)).GetRawData();
    }
    void ApiHelper::AcknowledgeMissedApproach(int id, const std::string& remarks) const
    {
        static_cast<void>(
            this->MakeApiRequest(this->requestBuilder.BuildMissedApproachAcknowledgeMessage(id, remarks)));
    }
} // namespace UKControllerPlugin::Api
