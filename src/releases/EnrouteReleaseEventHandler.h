#pragma once
#include "websocket/WebsocketEventProcessorInterface.h"
#include "api/ApiInterface.h"
#include "releases/EnrouteReleaseType.h"
#include "releases/CompareEnrouteReleaseTypes.h"
#include "tag/TagItemInterface.h"
#include "releases/EnrouteRelease.h"
#include "timedevent/AbstractTimedEvent.h"
#include "euroscope/EuroscopePluginLoopbackInterface.h"
#include "controller/HandoffEventHandlerInterface.h"
#include "task/TaskRunnerInterface.h"

namespace UKControllerPlugin {
    namespace Releases {

        /*
            Handles events around enroute releases
        */
        class EnrouteReleaseEventHandler: public UKControllerPlugin::Websocket::WebsocketEventProcessorInterface,
            public UKControllerPlugin::Tag::TagItemInterface,
            public UKControllerPlugin::TimedEvent::AbstractTimedEvent,
            public UKControllerPlugin::Controller::HandoffEventHandlerInterface
        {
            public:
                EnrouteReleaseEventHandler(
                    const UKControllerPlugin::Api::ApiInterface& api,
                    UKControllerPlugin::Euroscope::EuroscopePluginLoopbackInterface& plugin,
                    UKControllerPlugin::TaskManager::TaskRunnerInterface& taskRunner,
                    const std::set<
                        UKControllerPlugin::Releases::EnrouteReleaseType,
                        UKControllerPlugin::Releases::CompareEnrouteReleaseTypes
                    > releaseTypes,
                    const int releaseTypeSelectedCallbackId,
                    const int editReleasePointCallbackId
                );
                void AddIncomingRelease(
                    const std::string callsign,
                    UKControllerPlugin::Releases::EnrouteRelease release
                );
                void AddOutgoingRelease(
                    const std::string callsign,
                    UKControllerPlugin::Releases::EnrouteRelease release
                );
                const UKControllerPlugin::Releases::EnrouteRelease & GetIncomingRelease(
                    const std::string callsign
                ) const;
                const UKControllerPlugin::Releases::EnrouteRelease& GetOutgoingRelease(
                    const std::string callsign
                ) const;
                const std::set<
                    UKControllerPlugin::Releases::EnrouteReleaseType,
                    UKControllerPlugin::Releases::CompareEnrouteReleaseTypes
                > GetReleaseTypes(void) const;
                bool ReleaseMessageValid(const nlohmann::json& message) const;
                void DisplayReleaseTypeMenu(
                    UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface& flightplan,
                    UKControllerPlugin::Euroscope::EuroScopeCRadarTargetInterface& radarTarget,
                    std::string context,
                    const POINT& mousePos
                );
                void ReleaseTypeSelected(int functionId, std::string context, RECT);
                void DisplayReleasePointEditBox(
                    UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface& flightplan,
                    UKControllerPlugin::Euroscope::EuroScopeCRadarTargetInterface& radarTarget,
                    std::string context,
                    const POINT& mousePos
                );
                void EditReleasePoint(int functionId, std::string context, RECT);

                // Inherited via WebsocketEventProcessorInterface
                void ProcessWebsocketMessage(const UKControllerPlugin::Websocket::WebsocketMessage& message) override;
                std::set<UKControllerPlugin::Websocket::WebsocketSubscription> GetSubscriptions(void) const override;

                // Inherited via TagItemInterface
                std::string GetTagItemDescription(int tagItemId) const override;
                void SetTagItemData(UKControllerPlugin::Tag::TagData& tagData) override;

                // Inherited via AbstractTimedEvent
                void TimedEventTrigger(void) override;

                // Inherited via HandoffEventHandlerInterface
                void HandoffInitiated(
                    UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface& flightplan,
                    UKControllerPlugin::Euroscope::EuroScopeCControllerInterface& transferringController,
                    UKControllerPlugin::Euroscope::EuroScopeCControllerInterface& targetController
                ) override;

                // Colours
                const COLORREF outgoingItemColour = RGB(255, 255, 0);
                const COLORREF incomingItemColour = RGB(255, 0, 0);

                // Tag items
                const int enrouteReleaseTypeTagItemId = 108;
                const int enrouteReleasePointTagItemId = 109;
                const int enrouteReleasePointOrBlankTagItemId = 111;

                // Function ids
                const int releaseTypeSelectedCallbackId;
                const int editReleasePointCallbackId;

                const std::string noReleaseItemColumn1 = "N";
                const std::string noReleaseItemColumn2 = "No Release";

                // Invalid release
                const UKControllerPlugin::Releases::EnrouteRelease invalidRelease = {-1};

            private:

                void UpdateOutgoingReleaseType(
                    std::string callsign,
                    int type
                );

                std::string GetNoReleasePointText(int tagId) const;

                // For sending releases
                const UKControllerPlugin::Api::ApiInterface& api;

                // The plugin instance
                UKControllerPlugin::Euroscope::EuroscopePluginLoopbackInterface& plugin;

                // For running tasks asynchronously
                UKControllerPlugin::TaskManager::TaskRunnerInterface& taskRunner;

                // The different type of enroute releases
                const std::set<
                    UKControllerPlugin::Releases::EnrouteReleaseType,
                    UKControllerPlugin::Releases::CompareEnrouteReleaseTypes
                > releaseTypes;

                // Release point string to use when there is no release point
                const std::string noReleasePoint = "";

                // All incoming enroute releases
                std::map<std::string, UKControllerPlugin::Releases::EnrouteRelease> incomingReleases;

                // All outgoing enroute releases
                std::map<std::string, UKControllerPlugin::Releases::EnrouteRelease> outgoingReleases;

                // TAG item descriptions
                std::map<int, std::string> tagItemDescriptions {
                    {enrouteReleaseTypeTagItemId, "Enroute Release Type"},
                    {enrouteReleasePointTagItemId, "Enroute Release Point"},
                    {enrouteReleasePointOrBlankTagItemId, "Enroute Release Point or Blank"},
                };
            };
    }  // namespace Releases
}  // namespace UKControllerPlugin
