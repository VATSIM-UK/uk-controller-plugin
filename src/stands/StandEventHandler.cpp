#include "pch/stdafx.h"
#include "stands/StandEventHandler.h"
#include "websocket/WebsocketSubscription.h"

using UKControllerPlugin::Websocket::WebsocketSubscription;

namespace UKControllerPlugin {
    namespace Stands {

        StandEventHandler::StandEventHandler(const std::set<Stand, CompareStands> stands)
            : stands(stands)
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

        int StandEventHandler::GetAssignedStandForCallsign(std::string callsign) const
        {
            return this->standAssignments.count(callsign) ? this->standAssignments.at(callsign) : this->noStandAssigned;
        }

        void StandEventHandler::SetAssignedStand(std::string callsign, int standId)
        {
            this->standAssignments[callsign] = standId;
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

        void StandEventHandler::ProcessWebsocketMessage(const UKControllerPlugin::Websocket::WebsocketMessage& message)
        {
            if (message.event == "App\\Events\\StandAssignedEvent") {
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
                }
            };
        }
    }  // namespace Stands
}  // namespace UKControllerPlugin
