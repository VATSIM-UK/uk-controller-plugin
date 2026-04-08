#include "StandAssignedMessage.h"
#include "StandColourConfiguration.h"
#include "StandEventHandler.h"
#include "StandUnassignedMessage.h"
#include <format>
#include "api/ApiException.h"
#include "api/ApiInterface.h"
#include "api/ApiRequestFactory.h"
#include "api/ApiRequestException.h"
#include "euroscope/EuroScopeCControllerInterface.h"
#include "euroscope/EuroScopeCFlightPlanInterface.h"
#include "euroscope/EuroScopeCRadarTargetInterface.h"
#include "euroscope/EuroscopePluginLoopbackInterface.h"
#include "ownership/AirfieldServiceProviderCollection.h"
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
        UKControllerPlugin::Euroscope::UserSetting* userSetting,
        Integration::OutboundIntegrationEventHandler& integrationEventHandler,
        std::shared_ptr<Ownership::AirfieldServiceProviderCollection> ownership,
        std::set<Stand, CompareStands> stands,
        int standSelectedCallbackId)
        : api(api), taskRunner(taskRunner), plugin(plugin), stands(std::move(stands)),
          colourConfiguration(
              userSetting ? std::make_shared<StandColourConfiguration>(*userSetting)
                          : std::make_shared<StandColourConfiguration>()),
          integrationEventHandler(integrationEventHandler), ownership(ownership),
          standSelectedCallbackId(standSelectedCallbackId)
    {
        assert(this->ownership != nullptr && "Ownership must not be null");
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
            if (identifier != this->autoStandMenuItem) {
                return this->AssignStandInApi(callsign, airfield, identifier);
            }

            this->RequestStandFromApi(*aircraft);
        }

        return "";
    }

    auto StandEventHandler::AssignStandInApi(
        const std::string& callsign, const std::string& airfield, const std::string& identifier) -> std::string
    {
        using enum StandAssignment::Source;

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
        this->AssignStandToAircraft(callsign, *stand, User);

        int standId = stand->id;
        auto callsignForRequest = callsign;
        this->taskRunner.QueueAsynchronousTask([this, standId, callsignForRequest]() {
            try {
                this->api.AssignStandToAircraft(callsignForRequest, standId);
            } catch (ApiException&) {
                LogError("Failed to create stand assignment for " + callsignForRequest);
            }
        });

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

        // Add the penultimate item, "AUTO" in a fixed position
        menuItem.firstValue = this->autoStandMenuItem;
        menuItem.secondValue = "Automatic";
        menuItem.fixedPosition = true;
        this->plugin.AddItemToPopupList(menuItem);

        // Add the final item, no stand, in a fixed position
        menuItem.firstValue = this->noStandMenuItem;
        menuItem.secondValue = "None";
        menuItem.fixedPosition = true;
        this->plugin.AddItemToPopupList(menuItem);
    }

    auto StandEventHandler::GetAssignedStandForCallsign(const std::string& callsign) const -> int
    {
        auto assignment = this->standAssignments.find(callsign);
        return assignment == this->standAssignments.cend() ? noStandAssigned : assignment->second.standId;
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
        this->standAssignments[callsign] = {standId, StandAssignment::Source::SystemAuto};
    }

    void StandEventHandler::SetAssignedStand(const std::string& callsign, int standId, const std::string& source)
    {
        this->SetAssignedStand(callsign, standId, StandAssignment::FromString(source));
    }

    void StandEventHandler::SetAssignedStand(const std::string& callsign, int standId, StandAssignment::Source source)
    {
        this->standAssignments[callsign] = {standId, source};
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
            this->stands.find(assignedStand->second.standId) != this->stands.cend()) {
            startingText = this->stands.find(assignedStand->second.standId)->identifier;
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
        if (tagItemId == assignedStandTagItemId) {
            return "Assigned Stand";
        }
        if (tagItemId == standAssignmentSourceTagItemId) {
            return "Stand Assignment Source";
        }
        return "";
    }

    /*
        Set the text of the tag item to be the identifier for whichever stand is assigned
    */
    void StandEventHandler::SetTagItemData(UKControllerPlugin::Tag::TagData& tagData)
    {
        auto mapLock = this->LockStandMap();
        const auto& callsign = tagData.GetFlightplan().GetCallsign();
        if (!this->standAssignments.contains(callsign)) {
            return;
        }

        const auto& assignment = this->standAssignments.at(callsign);
        auto stand = this->stands.find(assignment.standId);
        if (stand == this->stands.cend()) {
            return;
        }

        if (tagData.GetItemCode() == assignedStandTagItemId) {
            tagData.SetItemString(stand->identifier);
        } else if (tagData.GetItemCode() == standAssignmentSourceTagItemId) {
            tagData.SetItemString(GetAssignmentSourceShorthand(assignment.source));
        }

        if (tagData.GetItemCode() != assignedStandTagItemId &&
            tagData.GetItemCode() != standAssignmentSourceTagItemId) {
            return;
        }
        tagData.SetTagColour(this->colourConfiguration->GetColourForSource(assignment.source));
    }

    auto StandEventHandler::AssignmentMessageValid(const nlohmann::json& message) const -> bool
    {
        return message.is_object() && message.contains("callsign") && message.at("callsign").is_string() &&
               message.contains("stand_id") && message.at("stand_id").is_number_integer() &&
               this->stands.find(message.at("stand_id").get<int>()) != this->stands.cend();
    }

    auto StandEventHandler::GetAssignmentSourceFromMessage(const nlohmann::json& message) -> StandAssignment::Source
    {
        using enum StandAssignment::Source;

        if (message.contains("assignment_source") && message.at("assignment_source").is_string()) {
            return StandAssignment::FromString(message.at("assignment_source").get<std::string>());
        }

        return Unknown;
    }

    auto StandEventHandler::GetAssignmentSourceShorthand(StandAssignment::Source source) -> std::string
    {
        using enum StandAssignment::Source;

        switch (source) {
        case Unknown:
            return "UNK ";
        case User:
            return "USER";
        case ReservationAllocator:
            return "RES ";
        case VaaAllocator:
            return "VAA ";
        case SystemAuto:
            return "AUTO";
        }
        return "UNK ";
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
        if (!this->standAssignments.contains(flightPlan.GetCallsign())) {
            return;
        }

        const auto& assignment = this->standAssignments.at(flightPlan.GetCallsign());
        const auto& stand = this->stands.find(assignment.standId);
        if (stand == this->stands.cend()) {
            LogWarning(
                std::format("Assigned stand id {} not found for {}", assignment.standId, flightPlan.GetCallsign()));
            return;
        }

        if (stand->identifier == flightPlan.GetAnnotation(this->annotationIndex)) {
            return;
        }

        flightPlan.AnnotateFlightStrip(this->annotationIndex, stand->identifier);
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
                        *this->stands.find(assignment->at("stand_id").get<int>()),
                        GetAssignmentSourceFromMessage(*assignment));
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
                *this->stands.find(message.data.at("stand_id").get<int>()),
                GetAssignmentSourceFromMessage(message.data));
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

    auto StandEventHandler::LockStandMap() -> std::lock_guard<std::recursive_mutex>
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
        using enum StandAssignment::Source;

        this->AssignStandToAircraft(callsign, stand, SystemAuto);
    }

    void StandEventHandler::AssignStandToAircraft(
        const std::string& callsign, const Stand& stand, StandAssignment::Source source)
    {
        this->AnnotateFlightStrip(callsign, stand.id);
        this->standAssignments[callsign] = {stand.id, source};
        this->integrationEventHandler.SendEvent(
            std::make_shared<StandAssignedMessage>(callsign, stand.airfieldCode, stand.identifier));
        const auto logMessage = std::format(
            "Stand id {} ({}/{}) assigned to {} (source: {})",
            stand.id,
            stand.airfieldCode,
            stand.identifier,
            callsign,
            StandAssignment::ToString(source));
        LogInfo(logMessage);
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

            if (assignedStand != noStandAssigned) {
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

    void StandEventHandler::RequestStandFromApi(const Euroscope::EuroScopeCFlightPlanInterface& flightplan)
    {
        // If EuroScope wont tell us where they are, do nothing.
        if (flightplan.GetDistanceFromOrigin() == 0.0) {
            return;
        }

        // If they're close to departure and we're providing delivery, assign a stand
        if (flightplan.GetDistanceFromOrigin() < this->maxDistanceForDepartureStands &&
            this->ownership->DeliveryControlProvidedByUser(flightplan.GetOrigin())) {
            this->RequestDepartureStandFromApi(flightplan);
        }

        // Otherwise, assume they're close to arrival
        if (!this->ownership->DeliveryControlProvidedByUser(flightplan.GetDestination())) {
            return;
        }

        this->RequestArrivalStandFromApi(flightplan);
    }
    void StandEventHandler::RequestDepartureStandFromApi(const Euroscope::EuroScopeCFlightPlanInterface& flightplan)
    {
        const auto radarTarget = this->plugin.GetRadarTargetForCallsign(flightplan.GetCallsign());
        if (!radarTarget) {
            return;
        }

        this->DoApiStandRequest(
            flightplan.GetCallsign(),
            {
                {"callsign", flightplan.GetCallsign()},
                {"departure_airfield", flightplan.GetOrigin()},
                {"assignment_type", "departure"},
                {"latitude", radarTarget->GetPosition().m_Latitude},
                {"longitude", radarTarget->GetPosition().m_Longitude},
            });
    }

    void StandEventHandler::RequestArrivalStandFromApi(const Euroscope::EuroScopeCFlightPlanInterface& flightplan)
    {
        this->DoApiStandRequest(
            flightplan.GetCallsign(),
            {{"callsign", flightplan.GetCallsign()},
             {"departure_airfield", flightplan.GetOrigin()},
             {"arrival_airfield", flightplan.GetDestination()},
             {"assignment_type", "arrival"},
             {"aircraft_type", flightplan.GetAircraftType()}});
    }

    void StandEventHandler::DoApiStandRequest(const std::string& callsign, const nlohmann::json data)
    {
        LogDebug("Requesting stand assignment from API: " + data.dump());
        const std::string requestCallsign = callsign;
        ApiRequest()
            .Post("stand/assignment/requestauto", data)
            .Then([this, requestCallsign](const UKControllerPluginUtils::Api::Response& response) {
                auto lock = this->LockStandMap();

                const auto& data = response.Data();
                if (!data.contains("stand_id") || !data.at("stand_id").is_number_integer()) {
                    LogWarning("Invalid stand assignment response " + data.dump());
                    return;
                }

                const auto standId = data.at("stand_id").get<int>();
                if (!this->stands.contains(standId)) {
                    LogWarning("Invalid stand assignment response, bad id " + data.dump());
                    return;
                }

                const auto& stand = this->stands.find(standId);
                LogInfo("API generated stand assignment " + std::to_string(standId) + " for " + requestCallsign);

                this->AssignStandToAircraft(requestCallsign, *stand);
            })
            .Catch([requestCallsign](const UKControllerPluginUtils::Api::ApiRequestException& exception) {
                LogError("Failed to request stand assignment for " + requestCallsign + ": " + exception.what());
            });
    }

} // namespace UKControllerPlugin::Stands
