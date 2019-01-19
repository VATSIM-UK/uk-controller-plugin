#pragma once

namespace UKControllerPlugin {
    namespace Euroscope {
        class EuroScopeCFlightPlanInterface;
        class EuroScopeCRadarTargetInterface;
        class EuroscopePluginLoopbackInterface;
    }  // namespace Euroscope
    namespace Controller {
        class ActiveCallsignCollection;
    }  // namespace Controller
    namespace Flightplan {
        class StoredFlightplan;
        class StoredFlightplanCollection;
    }  // namespace Flightplan
    namespace Airfield {
        class AirfieldOwnershipManager;
    }  // namespace Airfield
}  // namespace UKControllerPlugin

namespace UKControllerPlugin {
    namespace Squawk {

        /*
            Class that determines whether or not a squawk should be assigned.
        */
        class SquawkAssignment
        {
            public:
                SquawkAssignment(
                    const UKControllerPlugin::Flightplan::StoredFlightplanCollection & storedFlightplans,
                    UKControllerPlugin::Euroscope::EuroscopePluginLoopbackInterface & plugin,
                    const UKControllerPlugin::Airfield::AirfieldOwnershipManager & airfieldOwnership,
                    const UKControllerPlugin::Controller::ActiveCallsignCollection & activeCallsigns,
                    const bool disabled
                );
                bool CircuitAssignmentNeeded(
                    UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface & flightplan
                );
                bool ForceAssignmentAllowed(
                    UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface & flightplan
                ) const;
                bool ForceAssignmentNeeded(
                    UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface & flightplan
                ) const;
                bool ForceAssignmentOrNoSquawkAssigned(
                    UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface & flightplan
                ) const;
                bool GeneralAssignmentNeeded(
                    const UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface & flightPlan,
                    const UKControllerPlugin::Euroscope::EuroScopeCRadarTargetInterface & radarTarget
                ) const;
                bool LocalAssignmentNeeded(
                    const UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface & flightPlan,
                    const UKControllerPlugin::Euroscope::EuroScopeCRadarTargetInterface & radarTarget
                ) const;
                bool PreviousSquawkNeedsReassignment(
                    const UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface & flightPlan,
                    const UKControllerPlugin::Euroscope::EuroScopeCRadarTargetInterface & radarTarget
                ) const;

                // For untracked aircraft, the maximum speed for which squawks will
                // be assigned - limits to just on the ground.
                const int untrackedMaxAssignmentSpeed = 30;

                // For untracked aircraft, the maximum distance from the origin for which squawks will
                // be assigned - limits to being at the right airport
                const double untrackedMaxAssignmentDistanceFromOrigin = 3.0;

                // For tracked aircraft, the maximum distance from the controllers airfield for
                // which to use a local squawk.
                const double trackedLarsRadius = 40.0;

                // For tracked aircraft, the maximum altitude for which to assign local squawks.
                const int maxAssignmentAltitude = 6000;

                // Whether or not squawk assignment is disabled
                const bool disabled = true;

            private:
                bool NeedsLocalSquawkTracked(
                    const UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface & flightPlan,
                    const UKControllerPlugin::Euroscope::EuroScopeCRadarTargetInterface & radarTarget
                ) const;
                bool NeedsLocalSquawkUntracked(
                    const UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface & flightPlan,
                    const UKControllerPlugin::Euroscope::EuroScopeCRadarTargetInterface & radarTarget
                ) const;

                // The active callsigns
                const UKControllerPlugin::Controller::ActiveCallsignCollection & activeCallsigns;

                // Collection of current and recent flightplans
                const UKControllerPlugin::Flightplan::StoredFlightplanCollection & storedFlightplans;

                // Which controllers own which airfields
                const UKControllerPlugin::Airfield::AirfieldOwnershipManager & airfieldOwnership;

                // Link back to the radar screen, for things like distances
                UKControllerPlugin::Euroscope::EuroscopePluginLoopbackInterface & plugin;
        };
    }  // namespace Squawk
}  // namespace UKControllerPlugin
