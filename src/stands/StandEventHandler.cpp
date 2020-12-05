#include "pch/stdafx.h"
#include "stands/StandEventHandler.h"
#include "websocket/WebsocketSubscription.h"
#include "api/ApiException.h"
#include "plugin/PopupMenuItem.h"
#include "euroscope/EuroScopeCControllerInterface.h"

using UKControllerPlugin::Websocket::WebsocketSubscription;
using UKControllerPlugin::Api::ApiInterface;
using UKControllerPlugin::Api::ApiException;
using UKControllerPlugin::Plugin::PopupMenuItem;
using UKControllerPlugin::TaskManager::TaskRunnerInterface;
using UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface;
using UKControllerPlugin::Euroscope::EuroScopeCControllerInterface;
using UKControllerPlugin::Euroscope::EuroScopeCRadarTargetInterface;
using UKControllerPlugin::Euroscope::EuroscopePluginLoopbackInterface;

namespace UKControllerPlugin {
    namespace Stands {

        StandEventHandler::StandEventHandler(
            const ApiInterface& api,
            TaskRunnerInterface& taskRunner,
            EuroscopePluginLoopbackInterface& plugin,
            const std::set<Stand, CompareStands> stands,
            int standSelectedCallbackId
        )
            : api(api), taskRunner(taskRunner), plugin(plugin), stands(stands),
            standSelectedCallbackId(standSelectedCallbackId)
        {
        }

        /*
            Apply the stand annotation to the flightstrip so we can share it with other
            plugins like vSMR
        */
        void StandEventHandler::AnnotateFlightStrip(std::string callsign, int standId) const
        {
            // Find the flightplan to apply annotation
            std::shared_ptr<EuroScopeCFlightPlanInterface> fp = this->plugin.GetFlightplanForCallsign(callsign);

            if (!fp) {
                return;
            }

            fp->AnnotateFlightStrip(
                this->annotationIndex,
                this->stands.find(standId)->identifier
            );
        }

        void StandEventHandler::DoStandAssignment(
            std::shared_ptr<EuroScopeCFlightPlanInterface> aircraft,
            std::string airfield,
            std::string identifier
        ) {
            // Only allow this action if they're tracking the flightplan or its untracked
            if (!aircraft) {
                LogWarning("Tried assign a stand for a non-existant aircraft");
                return;
            }

            if (!this->CanAssignStand(*aircraft)) {
                LogInfo("Attempted to assign stand but flightplan is tracked by someone else " + aircraft->GetCallsign());
                return;
            }

            std::string callsign = aircraft->GetCallsign();
            // If no stand is selected, delete the current assignment if there is one
            if (
                (identifier == this->noStandMenuItem || identifier == this->noStandEditBoxItem) &&
                this->standAssignments.count(callsign)
            ) {
                this->standAssignments.erase(callsign);
                this->RemoveFlightStripAnnotation(callsign);
                this->taskRunner.QueueAsynchronousTask([this, callsign]() {
                    try {
                        this->api.DeleteStandAssignmentForAircraft(callsign);
                    }
                    catch (ApiException) {
                        LogError("Failed to delete stand assignment for " + callsign);
                    }
                });
            } else {
             // Find the requested stand
                auto stand = std::find_if(
                    this->stands.cbegin(),
                    this->stands.cend(),
                    [airfield, identifier](const Stand& stand) -> bool {
                        return stand.identifier == identifier && stand.airfieldCode == airfield;
                    }
                );

                if (stand == this->stands.cend()) {
                    LogInfo("Tried to assign a non-existant stand");
                    return;
                }

                // Assign that stand
                int standId = stand->id;
                this->standAssignments[callsign] = standId;
                this->AnnotateFlightStrip(callsign, standId);
                this->taskRunner.QueueAsynchronousTask([this, standId, callsign]() {
                    try {
                        this->api.AssignStandToAircraft(callsign, standId);
                    }
                    catch (ApiException) {
                        LogError("Failed to create stand assignment for " + callsign);
                    }
                });
            }
        }

        size_t StandEventHandler::CountStands(void) const
        {
            return this->stands.size();
        }

        size_t StandEventHandler::CountStandAssignments(void) const
        {
            return this->standAssignments.size();
        }

        void StandEventHandler::DisplayStandSelectionMenu(
            EuroScopeCFlightPlanInterface& flightplan,
            EuroScopeCRadarTargetInterface& radarTarget,
            std::string context,
            const POINT& mousePos
        ) {
            if (!this->CanAssignStand(flightplan)) {
                return;
            }

            // Pick the airfield to use
            this->lastAirfieldUsed = this->GetAirfieldForStandAssignment(flightplan);

            // Create the list in place
            RECT popupArea = {
                mousePos.x,
                mousePos.y,
                mousePos.x + 400,
                mousePos.y + 600
            };


            this->plugin.TriggerPopupList(
                popupArea,
                "Assign Stand",
                2
            );

            PopupMenuItem menuItem;
            menuItem.firstValue = "";
            menuItem.secondValue = this->lastAirfieldUsed;
            menuItem.callbackFunctionId = this->standSelectedCallbackId;
            menuItem.checked = EuroScopePlugIn::POPUP_ELEMENT_NO_CHECKBOX;
            menuItem.disabled = false;
            menuItem.fixedPosition = false;

            // Add each stand in turn
            for (
                auto stand = this->stands.cbegin();
                stand != this->stands.cend();
                ++stand
            ) {
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

        int StandEventHandler::GetAssignedStandForCallsign(std::string callsign) const
        {
            return this->standAssignments.count(callsign) ? this->standAssignments.at(callsign) : this->noStandAssigned;
        }

        std::string StandEventHandler::GetLastAirfield(void) const
        {
            return this->lastAirfieldUsed;
        }

        /*
            Remove the flight strip annotation for vSMR
        */
        void StandEventHandler::RemoveFlightStripAnnotation(std::string callsign) const
        {
            // Find the flightplan to apply annotation
            std::shared_ptr<EuroScopeCFlightPlanInterface> fp = this->plugin.GetFlightplanForCallsign(callsign);

            if (!fp) {
                return;
            }

            fp->AnnotateFlightStrip(
                this->annotationIndex,
                ""
            );
        }

        void StandEventHandler::SetAssignedStand(std::string callsign, int standId)
        {
            this->standAssignments[callsign] = standId;
        }

        void StandEventHandler::StandSelected(int functionId, std::string context, RECT)
        {

            /*
                EuroScope has a weird bug when using edit boxes where it duplicates up the OnFunctionCall
                event as a result of the box being filled in. By setting this to empty after we are done, we can make sure
                that the same event doesn't get processed twice.
            */
            if (this->lastAirfieldUsed == "") {
                return;
            }


            this->DoStandAssignment(
                this->plugin.GetSelectedFlightplan(),
                this->lastAirfieldUsed,
                context
            );

            this->lastAirfieldUsed = "";
        }

        void StandEventHandler::DisplayStandAssignmentEditBox(
            EuroScopeCFlightPlanInterface& flightplan,
            EuroScopeCRadarTargetInterface& radarTarget,
            std::string context,
            const POINT& mousePos
        ) {
            if (!this->CanAssignStand(flightplan)) {
                return;
            }

            // Pick the airfield to use
            this->lastAirfieldUsed = this->GetAirfieldForStandAssignment(flightplan);

            // If a stand is assigned, set the starting text to be the stand identifier
            std::string startingText = "";
            auto assignedStand = this->standAssignments.find(flightplan.GetCallsign());
            if (
                assignedStand != this->standAssignments.cend() &&
                this->stands.find(assignedStand->second) != this->stands.cend()
            ) {
                startingText = this->stands.find(assignedStand->second)->identifier;
            }

            // Display the popup
            this->plugin.ShowTextEditPopup(
                { mousePos.x, mousePos.y, mousePos.x + 80, mousePos.y + 25 },
                this->standSelectedCallbackId,
                startingText
            );
        }

        /*
            Users can only assign a stand if they're a valid VATSIM controller and the flightplan is either
            untracked or tracked by them.
        */
        bool StandEventHandler::CanAssignStand(EuroScopeCFlightPlanInterface& flightplan) const
        {
            return this->plugin.GetUserControllerObject()->IsVatsimRecognisedController() &&
                (!flightplan.IsTracked() || flightplan.IsTrackedByUser());
        }

        std::string StandEventHandler::GetTagItemDescription(int tagItemId) const
        {
            return "Assigned Stand";
        }

        /*
            Set the text of the tag item to be the identifier for whichever stand is assigned
        */
        void StandEventHandler::SetTagItemData(UKControllerPlugin::Tag::TagData& tagData)
        {
            if (
                this->standAssignments.count(tagData.flightPlan.GetCallsign())
            ) {
                auto stand = this->stands.find(this->standAssignments.at(tagData.flightPlan.GetCallsign()));
                if (stand == this->stands.cend()) {
                    return;
                }

                tagData.SetItemString(stand->identifier);
            }
        }

        bool StandEventHandler::AssignmentMessageValid(const nlohmann::json& message) const
        {
            return message.is_object() &&
                message.contains("callsign") &&
                message.at("callsign").is_string() &&
                message.contains("stand_id") &&
                message.at("stand_id").is_number_integer() &&
                this->stands.find(message.at("stand_id").get<int>()) != this->stands.cend();
        }

        bool StandEventHandler::UnassignmentMessageValid(const nlohmann::json& message) const
        {
            return message.is_object() &&
                message.contains("callsign") &&
                message.at("callsign").is_string();
        }

        /*
            Get the airfield to use for stand assignment purposes, depending on how far the aircraft is from
            its origin.
        */
        std::string StandEventHandler::GetAirfieldForStandAssignment(EuroScopeCFlightPlanInterface& flightplan)
        {
            return flightplan.GetDistanceFromOrigin() < this->maxDistanceFromDepartureAirport
                ? flightplan.GetOrigin()
                : flightplan.GetDestination();
        }

        /*
            Process message to assign stands
        */
        bool StandEventHandler::ProcessMessage(std::string message)
        {
            std::vector<std::string> parts;
            std::stringstream ss(message);
            std::string temp;

            if (message.substr(0, 6) != "STANDS") {
                return false;
            }

            while (std::getline(ss, temp, ':')) {
                parts.push_back(temp);
            }

            if (parts.size() != 4) {
                return false;
            }

            this->DoStandAssignment(
                this->plugin.GetFlightplanForCallsign(parts[1]),
                parts[2],
                parts[3]
            );
        }

        /*
            Every time the flightplan updates, keep the annotation up to date if it's
            not so. This is needed because there wont be any flightplans to annotate
            when the plugin first starts up, so it annotates them as they come along.
        */
        void StandEventHandler::FlightPlanEvent(
            EuroScopeCFlightPlanInterface& flightPlan,
            EuroScopeCRadarTargetInterface& radarTarget
        ) {
            if (!this->standAssignments.count(flightPlan.GetCallsign())) {
                return;
            }

            if (
                this->stands.find(this->standAssignments.at(flightPlan.GetCallsign()))->identifier ==
                    flightPlan.GetAnnotation(this->annotationIndex)
            ) {
                return;
            }

            flightPlan.AnnotateFlightStrip(
                this->annotationIndex,
                this->stands.find(this->standAssignments.at(flightPlan.GetCallsign()))->identifier
            );
        }

        void StandEventHandler::FlightPlanDisconnectEvent(EuroScopeCFlightPlanInterface& flightPlan)
        {
            // Nothing to do
        }

        void StandEventHandler::ControllerFlightPlanDataEvent(EuroScopeCFlightPlanInterface& flightPlan, int dataType)
        {
            // Nothing to do
        }

        /*
            Process messages from the websocket.
        */
        void StandEventHandler::ProcessWebsocketMessage(const UKControllerPlugin::Websocket::WebsocketMessage& message)
        {
            if (message.event == "pusher:connection_established") {
                // On connection to the websocket, download all the live stand assignments
                this->taskRunner.QueueAsynchronousTask([this]() {
                    try {
                        nlohmann::json standAssignments = this->api.GetAssignedStands();

                        if (!standAssignments.is_array()) {
                            LogWarning("Invalid stand assignment data");
                            return;
                        }

                        // Delete all existing assignments
                        this->standAssignments.clear();

                        for (
                            auto assignment = standAssignments.cbegin();
                            assignment != standAssignments.cend();
                            ++assignment
                        ) {
                            if (!this->AssignmentMessageValid(*assignment)) {
                                LogWarning("Invalid stand assignment message on mass assignment " + assignment->dump());
                                continue;
                            }

                            this->AnnotateFlightStrip(
                                assignment->at("callsign").get<std::string>(),
                                assignment->at("stand_id").get<int>()
                            );
                            this->standAssignments[assignment->at("callsign").get<std::string>()] =
                                assignment->at("stand_id").get<int>();
                        }
                        LogInfo("Loaded " + std::to_string(this->standAssignments.size()) + " stand assignments");
                    } catch (ApiException e) {
                        LogError("Unable to load stand assignment data");
                    }
                });
            } else if (message.event == "App\\Events\\StandAssignedEvent") {
                // If a stand has been assigned, assign it here
                if (!AssignmentMessageValid(message.data)) {
                    LogWarning("Invalid stand assignment message " + message.data.dump());
                    return;
                }

                this->AnnotateFlightStrip(
                    message.data.at("callsign").get<std::string>(),
                    message.data.at("stand_id").get<int>()
                );
                this->standAssignments[message.data.at("callsign").get<std::string>()] =
                    message.data.at("stand_id").get<int>();
                LogInfo(
                    "Stand id " + std::to_string(message.data.at("stand_id").get<int>()) +
                        " assigned to " + message.data.at("callsign").get<std::string>()
                );
            } else if (message.event == "App\\Events\\StandUnassignedEvent") {
                // If a stand has been unassigned, unassign it here
                if (!UnassignmentMessageValid(message.data)) {
                    LogWarning("Invalid stand unassignment message " + message.data.dump());
                    return;
                }

                this->RemoveFlightStripAnnotation(
                    message.data.at("callsign").get<std::string>()
                );
                this->standAssignments.erase(message.data.at("callsign").get<std::string>());
                LogInfo("Stand assignment removed for " + message.data.at("callsign").get<std::string>());
            }
        }

        std::set<UKControllerPlugin::Websocket::WebsocketSubscription> StandEventHandler::GetSubscriptions(void) const
        {
            return {
                {
                    WebsocketSubscription::SUB_TYPE_CHANNEL,
                    "private-stand-assignments"
                },
                {
                    WebsocketSubscription::SUB_TYPE_EVENT,
                    "pusher:connection_established"
                }
            };
        }
    }  // namespace Stands
}  // namespace UKControllerPlugin
