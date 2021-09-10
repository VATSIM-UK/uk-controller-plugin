#include "StandAssignedMessage.h"
#include "StandEventHandler.h"
#include "StandUnassignedMessage.h"
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
using UKControllerPlugin::Push::PushEventSubscription;
using UKControllerPlugin::TaskManager::TaskRunnerInterface;

namespace UKControllerPlugin::Stands {

    StandEventHandler::StandEventHandler(
        const ApiInterface& api,
        TaskRunnerInterface& taskRunner,
        EuroscopePluginLoopbackInterface& plugin,
        Integration::OutboundIntegrationEventHandler& integrationEventHandler,
        std::set<Stand, CompareStands> stands,
        int standSelectedCallbackId)
        : api(api), taskRunner(taskRunner), plugin(plugin), stands(std::move(stands)),
          standSelectedCallbackId(standSelectedCallbackId), integrationEventHandler(integrationEventHandler)
    {
    }

    /*
        Apply the stand annotation to the flightstrip so we can share it with other
        plugins like vSMR
    */
    void StandEventHandler::AnnotateFlightStrip(const std::string& callsign, int standId) const
    {
        // Find the flightplan to apply annotation
        std::shared_ptr<EuroScopeCFlightPlanInterface> fp = this->plugin.GetFlightplanForCallsign(callsign);

        if (!fp) {
            return;
        }

        fp->AnnotateFlightStrip(this->annotationIndex, this->stands.find(standId)->identifier);
    }

    auto StandEventHandler::DoStandAssignment(
        const std::shared_ptr<EuroScopeCFlightPlanInterface>& aircraft,
        const std::string& airfield,
        std::string identifier) -> std::string
    {
        // Only allow this action if they're tracking the flightplan or its untracked
        if (!aircraft) {
            LogWarning("Tried assign a stand for a non-existant aircraft");
            return "Tried assign a stand for a non-existant aircraft";
        }

        if (!this->CanAssignStand(*aircraft)) {
            LogInfo("Attempted to assign stand but flightplan is tracked by someone else " + aircraft->GetCallsign());
            return "Attempted to assign stand but flightplan is tracked by someone else";
        }

        // Uppercase any identifiers
        std::transform(
            identifier.begin(), identifier.end(), identifier.begin(), [](unsigned char c) { return std::toupper(c); });

        auto mapLock = this->LockStandMap();
        std::string callsign = aircraft->GetCallsign();
        // If no stand is selected, delete the current assignment if there is one
        if ((identifier == this->noStandMenuItem || identifier == this->noStandEditBoxItem) &&
            (this->standAssignments.count(callsign) != 0)) {
            this->UnassignStandForAircraft(callsign);
            this->taskRunner.QueueAsynchronousTask([this, callsign]() {
                try {
                    this->api.DeleteStandAssignmentForAircraft(callsign);
                } catch (ApiException&) {
                    LogError("Failed to delete stand assignment for " + callsign);
                }
            });
        } else {
            // Find the requested stand
            auto stand = std::find_if(
                this->stands.cbegin(), this->stands.cend(), [airfield, identifier](const Stand& stand) -> bool {
                    return stand.identifier == identifier && stand.airfieldCode == airfield;
                });

            if (stand == this->stands.cend()) {
                LogInfo("Tried to assign a non-existant stand");
                return "Tried to assign a non-existant stand";
            }

            // Assign that stand
            this->AssignStandToAircraft(callsign, *stand);

            int standId = stand->id;
            this->taskRunner.QueueAsynchronousTask([this, standId, callsign]() {
                try {
                    this->api.AssignStandToAircraft(callsign, standId);
                } catch (ApiException&) {
                    LogError("Failed to create stand assignment for " + callsign);
                }
            });
        }

        return "";
    }

    auto StandEventHandler::CountStandAssignments() const -> size_t
    {
        return this->standAssignments.size();
    }

    void StandEventHandler::DisplayStandSelectionMenu(
        EuroScopeCFlightPlanInterface& flightplan,
        EuroScopeCRadarTargetInterface& radarTarget,
        const std::string& context,
        const POINT& mousePos)
    {
        if (!this->CanAssignStand(flightplan)) {
            return;
        }

        // Pick the airfield to use
        this->lastAirfieldUsed = this->GetAirfieldForStandAssignment(flightplan);

        // Create the list in place
        RECT popupArea = {
            mousePos.x,
            mousePos.y,
            mousePos.x + 400, // NOLINT
            mousePos.y + 600  // NOLINT
        };

        this->plugin.TriggerPopupList(popupArea, "Assign Stand", 2);

        PopupMenuItem menuItem;
        menuItem.firstValue = "";
        menuItem.secondValue = this->lastAirfieldUsed;
        menuItem.callbackFunctionId = this->standSelectedCallbackId;
        menuItem.checked = EuroScopePlugIn::POPUP_ELEMENT_NO_CHECKBOX;
        menuItem.disabled = false;
        menuItem.fixedPosition = false;

        // Add each stand in turn
        for (auto stand = this->stands.cbegin(); stand != this->stands.cend(); ++stand) {
            if (stand->airfieldCode != this->lastAirfieldUsed) {
                continue;
            }

            menuItem.firstValue = stand->identifier;
            this->plugin.AddItemToPopupList(menuItem);
        }

        // Add the final item, no stand, in a fixed position
        menuItem.firstValue = this->noStandMenuItem;
        menuItem.secondValue = "None";
        menuItem.fixedPosition = true;
        this->plugin.AddItemToPopupList(menuItem);
    }

    auto StandEventHandler::GetAssignedStandForCallsign(const std::string& callsign) const -> int
    {
        auto assignment = this->standAssignments.find(callsign);
        return assignment == this->standAssignments.cend() ? this->noStandAssigned : assignment->second;
    }

    auto StandEventHandler::GetLastAirfield() const -> std::string
    {
        return this->lastAirfieldUsed;
    }

    /*
        Remove the flight strip annotation for vSMR
    */
    void StandEventHandler::RemoveFlightStripAnnotation(const std::string& callsign) const
    {
        // Find the flightplan to apply annotation
        std::shared_ptr<EuroScopeCFlightPlanInterface> fp = this->plugin.GetFlightplanForCallsign(callsign);

        if (!fp) {
            return;
        }

        fp->AnnotateFlightStrip(this->annotationIndex, "");
    }

    void StandEventHandler::SetAssignedStand(const std::string& callsign, int standId)
    {
        this->standAssignments[callsign] = standId;
    }

    void StandEventHandler::StandSelected(int functionId, std::string context, RECT mousePosition)
    {
        /*
            EuroScope has a weird bug when using edit boxes where it duplicates up the OnFunctionCall
            event as a result of the box being filled in. By setting this to empty after we are done, we can make sure
            that the same event doesn't get processed twice.
        */
        if (this->lastAirfieldUsed.empty()) {
            return;
        }

        this->DoStandAssignment(this->plugin.GetSelectedFlightplan(), this->lastAirfieldUsed, std::move(context));

        this->lastAirfieldUsed = "";
    }

    void StandEventHandler::DisplayStandAssignmentEditBox(
        EuroScopeCFlightPlanInterface& flightplan,
        EuroScopeCRadarTargetInterface& radarTarget,
        const std::string& context,
        const POINT& mousePos)
    {
        if (!this->CanAssignStand(flightplan)) {
            return;
        }

        // Pick the airfield to use
        this->lastAirfieldUsed = this->GetAirfieldForStandAssignment(flightplan);

        // If a stand is assigned, set the starting text to be the stand identifier
        std::string startingText;
        auto mapLock = this->LockStandMap();
        auto assignedStand = this->standAssignments.find(flightplan.GetCallsign());
        if (assignedStand != this->standAssignments.cend() &&
            this->stands.find(assignedStand->second) != this->stands.cend()) {
            startingText = this->stands.find(assignedStand->second)->identifier;
        }

        // Display the popup
        this->plugin.ShowTextEditPopup(
            {mousePos.x, mousePos.y, mousePos.x + 80, mousePos.y + 25}, // NOLINT
            this->standSelectedCallbackId,
            startingText);
    }

    /*
        Users can only assign a stand if they're a valid VATSIM controller and the flightplan is either
        untracked or tracked by them.
    */
    auto StandEventHandler::CanAssignStand(EuroScopeCFlightPlanInterface& flightplan) const -> bool
    {
        return this->plugin.GetUserControllerObject()->IsVatsimRecognisedController() &&
               (!flightplan.IsTracked() || flightplan.IsTrackedByUser());
    }

    auto StandEventHandler::GetTagItemDescription(int tagItemId) const -> std::string
    {
        return "Assigned Stand";
    }

    /*
        Set the text of the tag item to be the identifier for whichever stand is assigned
    */
    void StandEventHandler::SetTagItemData(UKControllerPlugin::Tag::TagData& tagData)
    {
        auto mapLock = this->LockStandMap();
        if (this->standAssignments.count(tagData.flightPlan.GetCallsign()) != 0) {
            auto stand = this->stands.find(this->standAssignments.at(tagData.flightPlan.GetCallsign()));
            if (stand == this->stands.cend()) {
                return;
            }

            tagData.SetItemString(stand->identifier);
        }
    }

    auto StandEventHandler::AssignmentMessageValid(const nlohmann::json& message) const -> bool
    {
        return message.is_object() && message.contains("callsign") && message.at("callsign").is_string() &&
               message.contains("stand_id") && message.at("stand_id").is_number_integer() &&
               this->stands.find(message.at("stand_id").get<int>()) != this->stands.cend();
    }

    auto StandEventHandler::UnassignmentMessageValid(const nlohmann::json& message) -> bool
    {
        return message.is_object() && message.contains("callsign") && message.at("callsign").is_string();
    }

    /*
        Get the airfield to use for stand assignment purposes, depending on how far the aircraft is from
        its origin.
    */
    auto StandEventHandler::GetAirfieldForStandAssignment(EuroScopeCFlightPlanInterface& flightplan) const
        -> std::string
    {
        return flightplan.GetDistanceFromOrigin() < this->maxDistanceFromDepartureAirport ? flightplan.GetOrigin()
                                                                                          : flightplan.GetDestination();
    }

    /*
        Process message to assign stands
    */
    auto StandEventHandler::ProcessMessage(std::string message) -> bool
    {
        std::vector<std::string> parts;
        std::stringstream ss(message);
        std::string temp;

        if (message.substr(0, 6) != "STANDS") { // NOLINT
            return false;
        }

        while (std::getline(ss, temp, ':')) {
            parts.push_back(temp);
        }

        if (parts.size() != 4) {
            return false;
        }

        this->DoStandAssignment(this->plugin.GetFlightplanForCallsign(parts[1]), parts[2], parts[3]);

        return true;
    }

    /*
        Every time the flightplan updates, keep the annotation up to date if it's
        not so. This is needed because there wont be any flightplans to annotate
        when the plugin first starts up, so it annotates them as they come along.
    */
    void StandEventHandler::FlightPlanEvent(
        EuroScopeCFlightPlanInterface& flightPlan, EuroScopeCRadarTargetInterface& radarTarget)
    {
        auto mapLock = this->LockStandMap();
        if (this->standAssignments.count(flightPlan.GetCallsign()) == 0) {
            return;
        }

        if (this->stands.find(this->standAssignments.at(flightPlan.GetCallsign()))->identifier ==
            flightPlan.GetAnnotation(this->annotationIndex)) {
            return;
        }

        flightPlan.AnnotateFlightStrip(
            this->annotationIndex, this->stands.find(this->standAssignments.at(flightPlan.GetCallsign()))->identifier);
    }

    void StandEventHandler::FlightPlanDisconnectEvent(EuroScopeCFlightPlanInterface& flightPlan)
    {
        // Nothing to do
    }

    void StandEventHandler::ControllerFlightPlanDataEvent(EuroScopeCFlightPlanInterface& flightPlan, int dataType)
    {
        // Nothing to do
    }

    void StandEventHandler::PluginEventsSynced()
    {
        this->taskRunner.QueueAsynchronousTask([this]() {
            try {
                nlohmann::json standAssignments = this->api.GetAssignedStands();

                if (!standAssignments.is_array()) {
                    LogWarning("Invalid stand assignment data");
                    return;
                }

                // Delete all existing assignments
                auto mapLock = this->LockStandMap();
                this->standAssignments.clear();

                for (auto assignment = standAssignments.cbegin(); assignment != standAssignments.cend(); ++assignment) {
                    if (!AssignmentMessageValid(*assignment)) {
                        LogWarning("Invalid stand assignment message on mass assignment " + assignment->dump());
                        continue;
                    }

                    this->AssignStandToAircraft(
                        assignment->at("callsign").get<std::string>(),
                        *this->stands.find(assignment->at("stand_id").get<int>()));
                }
                LogInfo("Loaded " + std::to_string(this->standAssignments.size()) + " stand assignments");
            } catch (ApiException&) {
                LogError("Unable to load stand assignment data");
            }
        });
    }

    /*
        Process messages from the websocket.
    */
    void StandEventHandler::ProcessPushEvent(const Push::PushEvent& message)
    {
        auto mapLock = this->LockStandMap();
        if (message.event == "App\\Events\\StandAssignedEvent") {
            // If a stand has been assigned, assign it here
            if (!AssignmentMessageValid(message.data)) {
                LogWarning("Invalid stand assignment message " + message.data.dump());
                return;
            }

            this->AssignStandToAircraft(
                message.data.at("callsign").get<std::string>(),
                *this->stands.find(message.data.at("stand_id").get<int>()));
        } else if (message.event == "App\\Events\\StandUnassignedEvent") {
            // If a stand has been unassigned, unassign it here
            if (!UnassignmentMessageValid(message.data)) {
                LogWarning("Invalid stand unassignment message " + message.data.dump());
                return;
            }

            this->UnassignStandForAircraft(message.data.at("callsign").get<std::string>());
        }
    }

    auto StandEventHandler::GetPushEventSubscriptions() const -> std::set<PushEventSubscription>
    {
        return {{PushEventSubscription::SUB_TYPE_CHANNEL, "private-stand-assignments"}};
    }

    auto StandEventHandler::LockStandMap() -> std::lock_guard<std::mutex>
    {
        return std::lock_guard(this->mapMutex);
    }

    void StandEventHandler::UnassignStandForAircraft(const std::string& callsign)
    {
        this->RemoveFlightStripAnnotation(callsign);
        this->standAssignments.erase(callsign);
        this->integrationEventHandler.SendEvent(std::make_shared<StandUnassignedMessage>(callsign));
        LogInfo("Stand assignment removed for " + callsign);
    }

    void StandEventHandler::AssignStandToAircraft(const std::string& callsign, const Stand& stand)
    {
        this->AnnotateFlightStrip(callsign, stand.id);
        this->standAssignments[callsign] = stand.id;
        this->integrationEventHandler.SendEvent(
            std::make_shared<StandAssignedMessage>(callsign, stand.airfieldCode, stand.identifier));
        LogInfo(
            "Stand id " + std::to_string(stand.id) + "(" + stand.airfieldCode + "/" + stand.identifier +
            ") "
            "assigned to " +
            callsign);
    }
    auto StandEventHandler::ActionsToProcess() const -> std::vector<Integration::MessageType>
    {
        return {{"assign_stand", 1}, {"unassign_stand", 1}};
    }

    void StandEventHandler::ProcessAction(
        std::shared_ptr<Integration::MessageInterface> message,
        std::function<void(void)> success,
        std::function<void(std::vector<std::string>)> fail)
    {
        auto messageData = message->GetMessageData();
        if (message->GetMessageType().type == "assign_stand") {
            if (!messageData.contains("callsign") || !messageData.at("callsign").is_string()) {
                fail({"Invalid callsign in message data"});
                return;
            }

            if (!messageData.contains("airfield") || !messageData.at("airfield").is_string()) {
                fail({"Invalid airfield field in message data"});
                return;
            }

            if (!messageData.contains("stand") || !messageData.at("stand").is_string()) {
                fail({"Invalid stand field in message data"});
                return;
            }

            auto errorMessage = this->DoStandAssignment(
                this->plugin.GetFlightplanForCallsign(messageData.at("callsign").get<std::string>()),
                messageData.at("airfield").get<std::string>(),
                messageData.at("stand").get<std::string>());

            if (errorMessage.empty()) {
                success();
            } else {
                fail({errorMessage});
            }
        } else if (message->GetMessageType().type == "unassign_stand") {
            if (!messageData.contains("callsign") || !messageData.at("callsign").is_string()) {
                fail({"Invalid callsign in message data"});
                return;
            }

            auto standsGuard = this->LockStandMap();
            auto callsign = messageData.at("callsign").get<std::string>();
            auto assignedStand = this->GetAssignedStandForCallsign(callsign);

            if (assignedStand != this->noStandAssigned) {
                this->UnassignStandForAircraft(callsign);
                this->taskRunner.QueueAsynchronousTask([this, callsign]() {
                    try {
                        this->api.DeleteStandAssignmentForAircraft(callsign);
                    } catch (ApiException&) {
                        LogError("Failed to delete stand assignment for " + callsign);
                    }
                });
            }

            success();
        }
    }
} // namespace UKControllerPlugin::Stands
