#include "pch/stdafx.h"
#include "stands/StandEventHandler.h"
#include "websocket/WebsocketSubscription.h"
#include "api/ApiException.h"

using UKControllerPlugin::Websocket::WebsocketSubscription;
using UKControllerPlugin::Api::ApiInterface;
using UKControllerPlugin::Api::ApiException;
using UKControllerPlugin::TaskManager::TaskRunnerInterface;
using UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface;
using UKControllerPlugin::Euroscope::EuroScopeCRadarTargetInterface;

namespace UKControllerPlugin {
    namespace Stands {

        StandEventHandler::StandEventHandler(
            const ApiInterface& api,
            TaskRunnerInterface& taskRunner,
            const std::set<Stand, CompareStands> stands,
            int standSelectedCallbackId
        )
            : api(api), taskRunner(taskRunner), stands(stands), standSelectedCallbackId(standSelectedCallbackId)
        {
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

        }

        int StandEventHandler::GetAssignedStandForCallsign(std::string callsign) const
        {
            return this->standAssignments.count(callsign) ? this->standAssignments.at(callsign) : this->noStandAssigned;
        }

        void StandEventHandler::SetAssignedStand(std::string callsign, int standId)
        {
            this->standAssignments[callsign] = standId;
        }

        void StandEventHandler::StandSelected(int functionId, std::string context, RECT)
        {

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

                        for (auto assignment = standAssignments.cbegin(); assignment != standAssignments.cend(); ++assignment) {
                            if (!this->AssignmentMessageValid(*assignment)) {
                                LogWarning("Invalid stand assignment message on mass assignment " + assignment->dump());
                                continue;
                            }

                            this->standAssignments[assignment->at("callsign").get<std::string>()] = assignment->at("stand_id").get<int>();
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

                this->standAssignments[message.data.at("callsign").get<std::string>()] = message.data.at("stand_id").get<int>();
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
