#include "SquawkAssignment.h"
#include "controller/ActiveCallsign.h"
#include "controller/ActiveCallsignCollection.h"
#include "controller/ControllerPosition.h"
#include "euroscope/EuroScopeCControllerInterface.h"
#include "euroscope/EuroScopeCFlightPlanInterface.h"
#include "euroscope/EuroScopeCRadarTargetInterface.h"
#include "euroscope/EuroscopePluginLoopbackInterface.h"
#include "flightplan/StoredFlightplan.h"
#include "flightplan/StoredFlightplanCollection.h"
#include "ownership/AirfieldOwnershipManager.h"

using UKControllerPlugin::Controller::ActiveCallsign;
using UKControllerPlugin::Controller::ActiveCallsignCollection;
using UKControllerPlugin::Controller::ControllerPosition;
using UKControllerPlugin::Euroscope::EuroScopeCControllerInterface;
using UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface;
using UKControllerPlugin::Euroscope::EuroScopeCRadarTargetInterface;
using UKControllerPlugin::Euroscope::EuroscopePluginLoopbackInterface;
using UKControllerPlugin::Flightplan::StoredFlightplanCollection;
using UKControllerPlugin::Ownership::AirfieldOwnershipManager;

namespace UKControllerPlugin::Squawk {

    SquawkAssignment::SquawkAssignment(
        const StoredFlightplanCollection& storedFlightplans,
        EuroscopePluginLoopbackInterface& plugin,
        const AirfieldOwnershipManager& airfieldOwnership,
        const ActiveCallsignCollection& activeCallsigns)
        : activeCallsigns(activeCallsigns), storedFlightplans(storedFlightplans), airfieldOwnership(airfieldOwnership),
          plugin(plugin)
    {
    }

    /*
        Returns whether or not the aircraft is believed to be doing VFR circuits
    */
    auto SquawkAssignment::CircuitAssignmentNeeded(
        EuroScopeCFlightPlanInterface& flightplan, EuroScopeCRadarTargetInterface& radarTarget) const -> bool
    {
        return flightplan.IsVfr() && !flightplan.HasAssignedSquawk() &&
               std::regex_search(
                   flightplan.GetRawRouteString(), std::regex("^(?:VFR )?CIRCUIT(?:S)?", std::regex::icase)) &&
               this->GeneralAssignmentNeeded(flightplan, radarTarget);
    }

    /*
        Returns whether or not the controller is in a position that they may force squawk assignment.
    */
    auto SquawkAssignment::ForceAssignmentAllowed(EuroScopeCFlightPlanInterface& flightplan) const -> bool
    {
        std::shared_ptr<EuroScopeCControllerInterface> myController = this->plugin.GetUserControllerObject();

        return myController != nullptr && myController->IsVatsimRecognisedController() &&
               (flightplan.IsTrackedByUser() || !flightplan.IsTracked());
    }

    /*
        Returns whether or not a squawk assignment should be forced.
    */
    auto SquawkAssignment::ForceAssignmentNeeded(EuroScopeCFlightPlanInterface& flightplan) const -> bool
    {
        return this->storedFlightplans.HasFlightplanForCallsign(flightplan.GetCallsign()) &&
               this->storedFlightplans.GetFlightplanForCallsign(flightplan.GetCallsign()) != flightplan;
    }

    /*
        Returns true if a forced assignment is required or no squawk is assigned.
    */
    auto SquawkAssignment::ForceAssignmentOrNoSquawkAssigned(EuroScopeCFlightPlanInterface& flightplan) const -> bool
    {
        return !flightplan.HasAssignedSquawk() || this->ForceAssignmentNeeded(flightplan);
    }

    /*
        Checks whether or not an assignment is needed for an automated squawk check.
    */
    auto SquawkAssignment::GeneralAssignmentNeeded(
        const EuroScopeCFlightPlanInterface& flightPlan, const EuroScopeCRadarTargetInterface& radarTarget) const
        -> bool
    {
        if (!this->AssignmentPossible(flightPlan, radarTarget)) {
            return false;
        }

        if (!flightPlan.IsTrackedByUser()) {
            return radarTarget.GetGroundSpeed() <= this->untrackedMaxAssignmentSpeed &&
                   flightPlan.GetDistanceFromOrigin() <= this->untrackedMaxAssignmentDistanceFromOrigin &&
                   !flightPlan.HasAssignedSquawk() &&
                   this->airfieldOwnership.AirfieldOwnedBy(
                       flightPlan.GetOrigin(), this->activeCallsigns.GetUserCallsign());
        }

        return !flightPlan.HasAssignedSquawk();
    }

    /*
        Returns whether or not a local squawk needs to be assigned.
    */
    auto SquawkAssignment::LocalAssignmentNeeded(
        const EuroScopeCFlightPlanInterface& flightPlan, const EuroScopeCRadarTargetInterface& radarTarget) const
        -> bool
    {
        if (!this->AssignmentPossible(flightPlan, radarTarget)) {
            return false;
        }

        return flightPlan.IsTrackedByUser() ? this->NeedsLocalSquawkTracked(flightPlan, radarTarget)
                                            : this->NeedsLocalSquawkUntracked(flightPlan, radarTarget);
    }

    /*
        Returns whether or not a tracked aircraft needs a local squawk.
    */
    auto SquawkAssignment::NeedsLocalSquawkTracked(
        const EuroScopeCFlightPlanInterface& flightPlan, const EuroScopeCRadarTargetInterface& radarTarget) const
        -> bool
    {
        if (this->activeCallsigns.GetUserCallsign().GetNormalisedPosition().GetType() == "CTR") {
            return !flightPlan.HasAssignedSquawk() && flightPlan.GetCruiseLevel() <= this->maxAssignmentAltitude;
        }

        // If the controller is APP/TWR, their vis centre should be on the airfield.
        return !flightPlan.HasAssignedSquawk() && radarTarget.GetFlightLevel() <= this->maxAssignmentAltitude &&
               this->plugin.GetDistanceFromUserVisibilityCentre(radarTarget.GetPosition()) <= this->trackedLarsRadius;
    }

    /*
        Whether or not an untracked aircraft require a local squawk.
    */
    auto SquawkAssignment::NeedsLocalSquawkUntracked(
        const EuroScopeCFlightPlanInterface& flightPlan, const EuroScopeCRadarTargetInterface& radarTarget) const
        -> bool
    {
        return (flightPlan.GetOrigin() == flightPlan.GetDestination() || flightPlan.GetFlightRules() == "V" ||
                (!flightPlan.HasSid() && flightPlan.GetCruiseLevel() <= this->maxAssignmentAltitude)) &&
               radarTarget.GetGroundSpeed() <= this->untrackedMaxAssignmentSpeed &&
               flightPlan.GetDistanceFromOrigin() <= this->untrackedMaxAssignmentDistanceFromOrigin &&
               this->airfieldOwnership.AirfieldOwnedBy(
                   flightPlan.GetOrigin(), this->activeCallsigns.GetUserCallsign()) &&
               !flightPlan.HasAssignedSquawk();
    }

    /*
        Returns true if a previously stored flightplan for a given callsign has an assigned squawk.
        For example, if they disconnect for an extended period.
    */
    auto SquawkAssignment::PreviousSquawkNeedsReassignment(
        const EuroScopeCFlightPlanInterface& flightPlan, const EuroScopeCRadarTargetInterface& radarTarget) const
        -> bool
    {
        if (!this->AssignmentPossible(flightPlan, radarTarget)) {
            return false;
        }

        return this->GeneralAssignmentNeeded(flightPlan, radarTarget) &&
               this->storedFlightplans.HasFlightplanForCallsign(flightPlan.GetCallsign()) &&
               this->storedFlightplans.GetFlightplanForCallsign(flightPlan.GetCallsign()).HasPreviouslyAssignedSquawk();
    }

    auto SquawkAssignment::AssignmentPossible(
        const EuroScopeCFlightPlanInterface& flightPlan, const EuroScopeCRadarTargetInterface& radarTarget) const
        -> bool
    {
        // We have to check for all zero's because of how ES handles flightplan events  halfway across Europe.
        return this->activeCallsigns.UserHasCallsign() && flightPlan.GetDistanceFromOrigin() != 0.0 &&
               radarTarget.GetFlightLevel() != 0;
    }
} // namespace UKControllerPlugin::Squawk
