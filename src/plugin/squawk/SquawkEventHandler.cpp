#include "SquawkEventHandler.h"
#include "SquawkGenerator.h"
#include "controller/ActiveCallsign.h"
#include "controller/ActiveCallsignCollection.h"
#include "euroscope/EuroScopeCFlightPlanInterface.h"
#include "euroscope/EuroScopeCRadarTargetInterface.h"
#include "euroscope/EuroscopePluginLoopbackInterface.h"
#include "euroscope/GeneralSettingsEntries.h"
#include "euroscope/UserSetting.h"
#include "flightplan/StoredFlightplan.h"
#include "flightplan/StoredFlightplanCollection.h"
#include "login/Login.h"

using UKControllerPlugin::Controller::ActiveCallsign;
using UKControllerPlugin::Controller::ActiveCallsignCollection;
using UKControllerPlugin::Controller::ControllerPosition;
using UKControllerPlugin::Controller::Login;
using UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface;
using UKControllerPlugin::Euroscope::EuroScopeCRadarTargetInterface;
using UKControllerPlugin::Euroscope::EuroscopePluginLoopbackInterface;
using UKControllerPlugin::Euroscope::GeneralSettingsEntries;
using UKControllerPlugin::Euroscope::UserSetting;
using UKControllerPlugin::Flightplan::StoredFlightplanCollection;
using UKControllerPlugin::Squawk::SquawkAssignment;
using UKControllerPlugin::Squawk::SquawkGenerator;

namespace UKControllerPlugin::Squawk {

    SquawkEventHandler::SquawkEventHandler(
        SquawkGenerator& generator,
        const ActiveCallsignCollection& activeCallsigns,
        const StoredFlightplanCollection& storedFlightplans,
        EuroscopePluginLoopbackInterface& pluginLoopback,
        const Login& login,
        bool automaticAssignmentDisabled)
        : generator(generator), activeCallsigns(activeCallsigns), storedFlightplans(storedFlightplans),
          pluginLoopback(pluginLoopback), login(login), automaticAssignmentDisabled(automaticAssignmentDisabled)
    {
    }

    /*
        Handle flightplan events
    */
    void SquawkEventHandler::FlightPlanEvent(
        EuroScopeCFlightPlanInterface& flightplan, EuroScopeCRadarTargetInterface& radarTarget)
    {

        if (this->automaticAssignmentDisabled || !this->userAutomaticAssignmentEnabled) {
            return;
        }

        this->AttemptAssignment(flightplan, radarTarget);
    }

    /*
        Nothing to handle here
    */
    void SquawkEventHandler::FlightPlanDisconnectEvent(EuroScopeCFlightPlanInterface& flightPlan)
    {
    }

    /*
        If we get a controller assigned data update, make sure the residual squawk is set.
    */
    void SquawkEventHandler::ControllerFlightPlanDataEvent(EuroScopeCFlightPlanInterface& flightPlan, int dataType)
    {
        if (dataType != EuroScopePlugIn::CTR_DATA_TYPE_SQUAWK) {
            return;
        }

        if (!this->storedFlightplans.HasFlightplanForCallsign(flightPlan.GetCallsign())) {
            return;
        }

        this->storedFlightplans.GetFlightplanForCallsign(flightPlan.GetCallsign())
            .SetPreviouslyAssignedSquawk(flightPlan.GetAssignedSquawk());
    }

    /*
        Force a squawk recycle for a general squawk.
    */
    void SquawkEventHandler::SquawkReycleGeneral(
        EuroScopeCFlightPlanInterface& flightplan,
        EuroScopeCRadarTargetInterface& radarTarget,
        const std::string& context,
        const POINT& mousePoss) const
    {
        LogInfo("Forcing general squawk update for " + flightplan.GetCallsign());
        this->generator.ForceGeneralSquawkForAircraft(flightplan, radarTarget);
    }

    /*
        Force a squawk recycle for a local squawk.
    */
    void SquawkEventHandler::SquawkRecycleLocal(
        EuroScopeCFlightPlanInterface& flightplan,
        EuroScopeCRadarTargetInterface& radarTarget,
        const std::string& context,
        const POINT& mousePos) const
    {
        LogInfo("Forcing local squawk update for " + flightplan.GetCallsign());
        this->generator.ForceLocalSquawkForAircraft(flightplan, radarTarget);
    }

    /*
        When the timed event goes off, check for tracked aircraft and whether they need squawks to be assigned.
        This is required because EuroScope doesn't provide a method to us akin to "OnAssumeAircraft".
    */
    void SquawkEventHandler::TimedEventTrigger()
    {
        if (!this->activeCallsigns.UserHasCallsign() || this->automaticAssignmentDisabled ||
            !this->userAutomaticAssignmentEnabled) {
            return;
        }

        this->AttemptAssignSquawksToAllAircraft();
    }

    /*
        User settings have been updated, check whether the user has selected to have
        automatic squawk assignments.
    */
    void SquawkEventHandler::UserSettingsUpdated(UserSetting& userSettings)
    {
        this->userAutomaticAssignmentEnabled =
            userSettings.GetBooleanEntry(GeneralSettingsEntries::squawkToggleSettingsKey, true);
    }

    /*
        Returns whether or not the user has allowed squawk assignments
    */
    auto SquawkEventHandler::UserAllowedSquawkAssignment() const -> bool
    {
        return this->userAutomaticAssignmentEnabled;
    }

    /*
        If the ActiveCallsign is the user callsign, then force a squawk regenerate.
    */
    void SquawkEventHandler::ActiveCallsignAdded(const ActiveCallsign& callsign)
    {
        if (!callsign.GetIsUser()) {
            return;
        }

        LogInfo("User is now an active callsign, mass assigning squawks");
        this->AttemptAssignSquawksToAllAircraft();
    }

    /*
        Nothing to see here
    */
    void SquawkEventHandler::ActiveCallsignRemoved(const ActiveCallsign& callsign)
    {
    }

    /*
        Nothing to see here
    */
    void SquawkEventHandler::CallsignsFlushed()
    {
    }

    void SquawkEventHandler::AttemptAssignment(
        EuroScopeCFlightPlanInterface& flightplan, EuroScopeCRadarTargetInterface& radarTarget) const
    {
        if (this->login.GetSecondsLoggedIn() < this->minAutomaticAssignmentLoginTime) {
            LogDebug(
                "Skipping squawk assignment for " + flightplan.GetCallsign() + " for now, as only recently logged in");
            return;
        }

        this->generator.AssignCircuitSquawkForAircraft(flightplan, radarTarget) ||
            this->generator.ReassignPreviousSquawkToAircraft(flightplan, radarTarget) ||
            this->generator.RequestLocalSquawkForAircraft(flightplan, radarTarget) ||
            this->generator.RequestGeneralSquawkForAircraft(flightplan, radarTarget);
    }

    void SquawkEventHandler::AttemptAssignSquawksToAllAircraft() const
    {
        this->pluginLoopback.ApplyFunctionToAllFlightplans(
            [this](
                const std::shared_ptr<EuroScopeCFlightPlanInterface>& fp,
                const std::shared_ptr<EuroScopeCRadarTargetInterface>& rt) {
                if (fp->HasAssignedSquawk() || (fp->IsTracked() && !fp->IsTrackedByUser())) {
                    return;
                }

                this->AttemptAssignment(*fp, *rt);
            });
    }

    auto SquawkEventHandler::AutomaticAssignmentsDisabled() const -> bool
    {
        return this->automaticAssignmentDisabled;
    }
} // namespace UKControllerPlugin::Squawk
