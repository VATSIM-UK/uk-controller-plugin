#include "EnrouteReleaseEventHandler.h"
#include "api/ApiException.h"
#include "api/ApiInterface.h"
#include "euroscope/EuroScopeCControllerInterface.h"
#include "euroscope/EuroScopeCFlightPlanInterface.h"
#include "euroscope/EuroScopeCRadarTargetInterface.h"
#include "euroscope/EuroscopePluginLoopbackInterface.h"
#include "tag/TagData.h"
#include "task/TaskRunnerInterface.h"

using UKControllerPlugin::Api::ApiException;
using UKControllerPlugin::Api::ApiInterface;
using UKControllerPlugin::Euroscope::EuroScopeCControllerInterface;
using UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface;
using UKControllerPlugin::Euroscope::EuroScopeCRadarTargetInterface;
using UKControllerPlugin::Euroscope::EuroscopePluginLoopbackInterface;
using UKControllerPlugin::Plugin::PopupMenuItem;
using UKControllerPlugin::Push::PushEvent;
using UKControllerPlugin::Push::PushEventSubscription;
using UKControllerPlugin::TaskManager::TaskRunnerInterface;

namespace UKControllerPlugin::Releases {

    EnrouteReleaseEventHandler::EnrouteReleaseEventHandler(
        const ApiInterface& api,
        EuroscopePluginLoopbackInterface& plugin,
        TaskRunnerInterface& taskRunner,
        std::set<EnrouteReleaseType, CompareEnrouteReleaseTypes> releaseTypes,
        const int releaseTypeSelectedCallbackId,
        const int editReleasePointCallbackId)
        : api(api), plugin(plugin), taskRunner(taskRunner), releaseTypes(std::move(releaseTypes)),
          releaseTypeSelectedCallbackId(releaseTypeSelectedCallbackId),
          editReleasePointCallbackId(editReleasePointCallbackId)
    {
    }

    void EnrouteReleaseEventHandler::AddIncomingRelease(const std::string& callsign, EnrouteRelease release)
    {
        this->incomingReleases[callsign] = std::move(release);
    }

    void EnrouteReleaseEventHandler::AddOutgoingRelease(const std::string& callsign, EnrouteRelease release)
    {
        this->outgoingReleases[callsign] = std::move(release);
    }

    auto EnrouteReleaseEventHandler::GetIncomingRelease(const std::string& callsign) const -> const EnrouteRelease&
    {
        if (this->incomingReleases.count(callsign) == 0) {
            return this->invalidRelease;
        }

        return this->incomingReleases.at(callsign);
    }

    auto EnrouteReleaseEventHandler::GetOutgoingRelease(const std::string& callsign) const -> const EnrouteRelease&
    {
        if (this->outgoingReleases.count(callsign) == 0) {
            return this->invalidRelease;
        }

        return this->outgoingReleases.at(callsign);
    }

    void EnrouteReleaseEventHandler::ProcessPushEvent(const PushEvent& message)
    {
        if (message.event != "App\\Events\\EnrouteReleaseEvent") {
            return;
        }

        if (!this->ReleaseMessageValid(message.data)) {
            LogWarning("Invalid enroute release message");
            return;
        }

        // Only do an incoming release if I'm the target
        std::shared_ptr<EuroScopeCControllerInterface> myController = this->plugin.GetUserControllerObject();
        if (!myController || message.data.at("target_controller").get<std::string>() != myController->GetCallsign()) {
            return;
        }

        // Add the incoming release
        LogInfo("Received enroute release for " + message.data.at("callsign").get<std::string>());
        this->incomingReleases[message.data.at("callsign").get<std::string>()] = {
            message.data.at("type").get<int>(),
            message.data.at("release_point").is_null() ? "" : message.data.at("release_point").get<std::string>(),
            std::chrono::system_clock::now() + std::chrono::minutes(3)};
    }

    auto EnrouteReleaseEventHandler::GetPushEventSubscriptions() const -> std::set<PushEventSubscription>
    {
        return {{PushEventSubscription::SUB_TYPE_CHANNEL, "private-enroute-releases"}};
    }

    auto EnrouteReleaseEventHandler::GetTagItemDescription(int tagItemId) const -> std::string
    {
        return this->tagItemDescriptions.count(tagItemId) != 0 ? this->tagItemDescriptions.at(tagItemId) : "";
    }

    void EnrouteReleaseEventHandler::SetTagItemData(UKControllerPlugin::Tag::TagData& tagData)
    {
        const auto itemCode = tagData.GetItemCode();
        const auto& flightplan = tagData.GetFlightplan();
        if (itemCode == enrouteReleaseTypeTagItemId) {
            tagData.SetItemString(this->noReleaseItemColumn1);

            // We can skip for valid release type checks here as this is checked on incoming messages
            // Prioritise displaying outgoing releases
            if (this->outgoingReleases.count(flightplan.GetCallsign()) != 0) {
                const EnrouteReleaseType& releaseType =
                    *this->releaseTypes.find(this->outgoingReleases.at(flightplan.GetCallsign()).releaseType);

                tagData.SetItemString(releaseType.tagString);
                tagData.SetEuroscopeColourCode(EuroScopePlugIn::TAG_COLOR_ONGOING_REQUEST_FROM_ME);
            } else if (this->incomingReleases.count(flightplan.GetCallsign()) != 0) {
                const EnrouteReleaseType& releaseType =
                    *this->releaseTypes.find(this->incomingReleases.at(flightplan.GetCallsign()).releaseType);

                tagData.SetItemString(releaseType.tagString);
                tagData.SetEuroscopeColourCode(EuroScopePlugIn::TAG_COLOR_TRANSFER_TO_ME_INITIATED);
            }
        } else if (itemCode == enrouteReleasePointTagItemId || itemCode == enrouteReleasePointOrBlankTagItemId) {
            tagData.SetItemString(GetNoReleasePointText(itemCode));

            // Prioritise displaying outgoing releases
            if (this->outgoingReleases.count(flightplan.GetCallsign()) != 0) {
                if (this->outgoingReleases.at(flightplan.GetCallsign()).releasePoint == this->noReleasePoint) {
                    return;
                }

                tagData.SetItemString(this->outgoingReleases.at(flightplan.GetCallsign()).releasePoint);
                tagData.SetEuroscopeColourCode(EuroScopePlugIn::TAG_COLOR_ONGOING_REQUEST_FROM_ME);
            } else if (this->incomingReleases.count(flightplan.GetCallsign()) != 0) {
                if (this->incomingReleases.at(flightplan.GetCallsign()).releasePoint == this->noReleasePoint) {
                    return;
                }

                tagData.SetItemString(this->incomingReleases.at(flightplan.GetCallsign()).releasePoint);
                tagData.SetEuroscopeColourCode(EuroScopePlugIn::TAG_COLOR_TRANSFER_TO_ME_INITIATED);
            }
        }
    }

    /*
        Tidy up any releases that we don't need anymore
    */
    void EnrouteReleaseEventHandler::TimedEventTrigger()
    {
        // Clear incoming releases
        for (auto incomingIt = this->incomingReleases.cbegin(); incomingIt != this->incomingReleases.cend();) {
            if (incomingIt->second.clearTime < std::chrono::system_clock::now()) {
                this->incomingReleases.erase(incomingIt++);
            } else {
                ++incomingIt;
            }
        }

        // Clear outgoing releases
        for (auto outgoingIt = this->outgoingReleases.cbegin(); outgoingIt != this->outgoingReleases.cend();) {
            if (outgoingIt->second.clearTime < std::chrono::system_clock::now()) {
                this->outgoingReleases.erase(outgoingIt++);
            } else {
                ++outgoingIt;
            }
        }
    }

    void EnrouteReleaseEventHandler::UpdateOutgoingReleaseType(const std::string& callsign, int type)
    {
        if (this->outgoingReleases.count("callsign") != 0) {
            this->outgoingReleases.at("callsign").releaseType = type;
            return;
        }

        this->outgoingReleases[callsign] = {type, this->noReleasePoint, (std::chrono::system_clock::time_point::max)()};
    }

    auto EnrouteReleaseEventHandler::GetNoReleasePointText(int tagId) -> std::string
    {
        return tagId == enrouteReleasePointOrBlankTagItemId ? "" : "RLSPT";
    }

    /*
        When a handoff is initiated, send the release to the API
        so that it can be broadcast.
    */
    void EnrouteReleaseEventHandler::HandoffInitiated(
        EuroScopeCFlightPlanInterface& flightplan,
        EuroScopeCControllerInterface& transferringController,
        EuroScopeCControllerInterface& targetController)
    {
        // Only send the release if there is one and the controller transferring is us
        if (!transferringController.IsCurrentUser()) {
            return;
        }

        if (this->outgoingReleases.count(flightplan.GetCallsign()) == 0) {
            return;
        }

        // Send the release to the API so it can be broadcast
        std::string callsign = flightplan.GetCallsign();
        std::string transferringCallsign = transferringController.GetCallsign();
        std::string targetCallsign = targetController.GetCallsign();
        EnrouteRelease release = this->outgoingReleases.at(callsign);
        this->taskRunner.QueueAsynchronousTask([this, release, callsign, transferringCallsign, targetCallsign]() {
            try {
                if (release.releasePoint != this->noReleasePoint) {
                    this->api.SendEnrouteReleaseWithReleasePoint(
                        callsign, transferringCallsign, targetCallsign, release.releaseType, release.releasePoint);
                } else {
                    this->api.SendEnrouteRelease(callsign, transferringCallsign, targetCallsign, release.releaseType);
                }
            } catch (ApiException& exception) {
                LogError("Failed to send enroute release: " + std::string(exception.what()));
            }
        });

        // Set a clear time for us, so it disappears in a while
        this->outgoingReleases.at(callsign).clearTime = std::chrono::system_clock::now() + std::chrono::minutes(3);
    }

    void EnrouteReleaseEventHandler::PluginEventsSynced()
    {
        // Nothing to do here
    }

    auto EnrouteReleaseEventHandler::ReleaseMessageValid(const nlohmann::json& message) const -> bool
    {
        return message.contains("callsign") && message.at("callsign").is_string() && message.contains("type") &&
               message.at("type").is_number_integer() &&
               this->releaseTypes.find(message.at("type").get<int>()) != this->releaseTypes.cend() &&
               message.contains("release_point") &&
               (message.at("release_point").is_null() || message.at("release_point").is_string()) &&
               message.contains("initiating_controller") && message.at("initiating_controller").is_string() &&
               message.contains("target_controller") && message.at("target_controller").is_string();
    }

    /*
        Display the types of release available.
    */
    void EnrouteReleaseEventHandler::DisplayReleaseTypeMenu(
        EuroScopeCFlightPlanInterface& flightplan,
        EuroScopeCRadarTargetInterface& radarTarget,
        const std::string& context,
        const POINT& mousePos)
    {
        // Create the list in place
        RECT popupArea = {
            mousePos.x,
            mousePos.y,
            mousePos.x + 400, // NOLINT
            mousePos.y + 600  // NOLINT
        };

        this->plugin.TriggerPopupList(popupArea, "Release Type", 2);

        PopupMenuItem menuItem;
        menuItem.firstValue = "";
        menuItem.secondValue = "";
        menuItem.callbackFunctionId = this->releaseTypeSelectedCallbackId;
        menuItem.checked = EuroScopePlugIn::POPUP_ELEMENT_NO_CHECKBOX;
        menuItem.disabled = false;
        menuItem.fixedPosition = false;

        // Add each type of release
        for (auto releaseType = this->releaseTypes.cbegin(); releaseType != this->releaseTypes.cend(); ++releaseType) {
            menuItem.firstValue = releaseType->tagString;
            menuItem.secondValue = releaseType->description;
            this->plugin.AddItemToPopupList(menuItem);
        }

        // Add the final item, no release, in a fixed position
        menuItem.firstValue = this->noReleaseItemColumn1;
        menuItem.secondValue = this->noReleaseItemColumn2;
        menuItem.fixedPosition = true;
        this->plugin.AddItemToPopupList(menuItem);
    }

    /*
        Release type has been selected for this aircraft, so select the release type for this aircraft
        The context string is the first column of the release type menu, which is the tag item string.
    */
    void EnrouteReleaseEventHandler::ReleaseTypeSelected(int functionId, const std::string& context, RECT position)
    {
        // Only allow this action if they're tracking the flightplan.
        std::shared_ptr<EuroScopeCFlightPlanInterface> fp = this->plugin.GetSelectedFlightplan();

        if (!fp) {
            LogWarning("Tried to do a release for a non-existant flight");
            return;
        }

        if (!fp->IsTrackedByUser()) {
            LogInfo("Attempted to set release type but flightplan is not tracked by user " + fp->GetCallsign());
            return;
        }

        // If it's none, then delete any release, otherwise set the release
        if (context == this->noReleaseItemColumn1) {
            this->outgoingReleases.erase(fp->GetCallsign());
        } else {
            // Find the release type and set it for the callsign
            auto releaseType = std::find_if(
                this->releaseTypes.cbegin(),
                this->releaseTypes.cend(),
                [context](const EnrouteReleaseType& releaseType) -> bool { return releaseType.tagString == context; });

            this->UpdateOutgoingReleaseType(fp->GetCallsign(), releaseType->id);
        }
    }

    void EnrouteReleaseEventHandler::DisplayReleasePointEditBox(
        EuroScopeCFlightPlanInterface& flightplan,
        EuroScopeCRadarTargetInterface& radarTarget,
        const std::string& context,
        const POINT& mousePos)
    {
        const EnrouteRelease& release = this->GetOutgoingRelease(flightplan.GetCallsign());
        if (release == this->invalidRelease) {
            return;
        }

        this->plugin.ShowTextEditPopup(
            {mousePos.x, mousePos.y, mousePos.x + 80, mousePos.y + 25}, // NOLINT
            this->editReleasePointCallbackId,
            release.releasePoint);
    }

    void EnrouteReleaseEventHandler::EditReleasePoint(int functionId, const std::string& context, RECT position)
    {
        // Only allow this action if they're tracking the flightplan.
        std::shared_ptr<EuroScopeCFlightPlanInterface> fp = this->plugin.GetSelectedFlightplan();

        if (!fp) {
            LogWarning("Tried to do a release for a non-existant flight");
            return;
        }

        if (!fp->IsTrackedByUser()) {
            LogInfo("Attempted to set release type but flightplan is not tracked by user " + fp->GetCallsign());
            return;
        }

        // Only update existing releases here, as release points without a release type are invalid
        if (this->outgoingReleases.count(fp->GetCallsign()) == 0) {
            return;
        }

        this->outgoingReleases.at(fp->GetCallsign()).releasePoint = context.substr(0, 15); // NOLINT
    }

    auto EnrouteReleaseEventHandler::GetReleaseTypeSelectedCallbackId() const -> int
    {
        return this->releaseTypeSelectedCallbackId;
    }

    auto EnrouteReleaseEventHandler::GetEditReleasePointCallbackId() const -> int
    {
        return this->editReleasePointCallbackId;
    }

    auto EnrouteReleaseEventHandler::GetReleaseTypeTagItemId() -> int
    {
        return enrouteReleaseTypeTagItemId;
    }

    auto EnrouteReleaseEventHandler::GetReleasePointTagItemId() -> int
    {
        return enrouteReleasePointTagItemId;
    }

    auto EnrouteReleaseEventHandler::GetReleasePointOrBlankTagItemId() -> int
    {
        return enrouteReleasePointOrBlankTagItemId;
    }

    auto EnrouteReleaseEventHandler::GetInvalidRelease() const -> const UKControllerPlugin::Releases::EnrouteRelease&
    {
        return this->invalidRelease;
    }
} // namespace UKControllerPlugin::Releases
