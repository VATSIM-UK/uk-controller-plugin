#pragma once
#include "stands/Stand.h"
#include "stands/CompareStands.h"
#include "tag/TagItemInterface.h"
#include "api/ApiInterface.h"
#include "task/TaskRunnerInterface.h"
#include "websocket/WebsocketEventProcessorInterface.h"
#include "euroscope/EuroscopePluginLoopbackInterface.h"

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
                    const UKControllerPlugin::Api::ApiInterface& api,
                    UKControllerPlugin::TaskManager::TaskRunnerInterface& taskRunner,
                    UKControllerPlugin::Euroscope::EuroscopePluginLoopbackInterface& plugin,
                    std::set<UKControllerPlugin::Stands::Stand, UKControllerPlugin::Stands::CompareStands> stands,
                    int standSelectedCallbackId
                );

                void AnnotateFlightStrip(std::string callsign, int standId) const;

                size_t CountStands(void) const;
                size_t CountStandAssignments(void) const;
                void DisplayStandSelectionMenu(
                    UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface& flightplan,
                    UKControllerPlugin::Euroscope::EuroScopeCRadarTargetInterface& radarTarget,
                    std::string context,
                    const POINT& mousePos
                );
                int GetAssignedStandForCallsign(std::string callsign) const;
                std::string GetLastAirfield(void) const;
                void RemoveFlightStripAnnotation(std::string callsign) const;
                void SetAssignedStand(std::string callsign, int standId);
                void StandSelected(int functionId, std::string context, RECT);
                void DisplayStandAssignmentEditBox(
                    UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface& flightplan,
                    UKControllerPlugin::Euroscope::EuroScopeCRadarTargetInterface& radarTarget,
                    std::string context,
                    const POINT& mousePos
                );

                // Inherited via WebsocketEventProcessorInterface
                void ProcessWebsocketMessage(const UKControllerPlugin::Websocket::WebsocketMessage& message) override;
                std::set<UKControllerPlugin::Websocket::WebsocketSubscription> GetSubscriptions(void) const override;

                // Inherited via TagItemInterface
                std::string GetTagItemDescription(int tagItemId) const override;
                void SetTagItemData(UKControllerPlugin::Tag::TagData& tagData) override;

                // No stand has been assigned to the aircraft
                const int noStandAssigned = -1;

                // The id for the callback when a stand is selected
                const int standSelectedCallbackId;

                // Max distance from departure airport to decide whether we show departure or arrival airport stands
                const int maxDistanceFromDepartureAirport = 7;

                // The menu item to display for no assigned stand
                const std::string noStandMenuItem = "--";

                // The text we get if no stand is specified in the edit box
                const std::string noStandEditBoxItem = "";

                // Annotation box 4
                const int annotationIndex = 3;

            private:

                bool AssignmentMessageValid(const nlohmann::json& message) const;
                bool CanAssignStand(UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface& flightplan) const;
                bool UnassignmentMessageValid(const nlohmann::json & message) const;
                std::string GetAirfieldForStandAssignment(
                    UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface& flightplan
                );

                // The last airfield that was used to populate the stand menu, used when we receive the callback
                std::string lastAirfieldUsed = "";

                // The API for making requests in relation to stands
                const UKControllerPlugin::Api::ApiInterface& api;

                // Allows API calls to be made asynchronously
                UKControllerPlugin::TaskManager::TaskRunnerInterface& taskRunner;

                // Used to interact with the main plugin
                UKControllerPlugin::Euroscope::EuroscopePluginLoopbackInterface& plugin;

                // All the stands we have
                std::set<UKControllerPlugin::Stands::Stand, UKControllerPlugin::Stands::CompareStands> stands;

                // The currently assigned stands and who they are assigned to
                std::map<std::string, int> standAssignments;
        };
    }  // namespace Stands
}  // namespace UKControllerPlugin
