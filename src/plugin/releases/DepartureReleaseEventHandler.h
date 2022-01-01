#pragma once
#include "push/PushEventProcessorInterface.h"
#include "releases/CompareDepartureReleases.h"
#include "tag/TagItemInterface.h"
#include "timedevent/AbstractTimedEvent.h"

namespace UKControllerPlugin {
    namespace Controller {
        class ControllerPositionCollection;
        class ActiveCallsignCollection;
    } // namespace Controller
    namespace Dialog {
        class DialogManager;
    } // namespace Dialog
    namespace Euroscope {
        class EuroscopePluginLoopbackInterface;
        class EuroScopeCFlightPlanInterface;
        class EuroScopeCRadarTargetInterface;
        class EuroscopeFlightplanListInterface;
    } // namespace Euroscope
    namespace TaskManager {
        class TaskRunnerInterface;
    } // namespace TaskManager
    namespace Api {
        class ApiInterface;
    } // namespace Api
    namespace Windows {
        class WinApiInterface;
    } // namespace Windows

    namespace Releases {

        class DepartureReleaseRequest;

        /*
            Handles events relating to departure releases.
        */
        class DepartureReleaseEventHandler : public Push::PushEventProcessorInterface,
                                             public TimedEvent::AbstractTimedEvent,
                                             public Tag::TagItemInterface
        {
            public:
            DepartureReleaseEventHandler(
                const Api::ApiInterface& api,
                TaskManager::TaskRunnerInterface& taskRunner,
                Euroscope::EuroscopePluginLoopbackInterface& plugin,
                const Controller::ControllerPositionCollection& controllers,
                const Controller::ActiveCallsignCollection& activeCallsigns,
                const Dialog::DialogManager& dialogManager,
                Windows::WinApiInterface& windows,
                int releaseDecisionCallbackId,
                int releaseCancellationCallbackId);
            void ProcessPushEvent(const Push::PushEvent& message) override;
            [[nodiscard]] auto GetPushEventSubscriptions() const -> std::set<Push::PushEventSubscription> override;
            void PluginEventsSynced() override{};
            void AddReleaseRequest(const std::shared_ptr<DepartureReleaseRequest>& request);
            auto GetReleaseRequest(int id) -> std::shared_ptr<DepartureReleaseRequest>;
            void TimedEventTrigger() override;
            void OpenRequestDialog(
                Euroscope::EuroScopeCFlightPlanInterface& flightplan,
                Euroscope::EuroScopeCRadarTargetInterface& radarTarget,
                const std::string& context,
                const POINT& mousePos);
            void OpenDecisionMenu(
                Euroscope::EuroScopeCFlightPlanInterface& flightplan,
                Euroscope::EuroScopeCRadarTargetInterface& radarTarget,
                const std::string& context,
                const POINT& mousePos);
            void ReleaseDecisionMade(int functionId, const std::string& context, RECT);
            void RequestRelease(const std::string& callsign, int targetControllerId);
            void RejectRelease(
                int releaseId,
                std::string remarks);
            void ApproveRelease(
                int releaseId,
                std::chrono::system_clock::time_point releasedAt,
                int expiresInSeconds,
                std::string remarks);
            [[nodiscard]] auto GetTagItemDescription(int tagItemId) const -> std::string override;
            void SetTagItemData(Tag::TagData& tagData) override;
            void ShowStatusDisplay(
                Euroscope::EuroScopeCFlightPlanInterface& flightplan,
                Euroscope::EuroScopeCRadarTargetInterface& radarTarget,
                const std::string& context,
                const POINT& mousePos);
            [[nodiscard]] auto GetReleasesToDisplay() const
                -> const std::set<std::shared_ptr<DepartureReleaseRequest>>&;
            auto GetReleasesRequiringUsersDecision()
                -> std::set<std::shared_ptr<DepartureReleaseRequest>, CompareDepartureReleases>;
            void SelectReleaseRequestToCancel(
                Euroscope::EuroScopeCFlightPlanInterface& flightplan,
                Euroscope::EuroScopeCRadarTargetInterface& radarTarget,
                const std::string& context,
                const POINT& mousePos);
            void RequestCancelled(int functionId, const std::string& context, RECT);

            private:
            void ProcessDepartureReleaseRequestedMessage(const nlohmann::json& data);
            void ProcessRequestAcknowledgedMessage(const nlohmann::json& data);
            void ProcessRequestRejectedMessage(const nlohmann::json& data);
            void ProcessRequestApprovedMessage(const nlohmann::json& data);
            void ProcessRequestCancelledMessage(const nlohmann::json& data);

            [[nodiscard]] auto DepartureReleaseRequestedMessageValid(const nlohmann::json& data) const -> bool;
            [[nodiscard]] auto DepartureReleaseAcknowledgedMessageValid(const nlohmann::json& data) const -> bool;
            [[nodiscard]] auto DepartureReleaseRejectedMessageValid(const nlohmann::json& data) const -> bool;
            [[nodiscard]] auto DepartureReleaseApprovedMessageValid(const nlohmann::json& data) const -> bool;
            [[nodiscard]] auto DepartureReleaseCancelMessageValid(const nlohmann::json& data) const -> bool;
            static auto ReleaseShouldBeRemoved(const std::shared_ptr<DepartureReleaseRequest>& releaseRequest) -> bool;
            [[nodiscard]] auto
            ControllerCanMakeReleaseDecision(const std::shared_ptr<DepartureReleaseRequest>& releaseRequest) const
                -> bool;
            auto FindReleaseRequiringDecisionForCallsign(std::string callsign)
                -> std::shared_ptr<DepartureReleaseRequest>;
            void SetReleaseStatusIndicatorTagData(Tag::TagData& tagData);
            void SetReleaseCountdownTagData(Tag::TagData& tagData);
            void SetRequestingControllerTagData(Tag::TagData& tagData);
            [[nodiscard]] auto UserRequestedRelease(const std::shared_ptr<DepartureReleaseRequest>& request) const
                -> bool;

            // A guard on the map to allow async operations
            std::mutex releaseMapGuard;

            // Callback for when a release decision is made
            const int releaseDecisionCallbackId;

            // Callback for when a release request is cancelled
            const int releaseCancellationCallbackId;

            static const int STATUS_INDICATOR_TAG_ITEM_ID = 124;
            static const int COUNTDOWN_TAG_ITEM_ID = 125;
            static const int REQUESTING_CONTROLLER_TAG_ITEM_ID = 126;

            static const int RELEASE_EXPIRY_SECONDS = 300;
            static const int RELEASE_DECISION_MADE_DELETE_AFTER_SECONDS = 90;

            // Release requests in progress
            std::map<int, std::shared_ptr<DepartureReleaseRequest>> releaseRequests;

            // Controller positions
            const Controller::ControllerPositionCollection& controllers;

            // Plugin
            Euroscope::EuroscopePluginLoopbackInterface& plugin;

            // For loading dialogs
            const Dialog::DialogManager& dialogManager;

            // For making API calls
            const Api::ApiInterface& api;

            // For running tasks asynchronously
            TaskManager::TaskRunnerInterface& taskRunner;

            // All the active controllers
            const Controller::ActiveCallsignCollection& activeCallsigns;

            // A set of releases to display in the view
            std::set<std::shared_ptr<DepartureReleaseRequest>> releasesToDisplay;

            // Windows api interface for playing sounds
            Windows::WinApiInterface& windows;
        };
    } // namespace Releases
} // namespace UKControllerPlugin
