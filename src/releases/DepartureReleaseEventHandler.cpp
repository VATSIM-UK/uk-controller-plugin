#include "pch/stdafx.h"
#include "releases/DepartureReleaseEventHandler.h"
#include "api/ApiException.h"
#include "api/ApiInterface.h"
#include "controller/ActiveCallsignCollection.h"
#include "controller/ControllerPosition.h"
#include "controller/ControllerPositionCollection.h"
#include "dialog/DialogManager.h"
#include "releases/DepartureReleaseRequest.h"
#include "time/ParseTimeStrings.h"
#include "time/SystemClock.h"
#include "plugin/PopupMenuItem.h"
#include "euroscope/EuroscopePluginLoopbackInterface.h"
#include "task/TaskRunnerInterface.h"
#include "releases/DepartureReleaseCountdownColours.h"
#include "releases/DepartureReleaseColours.h"
#include "timer/TimerDisplay.h"
#include "releases/DepartureReleaseRequestView.h"

namespace UKControllerPlugin {
    namespace Releases {

        DepartureReleaseEventHandler::DepartureReleaseEventHandler(
            const Api::ApiInterface& api,
            TaskManager::TaskRunnerInterface& taskRunner,
            Euroscope::EuroscopePluginLoopbackInterface& plugin,
            const Controller::ControllerPositionCollection& controllers,
            const Controller::ActiveCallsignCollection& activeCallsigns,
            const Dialog::DialogManager& dialogManager,
            const int triggerRequestDialogFunctionId,
            const int triggerDecisionMenuFunctionId,
            const int releaseDecisionCallbackId
        ): triggerRequestDialogFunctionId(triggerRequestDialogFunctionId),
           triggerDecisionMenuFunctionId(triggerDecisionMenuFunctionId),
           releaseDecisionCallbackId(releaseDecisionCallbackId),
           controllers(controllers), plugin(plugin), dialogManager(dialogManager), api(api), taskRunner(taskRunner),
           activeCallsigns(activeCallsigns)
        {
            this->releaseRequests[1] = std::make_shared<DepartureReleaseRequest>(
                1,
                "BAW326",
                1,
                25,
                Time::TimeNow() + std::chrono::minutes(5)
            );
            this->releaseRequests[1]->Approve(
                Time::TimeNow() + std::chrono::seconds(90),
                Time::TimeNow() + std::chrono::seconds(180)
            );
        }

        void DepartureReleaseEventHandler::ProcessWebsocketMessage(const Websocket::WebsocketMessage& message)
        {
            if (message.event == "departure_release.requested") {
                this->ProcessDepartureReleaseRequestedMessage(message.data);
            } else if (message.event == "departure_release.approved") {
                this->ProcessRequestApprovedMessage(message.data);
            } else if (message.event == "departure_release.rejected") {
                this->ProcessRequestRejectedMessage(message.data);
            } else if (message.event == "departure_release.acknowledged") {
                this->ProcessRequestAcknowledgedMessage(message.data);
            } else if (message.event == "departure_release.request_cancelled") {
                this->ProcessRequestCancelledMessage(message.data);
            }
        }

        std::set<Websocket::WebsocketSubscription> DepartureReleaseEventHandler::GetSubscriptions() const
        {
            return {
                {
                    Websocket::WebsocketSubscription::SUB_TYPE_CHANNEL,
                    "private-departure-releases"
                }
            };
        }

        void DepartureReleaseEventHandler::AddReleaseRequest(std::shared_ptr<DepartureReleaseRequest> request)
        {
            std::lock_guard<std::mutex> queueLock(this->releaseMapGuard);
            this->releaseRequests[request->Id()] = request;
        }

        std::shared_ptr<DepartureReleaseRequest> DepartureReleaseEventHandler::GetReleaseRequest(int id)
        {
            std::lock_guard<std::mutex> queueLock(this->releaseMapGuard);
            auto request = this->releaseRequests.find(id);
            return request == this->releaseRequests.cend() ? nullptr : request->second;
        }

        bool DepartureReleaseEventHandler::DepartureReleaseRequestedMessageValid(const nlohmann::json& data) const
        {
            return data.is_object() &&
                data.contains("id") &&
                data.at("id").is_number_integer() &&
                data.contains("callsign") &&
                data.at("callsign").is_string() &&
                data.contains("requesting_controller") &&
                data.at("requesting_controller").is_number_integer() &&
                this->controllers.FetchPositionById(data.at("requesting_controller").get<int>()) != nullptr &&
                data.contains("target_controller") &&
                data.at("target_controller").is_number_integer() &&
                this->controllers.FetchPositionById(data.at("target_controller").get<int>()) != nullptr &&
                data.contains("expires_at") &&
                data.at("expires_at").is_string() &&
                Time::ParseTimeString(data.at("expires_at").get<std::string>()) != Time::invalidTime;
        }

        /*
         * An acknowledged message needs an id that we know about.
         */
        bool DepartureReleaseEventHandler::DepartureReleaseAcknowledgedMessageValid(const nlohmann::json& data) const
        {
            return data.is_object() &&
                data.contains("id") &&
                data.at("id").is_number_integer() &&
                this->releaseRequests.find(data.at("id").get<int>()) != this->releaseRequests.cend();
        }

        /*
         * For now, rejecting a release only needs a valid id, so borrow the acknowledge method.
         */
        bool DepartureReleaseEventHandler::DepartureReleaseRejectedMessageValid(const nlohmann::json& data) const
        {
            return this->DepartureReleaseAcknowledgedMessageValid(data);
        }

        bool DepartureReleaseEventHandler::DepartureReleaseApprovedMessageValid(const nlohmann::json& data) const
        {
            return data.is_object() &&
                data.contains("id") &&
                data.at("id").is_number_integer() &&
                this->releaseRequests.find(data.at("id").get<int>()) != this->releaseRequests.cend() &&
                data.contains("expires_at") &&
                data.at("expires_at").is_string() &&
                Time::ParseTimeString(data.at("expires_at").get<std::string>()) != Time::invalidTime &&
                data.contains("released_at") &&
                data.at("released_at").is_string() &&
                Time::ParseTimeString(data.at("released_at").get<std::string>()) != Time::invalidTime;
        }

        /*
         * For now, cancelling a release request only needs a valid id, so borrow the acknowledge method.
         */
        bool DepartureReleaseEventHandler::DepartureReleaseCancelMessageValid(const nlohmann::json& data) const
        {
            return this->DepartureReleaseAcknowledgedMessageValid(data);
        }

        /*
         * Whether releases should be removed from the lists.
         */
        bool DepartureReleaseEventHandler::ReleaseShouldBeRemoved(
            const std::shared_ptr<DepartureReleaseRequest>& releaseRequest
        )
        {
            if (releaseRequest->Approved()) {
                return releaseRequest->ReleaseExpiryTime() + std::chrono::seconds(90) <
                    std::chrono::system_clock::now();
            }

            if (releaseRequest->Rejected()) {
                return releaseRequest->RejectedAtTime() + std::chrono::seconds(90) <
                    std::chrono::system_clock::now();
            }

            return releaseRequest->RequestExpiryTime() < std::chrono::system_clock::now();
        }

        bool DepartureReleaseEventHandler::ControllerCanMakeReleaseDecision(
            const std::shared_ptr<DepartureReleaseRequest>& release
        ) const
        {
            if (!release) {
                LogError("Cannot approve release, release not found");
                return false;
            }

            if (
                !this->activeCallsigns.UserHasCallsign() ||
                this->activeCallsigns.GetUserCallsign().GetNormalisedPosition().GetId() != release->TargetController()
            ) {
                LogError("Cannot approve release, user not authorised to approve");
                return false;
            }

            return true;
        }

        std::string DepartureReleaseEventHandler::GetTagItemDescription(int tagItemId) const
        {
            switch (tagItemId) {
                case 124:
                    return "Departure Release Status Indicator";
                case 125:
                    return "Departure Release Countdown";
                default:
                    return "";
            }
        }

        void DepartureReleaseEventHandler::SetTagItemData(Tag::TagData& tagData)
        {
            switch (tagData.itemCode) {
                case 124:
                    this->SetReleaseStatusIndicatorTagData(tagData);
                    break;
                case 125:
                    this->SetReleaseCountdownTagData(tagData);
                    break;
                default:
                    break;
            }
        }

        void DepartureReleaseEventHandler::ShowStatusDisplay(
            Euroscope::EuroScopeCFlightPlanInterface& flightplan,
            Euroscope::EuroScopeCRadarTargetInterface& radarTarget,
            std::string context,
            const POINT& mousePos
        )
        {
            std::string callsign = flightplan.GetCallsign();
            std::lock_guard<std::mutex> queueLock(this->releaseMapGuard);
            std::set<std::shared_ptr<DepartureReleaseRequest>> releasesForCallsign;
            std::for_each(
                this->releaseRequests.cbegin(),
                this->releaseRequests.cend(),
                [callsign, &releasesForCallsign](
                const std::pair<int, std::shared_ptr<DepartureReleaseRequest>>& release)
                {
                    if (release.second->Callsign() == callsign) {
                        releasesForCallsign.insert(release.second);
                    }
                }
            );

            // Dont continue if nothing to display
            if (releasesForCallsign.empty()) {
                return;
            }

            // Replace the display list
            this->releasesToDisplay = releasesForCallsign;

            // Setup the displays time and position
            DepartureReleaseRequestView::MoveAllInstances(mousePos);
            DepartureReleaseRequestView::DisplayFor(std::chrono::seconds(3));
        }

        const std::set<std::shared_ptr<DepartureReleaseRequest>>& DepartureReleaseEventHandler::
        GetReleasesToDisplay() const
        {
            return this->releasesToDisplay;
        }

        const std::shared_ptr<DepartureReleaseRequest>
        DepartureReleaseEventHandler::FindReleaseRequiringDecisionForCallsign(
            std::string callsign
        )
        {
            if (!this->activeCallsigns.UserHasCallsign()) {
                return nullptr;
            }

            int userControllerId = this->activeCallsigns.GetUserCallsign().GetNormalisedPosition().GetId();
            std::lock_guard<std::mutex> queueLock(this->releaseMapGuard);
            auto release = std::find_if(
                this->releaseRequests.cbegin(),
                this->releaseRequests.cend(),
                [&callsign, userControllerId]
            (const std::pair<int, std::shared_ptr<DepartureReleaseRequest>>& release) -> bool
                {
                    return release.second->Callsign() == callsign &&
                        release.second->RequiresDecision() &&
                        userControllerId == release.second->TargetController();
                }
            );

            return release != this->releaseRequests.cend() ? release->second : nullptr;
        }

        void DepartureReleaseEventHandler::SetReleaseStatusIndicatorTagData(Tag::TagData& tagData)
        {
            std::lock_guard<std::mutex> queueLock(this->releaseMapGuard);

            /*
             * Go through all the release requests and find the most recent one for each target controller.
             */
            int approvals = 0;
            int rejections = 0;
            int expiries = 0;
            int awaitingReleasedAtTime = 0;
            int relevant = 0;
            for (
                auto releaseRequest = this->releaseRequests.rbegin();
                releaseRequest != this->releaseRequests.rend();
                ++releaseRequest

            ) {
                if (releaseRequest->second->Callsign() != tagData.flightPlan.GetCallsign()) {
                    continue;
                }


                if (releaseRequest->second->Approved()) {
                    approvals++;
                    if (releaseRequest->second->ApprovalExpired()) {
                        expiries++;
                    }

                    if (releaseRequest->second->AwaitingReleasedTime()) {
                        awaitingReleasedAtTime++;
                    }
                } else if (releaseRequest->second->Rejected()) {
                    rejections++;
                } else if (releaseRequest->second->RequestExpired()) {
                    expiries++;
                }

                relevant++;
            }

            // No releases, nothing to do
            if (relevant == 0) {
                return;
            }

            /*
             * Set the indicator colour and text.
             *
             * If any release request is rejected: display rejected colour.
             * If all outstanding requests are approved:
             * - Show released colour if all releases have passed release time
             * - Show pending time colour if still waiting for release time to pass
             * - Show expired colour if release has expired
             * Otherwise: display pending
             */
            COLORREF indicatorColour = statusIndicatorReleasePending;
            if (rejections != 0) {
                indicatorColour = statusIndicatorReleaseRejected;
            } else if (expiries != 0) {
                indicatorColour = statusIndicatorReleaseExpired;
            } else if (approvals == relevant) {
                indicatorColour = awaitingReleasedAtTime != 0
                                      ? statusIndicatorReleasedAwaitingTime
                                      : statusIndicatorReleased;
            }
            tagData.SetItemString(std::to_string(approvals - expiries) + "/" + std::to_string(relevant));
            tagData.SetTagColour(indicatorColour);
        }

        /*
         * Get the release countdown timer.
         */
        void DepartureReleaseEventHandler::SetReleaseCountdownTagData(Tag::TagData& tagData)
        {
            std::lock_guard<std::mutex> queueLock(this->releaseMapGuard);

            std::set<std::shared_ptr<DepartureReleaseRequest>> relevantReleases;
            int releasesPendingReleaseTime = 0;
            for (
                auto releaseRequest = this->releaseRequests.rbegin();
                releaseRequest != this->releaseRequests.rend();
                ++releaseRequest
            ) {
                if (releaseRequest->second->Callsign() != tagData.flightPlan.GetCallsign()) {
                    continue;
                }

                // If we find a release that isn't approved or approval has expired, do nothing
                if (
                    !releaseRequest->second->Approved() ||
                    (releaseRequest->second->Approved() && releaseRequest->second->ApprovalExpired())
                ) {
                    return;
                }

                if (releaseRequest->second->AwaitingReleasedTime()) {
                    releasesPendingReleaseTime++;
                }

                relevantReleases.insert(releaseRequest->second);
            }

            // No releases, nothing to do
            if (relevantReleases.empty()) {
                return;
            }

            std::chrono::system_clock::time_point countdownTime;
            if (releasesPendingReleaseTime != 0) {
                // Find the release that has a released at time that is furthest away
                std::chrono::system_clock::time_point furthestPendingRelease = (
                    std::chrono::system_clock::time_point::min)();

                for (auto relevantRelease : relevantReleases) {
                    if (relevantRelease->ReleasedAtTime() > furthestPendingRelease) {
                        furthestPendingRelease = relevantRelease->ReleasedAtTime();
                    }
                }

                countdownTime = furthestPendingRelease;
                tagData.SetTagColour(PendingReleaseTimeColour());
            } else {
                // Find the release that has the closest expiry time
                std::chrono::system_clock::time_point closestReleaseExpiry = (
                    std::chrono::system_clock::time_point::max)();

                for (auto relevantRelease : relevantReleases) {
                    if (relevantRelease->ReleaseExpiryTime() < closestReleaseExpiry) {
                        closestReleaseExpiry = relevantRelease->ReleaseExpiryTime();
                    }
                }
                countdownTime = closestReleaseExpiry;
                tagData.SetTagColour(TimeUntilExpiryColour(closestReleaseExpiry));
            }

            // Set the timer display
            tagData.SetItemString(Timer::GetTimerDisplay(countdownTime));
        }

        /**
         * Create a new departure release request.
         */
        void DepartureReleaseEventHandler::ProcessDepartureReleaseRequestedMessage(const nlohmann::json& data)
        {
            if (!DepartureReleaseRequestedMessageValid(data)) {
                LogError("Invalid departure release requested message");
                return;
            }

            // Add the release
            int releaseRequestId = data.at("id").get<int>();
            int targetController = data.at("target_controller").get<int>();
            std::string callsign = data.at("callsign").get<std::string>();
            this->releaseRequests[data.at("id").get<int>()] = std::make_shared<DepartureReleaseRequest>(
                releaseRequestId,
                callsign,
                data.at("requesting_controller").get<int>(),
                targetController,
                Time::ParseTimeString(data.at("expires_at").get<std::string>())
            );

            // Remove any others for the same callsign and controller
            for (
                auto releaseRequest = this->releaseRequests.begin();
                releaseRequest != this->releaseRequests.end();
            ) {
                if (
                    releaseRequest->second->Callsign() == callsign &&
                    releaseRequest->second->TargetController() == targetController &&
                    releaseRequest->second->Id() != releaseRequestId
                ) {
                    releaseRequest = this->releaseRequests.erase(releaseRequest);
                } else {
                    ++releaseRequest;
                }
            }
        }

        /**
         * Acknowledge a departure release request.
         */
        void DepartureReleaseEventHandler::ProcessRequestAcknowledgedMessage(const nlohmann::json& data)
        {
            if (!DepartureReleaseAcknowledgedMessageValid(data)) {
                LogError("Invalid departure release acknowledged message");
                return;
            }

            this->releaseRequests.find(data.at("id").get<int>())->second->Acknowledge();
        }

        /**
         * Reject a departure release request.
         */
        void DepartureReleaseEventHandler::ProcessRequestRejectedMessage(const nlohmann::json& data)
        {
            if (!DepartureReleaseRejectedMessageValid(data)) {
                LogError("Invalid departure release rejected message");
                return;
            }

            this->releaseRequests.find(data.at("id").get<int>())->second->Reject();
        }

        /**
         * Approve a departure release request.
         */
        void DepartureReleaseEventHandler::ProcessRequestApprovedMessage(const nlohmann::json& data)
        {
            if (!DepartureReleaseApprovedMessageValid(data)) {
                LogError("Invalid departure release approved message");
                return;
            }

            this->releaseRequests.find(data.at("id").get<int>())->second->Approve(
                Time::ParseTimeString(data.at("released_at").get<std::string>()),
                Time::ParseTimeString(data.at("expires_at").get<std::string>())
            );
        }

        /**
         * Cancel a departure release request.
         */
        void DepartureReleaseEventHandler::ProcessRequestCancelledMessage(const nlohmann::json& data)
        {
            if (!DepartureReleaseCancelMessageValid(data)) {
                LogError("Invalid departure release cancelled message");
                return;
            }

            std::lock_guard<std::mutex> queueLock(this->releaseMapGuard);
            this->releaseRequests.erase(data.at("id").get<int>());
        }

        /*
         * Remove releases that have expired and no longer need
         * to be displayed.
         */
        void DepartureReleaseEventHandler::TimedEventTrigger()
        {
            std::lock_guard<std::mutex> queueLock(this->releaseMapGuard);
            for (
                auto release = this->releaseRequests.cbegin();
                release != this->releaseRequests.cend();
            ) {
                if (this->ReleaseShouldBeRemoved(release->second)) {
                    release = this->releaseRequests.erase(release);
                } else {
                    ++release;
                }
            }
        }

        /*
         * Open the departure release request dialog to start requesting a new
         * release.
         */
        void DepartureReleaseEventHandler::OpenRequestDialog(
            Euroscope::EuroScopeCFlightPlanInterface& flightplan,
            Euroscope::EuroScopeCRadarTargetInterface& radarTarget,
            std::string context,
            const POINT& mousePos
        )
        {
            if (
                !this->activeCallsigns.UserHasCallsign() ||
                !this->activeCallsigns.GetUserCallsign().GetNormalisedPosition().RequestsDepartureReleases()
            ) {
                LogWarning("Not opening departure release request dialog, user cannot request releases");
                return;
            }

            std::string callsign = flightplan.GetCallsign();
            this->dialogManager.OpenDialog(IDD_DEPARTURE_RELEASE_REQUEST, reinterpret_cast<LPARAM>(callsign.c_str()));
        }

        /*
         * Open the decision popup menu for a particular departure release.
         */
        void DepartureReleaseEventHandler::OpenDecisionMenu(
            Euroscope::EuroScopeCFlightPlanInterface& flightplan,
            Euroscope::EuroScopeCRadarTargetInterface& radarTarget,
            std::string context,
            const POINT& mousePos
        )
        {
            auto release = this->FindReleaseRequiringDecisionForCallsign(flightplan.GetCallsign());
            if (!release) {
                return;
            }

            // Create the list in place
            RECT popupArea = {
                mousePos.x,
                mousePos.y,
                mousePos.x + 400,
                mousePos.y + 600
            };

            this->plugin.TriggerPopupList(
                popupArea,
                "Departure Release Decision",
                1
            );

            Plugin::PopupMenuItem menuItem;
            menuItem.firstValue = "";
            menuItem.secondValue = "";
            menuItem.callbackFunctionId = this->releaseDecisionCallbackId;
            menuItem.checked = EuroScopePlugIn::POPUP_ELEMENT_NO_CHECKBOX;
            menuItem.disabled = false;
            menuItem.fixedPosition = false;

            menuItem.firstValue = "Approve";
            this->plugin.AddItemToPopupList(menuItem);
            menuItem.firstValue = "Reject";
            this->plugin.AddItemToPopupList(menuItem);
            menuItem.firstValue = "Acknowledge";
            this->plugin.AddItemToPopupList(menuItem);
        }

        /*
         * Departure release decision has been made, process that decision.
         */
        void DepartureReleaseEventHandler::ReleaseDecisionMade(int functionId, std::string context, RECT)
        {
            // Check we can make this decision
            auto fp = this->plugin.GetSelectedFlightplan();
            if (!fp) {
                LogWarning("Cannot make release decision, no callsign selected");
                return;
            }

            auto release = this->FindReleaseRequiringDecisionForCallsign(fp->GetCallsign());
            if (!release) {
                return;
            }

            // Release has been approved
            if (context == "Approve") {
                this->dialogManager.OpenDialog(
                    IDD_DEPARTURE_RELEASE_APPROVE,
                    reinterpret_cast<LPARAM>(&release)
                );
                return;
            }

            // Release has been rejected
            if (context == "Reject") {
                this->taskRunner.QueueAsynchronousTask([this, &release]()
                {
                    try {
                        this->api.RejectDepartureReleaseRequest(
                            release->Id(),
                            release->TargetController()
                        );
                        release->Reject();
                        LogInfo("Rejected departure release id " + std::to_string(release->Id()));
                    } catch (Api::ApiException apiException) {
                        LogError("ApiException when rejecting departure release: "
                            + std::string(apiException.what()));
                    }
                });
                return;
            }

            // Release has been acknowledged
            this->taskRunner.QueueAsynchronousTask([this, &release]()
            {
                try {
                    this->api.AcknowledgeDepartureReleaseRequest(
                        release->Id(),
                        release->TargetController()
                    );
                    release->Acknowledge();
                    LogInfo("Acknowledged departure release id " + std::to_string(release->Id()));
                } catch (Api::ApiException apiException) {
                    LogError("ApiException when acknowledging departure release: "
                        + std::string(apiException.what()));
                }
            });
        }

        /*
         * Given a callsign and a target controller, check that releases can be peformed
         * and perform the request. Create the request locally once we have an id to
         * tide us over until the next event update.
         */
        void DepartureReleaseEventHandler::RequestRelease(std::string callsign, int targetControllerId)
        {
            this->taskRunner.QueueAsynchronousTask([this, callsign, targetControllerId]()
            {
                auto controller = this->controllers.FetchPositionById(targetControllerId);
                if (!controller || !controller->ReceivesDepartureReleases()) {
                    LogError("Cannot request release, target controller is invalid");
                    return;
                }

                if (
                    !this->activeCallsigns.UserHasCallsign() ||
                    !this->activeCallsigns.GetUserCallsign().GetNormalisedPosition().RequestsDepartureReleases()
                ) {
                    LogError("Cannot request release, controller position cannot request releases");
                    return;
                }

                int userPositionId = this->activeCallsigns.GetUserCallsign().GetNormalisedPosition().GetId();

                try {
                    nlohmann::json response = this->api.RequestDepartureRelease(
                        callsign,
                        this->activeCallsigns.GetUserCallsign().GetNormalisedPosition().GetId(),
                        targetControllerId,
                        300
                    );

                    if (!response.contains("id") || !response.at("id").is_number_integer()) {
                        LogError("Api returned invalid departure release id");
                        return;
                    }

                    this->AddReleaseRequest(
                        std::make_shared<DepartureReleaseRequest>(
                            response.at("id").get<int>(),
                            callsign,
                            userPositionId,
                            targetControllerId,
                            Time::TimeNow() + std::chrono::seconds(300)
                        )
                    );
                    LogInfo(
                        "Requested departure release id " + std::to_string(response.at("id").get<int>())
                        + " for " + callsign
                    );
                } catch (Api::ApiException apiException) {
                    LogError("ApiException when requesting departure release");
                }
            });
        }

        /*
         * Given a release id, approve it.
         */
        void DepartureReleaseEventHandler::ApproveRelease(
            int releaseId,
            std::chrono::system_clock::time_point releasedAt,
            int expiresInSeconds
        )
        {
            auto release = this->GetReleaseRequest(releaseId);
            if (!this->ControllerCanMakeReleaseDecision(release)) {
                return;
            }

            this->taskRunner.QueueAsynchronousTask([this, release, releasedAt, expiresInSeconds]()
            {
                try {
                    this->api.ApproveDepartureReleaseRequest(
                        release->Id(),
                        release->TargetController(),
                        releasedAt,
                        expiresInSeconds
                    );
                    release->Approve(releasedAt, releasedAt + std::chrono::seconds(expiresInSeconds));
                    LogInfo("Approved departure release id " + std::to_string(release->Id()));
                } catch (Api::ApiException) {
                    LogError("ApiException approving departure release " + std::to_string(release->Id()));
                }
            });
        }
    } // namespace Releases
}  // namespace UKControllerPlugin
