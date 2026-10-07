#pragma once
#include "AircraftState.h"
#include "flightplan/FlightPlanEventHandlerInterface.h"
#include "push/PushEventProcessorInterface.h"
#include "timedevent/AbstractTimedEvent.h"

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

namespace UKControllerPlugin::AircraftState {

    class AircraftStateEventHandler : public Flightplan::FlightPlanEventHandlerInterface,
                                      public Push::PushEventProcessorInterface,
                                      public TimedEvent::AbstractTimedEvent
    {
        public:
        AircraftStateEventHandler(
            const Api::ApiInterface& api,
            TaskManager::TaskRunnerInterface& taskRunner,
            Euroscope::EuroscopePluginLoopbackInterface& plugin);

        // Inherited via FlightPlanEventHandlerInterface
        void FlightPlanEvent(
            Euroscope::EuroScopeCFlightPlanInterface& flightPlan,
            Euroscope::EuroScopeCRadarTargetInterface& radarTarget) override;
        void FlightPlanDisconnectEvent(Euroscope::EuroScopeCFlightPlanInterface& flightPlan) override;
        void ControllerFlightPlanDataEvent(Euroscope::EuroScopeCFlightPlanInterface& flightPlan, int dataType) override;

        // Inherited via PushEventProcessorInterface
        void ProcessPushEvent(const Push::PushEvent& message) override;
        void PluginEventsSynced() override;
        [[nodiscard]] auto GetPushEventSubscriptions() const -> std::set<Push::PushEventSubscription> override;

        // Inherited via AbstractTimedEvent
        void TimedEventTrigger() override;

        [[nodiscard]] auto Count() const -> size_t;
        [[nodiscard]] auto Get(const std::string& callsign) const -> AircraftState;
        [[nodiscard]] auto PendingCount() const -> size_t;

        private:
        struct PendingUpdate
        {
            nlohmann::json update;
            std::map<std::string, std::chrono::system_clock::time_point> changedAt;
        };

        void LoadStates();
        void ReconcileAll();

        // Both return whether the aircraft could be written at all, not whether anything changed.
        auto Reconcile(const std::string& callsign) -> bool;

        [[nodiscard]] auto
        Reconcile(Euroscope::EuroScopeCFlightPlanInterface& flightPlan, const AircraftState& desired) const -> bool;
        [[nodiscard]] auto DesiredState(const std::string& callsign) const -> AircraftState;
        [[nodiscard]] auto LockStates() const -> std::lock_guard<std::recursive_mutex>;
        static auto StateFromMessage(const nlohmann::json& message) -> AircraftState;
        static auto MessageValid(const nlohmann::json& message) -> bool;

        const Api::ApiInterface& api;
        TaskManager::TaskRunnerInterface& taskRunner;
        Euroscope::EuroscopePluginLoopbackInterface& plugin;

        std::map<std::string, AircraftState> states;

        std::map<std::string, PendingUpdate> pendingUpdates;
        std::set<std::string> reconciled;

        mutable std::recursive_mutex stateMutex;
    };
} // namespace UKControllerPlugin::AircraftState
