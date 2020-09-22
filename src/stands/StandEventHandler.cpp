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

        void StandEventHandler::SetAssignedStand(std::string callsign, int standId)
        {
            this->standAssignments[callsign] = standId;
        }

        std::string StandEventHandler::GetTagItemDescription(int tagItemId) const
        {
            return "Assigned Stand";
        }

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

        void StandEventHandler::ProcessWebsocketMessage(const UKControllerPlugin::Websocket::WebsocketMessage& message)
        {
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
