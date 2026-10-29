#include "AircraftStateEventHandler.h"
#include "api/ApiException.h"
#include "api/ApiInterface.h"
#include "euroscope/EuroScopeCFlightPlanInterface.h"
#include "euroscope/EuroscopePluginLoopbackInterface.h"
#include "task/TaskRunnerInterface.h"
#include "time/ParseTimeStrings.h"
#include "time/SystemClock.h"

using UKControllerPlugin::Api::ApiException;
using UKControllerPlugin::Api::ApiInterface;
using UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface;
using UKControllerPlugin::Euroscope::EuroscopePluginLoopbackInterface;
using UKControllerPlugin::Push::PushEvent;
using UKControllerPlugin::Push::PushEventSubscription;
using UKControllerPlugin::TaskManager::TaskRunnerInterface;

namespace UKControllerPlugin::AircraftState {

    AircraftStateEventHandler::AircraftStateEventHandler(
        const ApiInterface& api, TaskRunnerInterface& taskRunner, EuroscopePluginLoopbackInterface& plugin)
        : api(api), taskRunner(taskRunner), plugin(plugin)
    {
    }

    // Written at most once - two clients re-asserting their own copy fight each other forever.
    void AircraftStateEventHandler::FlightPlanEvent(
        EuroScopeCFlightPlanInterface& flightPlan, Euroscope::EuroScopeCRadarTargetInterface& radarTarget)
    {
        const auto callsign = flightPlan.GetCallsign();

        auto lock = this->LockStates();
        if (this->reconciled.contains(callsign)) {
            return;
        }

        const auto state = this->states.find(callsign);
        if (state == this->states.cend()) {
            return;
        }

        if (this->Reconcile(flightPlan, state->second)) {
            this->reconciled.insert(callsign);
        }
    }

    void AircraftStateEventHandler::FlightPlanDisconnectEvent(EuroScopeCFlightPlanInterface& flightPlan)
    {
        auto lock = this->LockStates();
        this->states.erase(flightPlan.GetCallsign());
        this->pendingUpdates.erase(flightPlan.GetCallsign());
        this->reconciled.erase(flightPlan.GetCallsign());
    }

    // A scratchpad write fires this on every connected client, so comparing against the desired
    // state is what suppresses echoes.
    void
    AircraftStateEventHandler::ControllerFlightPlanDataEvent(EuroScopeCFlightPlanInterface& flightPlan, int dataType)
    {
        if (dataType != EuroScopePlugIn::CTR_DATA_TYPE_GROUND_STATE &&
            dataType != EuroScopePlugIn::CTR_DATA_TYPE_CLEARENCE_FLAG) {
            return;
        }

        const auto callsign = flightPlan.GetCallsign();
        const auto desired = this->DesiredState(callsign);

        auto updated = desired;
        nlohmann::json update;

        if (flightPlan.GetClearanceFlag() != desired.clearanceFlag) {
            update["clearance_flag"] = flightPlan.GetClearanceFlag();
            updated.clearanceFlag = flightPlan.GetClearanceFlag();
        }

        const auto groundState = flightPlan.GetGroundState();
        if (!groundState.empty() && groundState != desired.groundState) {
            update["ground_state"] = groundState;
            updated.groundState = groundState;
        }

        if (update.empty()) {
            return;
        }

        auto lock = this->LockStates();

        this->states[callsign] = updated;

        const auto now = Time::TimeNow();
        auto& pending = this->pendingUpdates[callsign];

        for (const auto& field : update.items()) {
            pending.update[field.key()] = field.value();
            pending.changedAt[field.key()] = now;
        }
    }

    void AircraftStateEventHandler::TimedEventTrigger()
    {
        nlohmann::json updates = nlohmann::json::array();

        {
            auto lock = this->LockStates();
            for (const auto& pending : this->pendingUpdates) {
                auto update = pending.second.update;
                update["callsign"] = pending.first;

                for (const auto& changed : pending.second.changedAt) {
                    update[changed.first + "_at"] = Time::ToDateTimeString(changed.second);
                }

                updates.push_back(update);
            }

            this->pendingUpdates.clear();
        }

        if (updates.empty()) {
            return;
        }

        this->taskRunner.QueueAsynchronousTask([this, updates]() {
            try {
                this->api.UpdateAircraftStates(updates);
            } catch (ApiException&) {
                LogError("Unable to update " + std::to_string(updates.size()) + " aircraft states");
            }
        });
    }

    // The only read - PollingPushEventConnection syncs once, at login.
    void AircraftStateEventHandler::PluginEventsSynced()
    {
        this->taskRunner.QueueAsynchronousTask([this]() { this->LoadStates(); });
    }

    void AircraftStateEventHandler::LoadStates()
    {
        try {
            const nlohmann::json aircraftStates = this->api.GetAircraftStates();

            if (!aircraftStates.is_array()) {
                LogWarning("Invalid aircraft state data");
                return;
            }

            auto lock = this->LockStates();
            this->states.clear();

            for (const auto& state : aircraftStates) {
                if (!MessageValid(state)) {
                    LogWarning("Invalid aircraft state on mass assignment " + state.dump());
                    continue;
                }

                this->states[state.at("callsign").get<std::string>()] = StateFromMessage(state);
            }

            LogInfo("Loaded " + std::to_string(this->states.size()) + " aircraft states");
            this->ReconcileAll();
        } catch (ApiException&) {
            LogError("Unable to load aircraft state data");
        }
    }

    void AircraftStateEventHandler::ReconcileAll()
    {
        auto lock = this->LockStates();
        this->reconciled.clear();

        for (const auto& state : this->states) {
            if (this->Reconcile(state.first)) {
                this->reconciled.insert(state.first);
            }
        }
    }

    void AircraftStateEventHandler::ProcessPushEvent(const PushEvent& message)
    {
        // Nothing to do - see GetPushEventSubscriptions.
    }

    /*
        Deliberately nothing - EuroScope propagates changes itself, and clients that re-assert
        what the server told them oscillate. PluginEventsSynced is delivered regardless of
        subscriptions.
    */
    auto AircraftStateEventHandler::GetPushEventSubscriptions() const -> std::set<PushEventSubscription>
    {
        return {};
    }

    auto AircraftStateEventHandler::Count() const -> size_t
    {
        auto lock = this->LockStates();
        return this->states.size();
    }

    auto AircraftStateEventHandler::Get(const std::string& callsign) const -> AircraftState
    {
        return this->DesiredState(callsign);
    }

    auto AircraftStateEventHandler::PendingCount() const -> size_t
    {
        auto lock = this->LockStates();
        return this->pendingUpdates.size();
    }

    auto AircraftStateEventHandler::Reconcile(const std::string& callsign) -> bool
    {
        const auto flightPlan = this->plugin.GetFlightplanForCallsign(callsign);
        if (flightPlan == nullptr) {
            return false;
        }

        return this->Reconcile(*flightPlan, this->DesiredState(callsign));
    }

    auto
    AircraftStateEventHandler::Reconcile(EuroScopeCFlightPlanInterface& flightPlan, const AircraftState& desired) const
        -> bool
    {
        if (!flightPlan.IsTrackedByUser() && flightPlan.IsTracked()) {
            return false;
        }

        if (flightPlan.GetClearanceFlag() != desired.clearanceFlag) {
            flightPlan.SetClearanceFlag(desired.clearanceFlag);
        }

        if (!desired.groundState.empty() && flightPlan.GetGroundState() != desired.groundState) {
            flightPlan.SetGroundState(desired.groundState);
        }

        return true;
    }

    auto AircraftStateEventHandler::DesiredState(const std::string& callsign) const -> AircraftState
    {
        auto lock = this->LockStates();
        const auto state = this->states.find(callsign);
        return state == this->states.cend() ? AircraftState{} : state->second;
    }

    auto AircraftStateEventHandler::LockStates() const -> std::lock_guard<std::recursive_mutex>
    {
        return std::lock_guard(this->stateMutex);
    }

    auto AircraftStateEventHandler::StateFromMessage(const nlohmann::json& message) -> AircraftState
    {
        AircraftState state;
        state.clearanceFlag = message.at("clearance_flag").get<bool>();

        if (!message.at("ground_state").is_null()) {
            state.groundState = message.at("ground_state").get<std::string>();
        }

        return state;
    }

    auto AircraftStateEventHandler::MessageValid(const nlohmann::json& message) -> bool
    {
        return message.is_object() && message.contains("callsign") && message.at("callsign").is_string() &&
               message.contains("clearance_flag") && message.at("clearance_flag").is_boolean() &&
               message.contains("ground_state") &&
               (message.at("ground_state").is_null() ||
                (message.at("ground_state").is_string() &&
                 GROUND_STATES.contains(message.at("ground_state").get<std::string>())));
    }
} // namespace UKControllerPlugin::AircraftState
