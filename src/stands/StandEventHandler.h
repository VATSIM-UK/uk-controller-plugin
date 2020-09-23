#pragma once
#include "stands/Stand.h"
#include "stands/CompareStands.h"
#include "tag/TagItemInterface.h"
#include "websocket/WebsocketEventProcessorInterface.h"

namespace UKControllerPlugin {
    namespace Stands {
        /*
            A new class
        */
        class StandEventHandler: public UKControllerPlugin::Tag::TagItemInterface,
            public UKControllerPlugin::Websocket::WebsocketEventProcessorInterface
        {
            public:
                StandEventHandler(
                    std::set<UKControllerPlugin::Stands::Stand, UKControllerPlugin::Stands::CompareStands> stands
                );

                size_t CountStands(void) const;
                size_t CountStandAssignments(void) const;
                int GetAssignedStandForCallsign(std::string callsign) const;
                void SetAssignedStand(std::string callsign, int standId);

                // Inherited via WebsocketEventProcessorInterface
                void ProcessWebsocketMessage(const UKControllerPlugin::Websocket::WebsocketMessage& message) override;
                std::set<UKControllerPlugin::Websocket::WebsocketSubscription> GetSubscriptions(void) const override;

                // Inherited via TagItemInterface
                std::string GetTagItemDescription(int tagItemId) const override;
                void SetTagItemData(UKControllerPlugin::Tag::TagData& tagData) override;

                // No stand has been assigned to the aircraft
                const int noStandAssigned = -1;

            private:

                bool AssignmentMessageValid(const nlohmann::json& message) const;
                bool UnassignmentMessageValid(const nlohmann::json & message) const;

                // All the stands we have
                std::set<UKControllerPlugin::Stands::Stand, UKControllerPlugin::Stands::CompareStands> stands;

                // The currently assigned stands and who they are assigned to
                std::map<std::string, int> standAssignments;
        };
    }  // namespace Stands
}  // namespace UKControllerPlugin
