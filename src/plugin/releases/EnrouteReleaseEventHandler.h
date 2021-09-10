#pragma once
#include "CompareEnrouteReleaseTypes.h"
#include "EnrouteRelease.h"
#include "EnrouteReleaseType.h"
#include "controller/HandoffEventHandlerInterface.h"
#include "push/PushEventProcessorInterface.h"
#include "tag/TagItemInterface.h"
#include "timedevent/AbstractTimedEvent.h"

namespace UKControllerPlugin {
    namespace Api {
        class ApiInterface;
    } // namespace Api
    namespace Euroscope {
        class EuroScopeCFlightPlanInterface;
        class EuroScopeCRadarTargetInterface;
        class EuroscopePluginLoopbackInterface;
    } // namespace Euroscope
    namespace TaskManager {
        class TaskRunnerInterface;
    } // namespace TaskManager
} // namespace UKControllerPlugin

namespace UKControllerPlugin::Releases {

    /*
        Handles events around enroute releases
    */
    class EnrouteReleaseEventHandler : public Push::PushEventProcessorInterface,
                                       public UKControllerPlugin::Tag::TagItemInterface,
                                       public UKControllerPlugin::TimedEvent::AbstractTimedEvent,
                                       public UKControllerPlugin::Controller::HandoffEventHandlerInterface
    {
        public:
        EnrouteReleaseEventHandler(
            const UKControllerPlugin::Api::ApiInterface& api,
            UKControllerPlugin::Euroscope::EuroscopePluginLoopbackInterface& plugin,
            UKControllerPlugin::TaskManager::TaskRunnerInterface& taskRunner,
            std::set<
                UKControllerPlugin::Releases::EnrouteReleaseType,
                UKControllerPlugin::Releases::CompareEnrouteReleaseTypes> releaseTypes,
            int releaseTypeSelectedCallbackId,
            int editReleasePointCallbackId);
        void AddIncomingRelease(const std::string& callsign, UKControllerPlugin::Releases::EnrouteRelease release);
        void AddOutgoingRelease(const std::string& callsign, UKControllerPlugin::Releases::EnrouteRelease release);
        [[nodiscard]] auto GetIncomingRelease(const std::string& callsign) const
            -> const UKControllerPlugin::Releases::EnrouteRelease&;
        [[nodiscard]] auto GetOutgoingRelease(const std::string& callsign) const
            -> const UKControllerPlugin::Releases::EnrouteRelease&;
        [[nodiscard]] auto GetInvalidRelease() const
            -> const UKControllerPlugin::Releases::EnrouteRelease&;
        [[nodiscard]] auto ReleaseMessageValid(const nlohmann::json& message) const -> bool;
        void DisplayReleaseTypeMenu(
            UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface& flightplan,
            UKControllerPlugin::Euroscope::EuroScopeCRadarTargetInterface& radarTarget,
            const std::string& context,
            const POINT& mousePos);
        void ReleaseTypeSelected(int functionId, const std::string& context, RECT);
        void DisplayReleasePointEditBox(
            UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface& flightplan,
            UKControllerPlugin::Euroscope::EuroScopeCRadarTargetInterface& radarTarget,
            const std::string& context,
            const POINT& mousePos);
        void EditReleasePoint(int functionId, const std::string& context, RECT);

        // Inherited via WebsocketEventProcessorInterface
        void ProcessPushEvent(const Push::PushEvent& message) override;
        [[nodiscard]] auto GetPushEventSubscriptions() const -> std::set<Push::PushEventSubscription> override;

        // Inherited via TagItemInterface
        [[nodiscard]] auto GetTagItemDescription(int tagItemId) const -> std::string override;
        void SetTagItemData(UKControllerPlugin::Tag::TagData& tagData) override;

        // Inherited via AbstractTimedEvent
        void TimedEventTrigger() override;

        // Inherited via HandoffEventHandlerInterface
        void HandoffInitiated(
            UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface& flightplan,
            UKControllerPlugin::Euroscope::EuroScopeCControllerInterface& transferringController,
            UKControllerPlugin::Euroscope::EuroScopeCControllerInterface& targetController) override;
        void PluginEventsSynced() override;

        [[nodiscard]] auto GetReleaseTypeSelectedCallbackId() const -> int;
        [[nodiscard]] auto GetEditReleasePointCallbackId() const -> int;
        [[nodiscard]] static auto GetReleaseTypeTagItemId() -> int;
        [[nodiscard]] static auto GetReleasePointTagItemId() -> int;
        [[nodiscard]] static auto GetReleasePointOrBlankTagItemId() -> int;

        private:
        void UpdateOutgoingReleaseType(const std::string& callsign, int type);

        [[nodiscard]] auto GetNoReleasePointText(int tagId) const -> std::string;

        // For sending releases
        const UKControllerPlugin::Api::ApiInterface& api;

        // The plugin instance
        UKControllerPlugin::Euroscope::EuroscopePluginLoopbackInterface& plugin;

        // For running tasks asynchronously
        UKControllerPlugin::TaskManager::TaskRunnerInterface& taskRunner;

        // The different type of enroute releases
        const std::set<
            UKControllerPlugin::Releases::EnrouteReleaseType,
            UKControllerPlugin::Releases::CompareEnrouteReleaseTypes>
            releaseTypes;

        // Release point string to use when there is no release point
        const std::string noReleasePoint;

        // All incoming enroute releases
        std::map<std::string, UKControllerPlugin::Releases::EnrouteRelease> incomingReleases;

        // All outgoing enroute releases
        std::map<std::string, UKControllerPlugin::Releases::EnrouteRelease> outgoingReleases;

        // Tag items
        static const int enrouteReleaseTypeTagItemId = 108;
        static const int enrouteReleasePointTagItemId = 109;
        static const int enrouteReleasePointOrBlankTagItemId = 111;

        // Function ids
        const int releaseTypeSelectedCallbackId;
        const int editReleasePointCallbackId;

        const std::string noReleaseItemColumn1 = "N";
        const std::string noReleaseItemColumn2 = "No Release";

        // Invalid release
        const UKControllerPlugin::Releases::EnrouteRelease invalidRelease = {-1};

        // TAG item descriptions
        std::map<int, std::string> tagItemDescriptions{
            {enrouteReleaseTypeTagItemId, "Enroute Release Type"},
            {enrouteReleasePointTagItemId, "Enroute Release Point"},
            {enrouteReleasePointOrBlankTagItemId, "Enroute Release Point or Blank"},
        };
    };
} // namespace UKControllerPlugin::Releases
