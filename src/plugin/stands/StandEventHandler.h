#pragma once
#include "CompareStands.h"
#include "Stand.h"
#include "flightplan/FlightPlanEventHandlerInterface.h"
#include "integration/ExternalMessageHandlerInterface.h"
#include "integration/IntegrationActionProcessor.h"
#include "integration/OutboundIntegrationEventHandler.h"
#include "push/PushEventProcessorInterface.h"
#include "tag/TagItemInterface.h"

namespace UKControllerPlugin {
    namespace Api {
        class ApiInterface;
    } // namespace Api
    namespace Euroscope {
        class EuroscopePluginLoopbackInterface;
    } // namespace Euroscope
    namespace TaskManager {
        class TaskRunnerInterface;
    } // namespace TaskManager
} // namespace UKControllerPlugin

namespace UKControllerPlugin::Stands {
    /*
        Handles events related to stands.
    */
    class StandEventHandler : public Tag::TagItemInterface,
                              public Push::PushEventProcessorInterface,
                              public Flightplan::FlightPlanEventHandlerInterface,
                              public Integration::ExternalMessageHandlerInterface,
                              public Integration::IntegrationActionProcessor
    {
        public:
        StandEventHandler(
            const UKControllerPlugin::Api::ApiInterface& api,
            TaskManager::TaskRunnerInterface& taskRunner,
            Euroscope::EuroscopePluginLoopbackInterface& plugin,
            Integration::OutboundIntegrationEventHandler& integrationEventHandler,
            std::set<Stands::Stand, UKControllerPlugin::Stands::CompareStands> stands,
            int standSelectedCallbackId);
        [[nodiscard]] auto ActionsToProcess() const -> std::vector<Integration::MessageType> override;
        void ProcessAction(
            std::shared_ptr<Integration::MessageInterface> message,
            std::function<void(void)> success,
            std::function<void(std::vector<std::string>)> fail) override;

        void AnnotateFlightStrip(const std::string& callsign, int standId) const;
        auto DoStandAssignment(
            const std::shared_ptr<UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface>& aircraft,
            const std::string& airfield,
            std::string identifier) -> std::string;

        [[nodiscard]] auto CountStandAssignments() const -> size_t;
        void DisplayStandSelectionMenu(
            UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface& flightplan,
            UKControllerPlugin::Euroscope::EuroScopeCRadarTargetInterface& radarTarget,
            const std::string& context,
            const POINT& mousePos);
        [[nodiscard]] auto GetAssignedStandForCallsign(const std::string& callsign) const -> int;
        [[nodiscard]] auto GetLastAirfield() const -> std::string;
        void RemoveFlightStripAnnotation(const std::string& callsign) const;
        void SetAssignedStand(const std::string& callsign, int standId);
        void StandSelected(int functionId, std::string context, RECT);
        void DisplayStandAssignmentEditBox(
            UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface& flightplan,
            UKControllerPlugin::Euroscope::EuroScopeCRadarTargetInterface& radarTarget,
            const std::string& context,
            const POINT& mousePos);
        void ProcessPushEvent(const Push::PushEvent& message) override;
        [[nodiscard]] auto GetPushEventSubscriptions() const -> std::set<Push::PushEventSubscription> override;

        // Inherited via TagItemInterface
        [[nodiscard]] auto GetTagItemDescription(int tagItemId) const -> std::string override;
        void SetTagItemData(UKControllerPlugin::Tag::TagData& tagData) override;

        // Inherited via FlightPlanEventHandlerInterface
        void FlightPlanEvent(
            UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface& flightPlan,
            UKControllerPlugin::Euroscope::EuroScopeCRadarTargetInterface& radarTarget) override;
        void
        FlightPlanDisconnectEvent(UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface& flightPlan) override;
        void ControllerFlightPlanDataEvent(
            UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface& flightPlan, int dataType) override;
        void PluginEventsSynced() override;

        // Inherited via ExternalMessageHandlerInterface
        auto ProcessMessage(std::string message) -> bool override;

        // No stand has been assigned to the aircraft
        inline static const int noStandAssigned = -1;

        private:
        void AssignStandToAircraft(const std::string& callsign, const Stand& stand);
        void UnassignStandForAircraft(const std::string& callsign);
        [[nodiscard]] auto AssignmentMessageValid(const nlohmann::json& message) const -> bool;
        auto CanAssignStand(UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface& flightplan) const -> bool;
        static auto UnassignmentMessageValid(const nlohmann::json& message) -> bool;
        auto
        GetAirfieldForStandAssignment(UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface& flightplan) const
            -> std::string;
        auto LockStandMap() -> std::lock_guard<std::mutex>;

        // The last airfield that was used to populate the stand menu, used when we receive the callback
        std::string lastAirfieldUsed;

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

        // Locks the stand assignments map to prevent concurrent edits
        std::mutex mapMutex;

        // Allows us to send events to our integrations
        Integration::OutboundIntegrationEventHandler& integrationEventHandler;

        // The id for the callback when a stand is selected
        const int standSelectedCallbackId;

        // Max distance from departure airport to decide whether we show departure or arrival airport stands
        const int maxDistanceFromDepartureAirport = 7;

        // The menu item to display for no assigned stand
        const std::string noStandMenuItem = "--";

        // The text we get if no stand is specified in the edit box
        const std::string noStandEditBoxItem;

        // Annotation box 4
        const int annotationIndex = 3;
    };
} // namespace UKControllerPlugin::Stands
