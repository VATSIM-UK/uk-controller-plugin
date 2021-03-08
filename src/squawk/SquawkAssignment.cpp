#include "pch/stdafx.h"
#include "squawk/SquawkAssignment.h"
#include "ownership/AirfieldOwnershipManager.h"
#include "controller/ControllerPosition.h"
#include "flightplan/StoredFlightplan.h"
#include "euroscope/EuroscopePluginLoopbackInterface.h"
#include "controller/ActiveCallsign.h"
#include "euroscope/EuroScopeCFlightPlanInterface.h"
#include "euroscope/EuroScopeCRadarTargetInterface.h"
#include "euroscope/EuroScopeCControllerInterface.h"
#include "controller/ActiveCallsignCollection.h"
#include "flightplan/StoredFlightplanCollection.h"

using UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface;
using UKControllerPlugin::Euroscope::EuroScopeCRadarTargetInterface;
using UKControllerPlugin::Euroscope::EuroScopeCControllerInterface;
using UKControllerPlugin::Controller::ActiveCallsign;
using UKControllerPlugin::Ownership::AirfieldOwnershipManager;
using UKControllerPlugin::Euroscope::EuroscopePluginLoopbackInterface;
using UKControllerPlugin::Flightplan::StoredFlightplanCollection;
using UKControllerPlugin::Controller::ControllerPosition;
using UKControllerPlugin::Controller::ActiveCallsignCollection;

namespace UKControllerPlugin {
    namespace Squawk {


        SquawkAssignment::SquawkAssignment(
            const StoredFlightplanCollection & storedFlightplans,
            EuroscopePluginLoopbackInterface & plugin,
            const AirfieldOwnershipManager & airfieldOwnership,
            const ActiveCallsignCollection & activeCallsigns,
            const bool disabled
        )
            : storedFlightplans(storedFlightplans), plugin(plugin), airfieldOwnership(airfieldOwnership),
            activeCallsigns(activeCallsigns), disabled(disabled)
        {

        }

        /*
            Returns whether or not the aircraft is believed to be doing VFR circuits
        */
        bool SquawkAssignment::CircuitAssignmentNeeded(
            EuroScopeCFlightPlanInterface & flightplan,
            EuroScopeCRadarTargetInterface & radarTarget
        ) const
        {
            return flightplan.IsVfr() &&
                !flightplan.HasAssignedSquawk() &&
                std::regex_search(
                    flightplan.GetRawRouteString(),
                    std::regex("^(?:VFR )?CIRCUIT(?:S)?", std::regex::icase)
                ) &&
                this->GeneralAssignmentNeeded(flightplan, radarTarget);
        }

        /*
            Returns whether or not the controller is in a position that they may force squawk assignment.
        */
        bool SquawkAssignment::ForceAssignmentAllowed(EuroScopeCFlightPlanInterface & flightplan) const
        {
            std::shared_ptr<EuroScopeCControllerInterface> myController =
                this->plugin.GetUserControllerObject();

            return myController != nullptr && myController->IsVatsimRecognisedController()
                && (flightplan.IsTrackedByUser() || !flightplan.IsTracked());
        }

        /*
            Returns whether or not a squawk assignment should be forced.
        */
        bool SquawkAssignment::ForceAssignmentNeeded(EuroScopeCFlightPlanInterface & flightplan) const
        {
            return this->storedFlightplans.HasFlightplanForCallsign(flightplan.GetCallsign()) &&
                this->storedFlightplans.GetFlightplanForCallsign(flightplan.GetCallsign()) != flightplan;
        }

        /*
            Returns true if a forced assignment is required or no squawk is assigned.
        */
        bool SquawkAssignment::ForceAssignmentOrNoSquawkAssigned(EuroScopeCFlightPlanInterface & flightplan) const
        {
            return !flightplan.HasAssignedSquawk() || this->ForceAssignmentNeeded(flightplan);
        }

        /*
            Checks whether or not an assignment is needed for an automated squawk check.
        */
        bool SquawkAssignment::GeneralAssignmentNeeded(
            const EuroScopeCFlightPlanInterface & flightPlan,
            const EuroScopeCRadarTargetInterface & radarTarget
        ) const
        {
            if (!this->AssignmentPossible(flightPlan, radarTarget)) {
                return false;
            }

            if (!flightPlan.IsTrackedByUser()) {
                return radarTarget.GetGroundSpeed() <= this->untrackedMaxAssignmentSpeed &&
                    flightPlan.GetDistanceFromOrigin() <= this->untrackedMaxAssignmentDistanceFromOrigin &&
                    !flightPlan.HasAssignedSquawk() &&
                    this->airfieldOwnership.AirfieldOwnedBy(
                        flightPlan.GetOrigin(),
                        this->activeCallsigns.GetUserCallsign()
                    );
            }

            return !flightPlan.HasAssignedSquawk();
        }

        /*
            Returns whether or not a local squawk needs to be assigned.
        */
        bool SquawkAssignment::LocalAssignmentNeeded(
            const EuroScopeCFlightPlanInterface & flightPlan,
            const EuroScopeCRadarTargetInterface & radarTarget
        ) const
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
        bool SquawkAssignment::NeedsLocalSquawkTracked(
            const EuroScopeCFlightPlanInterface & flightPlan,
            const EuroScopeCRadarTargetInterface & radarTarget
        ) const
        {
            if (this->activeCallsigns.GetUserCallsign().GetNormalisedPosition().GetType() == "CTR") {
                return !flightPlan.HasAssignedSquawk() && flightPlan.GetCruiseLevel() <= this->maxAssignmentAltitude;
            }

            // If the controller is APP/TWR, their vis centre should be on the airfield.
            return !flightPlan.HasAssignedSquawk() &&
                radarTarget.GetFlightLevel() <= this->maxAssignmentAltitude &&
                this->plugin.GetDistanceFromUserVisibilityCentre(radarTarget.GetPosition()) <= this->trackedLarsRadius;
        }

        /*
            Whether or not an untracked aircraft require a local squawk.
        */
        bool SquawkAssignment::NeedsLocalSquawkUntracked(
            const EuroScopeCFlightPlanInterface & flightPlan,
            const EuroScopeCRadarTargetInterface & radarTarget
        ) const
        {
            return (
                flightPlan.GetOrigin() == flightPlan.GetDestination() ||
                flightPlan.GetFlightRules() == "V" ||
                (!flightPlan.HasSid() && flightPlan.GetCruiseLevel() <= this->maxAssignmentAltitude)
                ) &&
                radarTarget.GetGroundSpeed() <= this->untrackedMaxAssignmentSpeed &&
                flightPlan.GetDistanceFromOrigin() <= this->untrackedMaxAssignmentDistanceFromOrigin &&
                this->airfieldOwnership.AirfieldOwnedBy(
                    flightPlan.GetOrigin(), this->activeCallsigns.GetUserCallsign()
                ) &&
                !flightPlan.HasAssignedSquawk();
        }

        /*
            Returns true if a previously stored flightplan for a given callsign has an assigned squawk.
            For example, if they disconnect for an extended period.
        */
        bool SquawkAssignment::PreviousSquawkNeedsReassignment(
            const EuroScopeCFlightPlanInterface & flightPlan,
            const EuroScopeCRadarTargetInterface & radarTarget
        ) const
        {
            if (!this->AssignmentPossible(flightPlan, radarTarget)) {
                return false;
            }

            return this->GeneralAssignmentNeeded(flightPlan, radarTarget) &&
                this->storedFlightplans.HasFlightplanForCallsign(flightPlan.GetCallsign()) &&
                this->storedFlightplans.GetFlightplanForCallsign(flightPlan.GetCallsign())
                    .HasPreviouslyAssignedSquawk();
        }

        bool SquawkAssignment::AssignmentPossible(
            const EuroScopeCFlightPlanInterface& flightPlan,
            const EuroScopeCRadarTargetInterface& radarTarget) const
        {
            // We have to check for all zero's because of how ES handles flightplan events  halfway across Europe.
            return this->activeCallsigns.UserHasCallsign() &&
                flightPlan.GetDistanceFromOrigin() != 0.0 &&
                radarTarget.GetFlightLevel() != 0;
        }
    }  // namespace Squawk
}  // namespace UKControllerPlugin
