#pragma once
#include "flightplan/FlightPlanEventHandlerInterface.h"
#include "airfield/NormaliseSid.h"
#include "initialaltitude/InitialAltitudeGenerator.h"
#include "timedevent/DeferredEventHandler.h"

// Forward declarations

namespace UKControllerPlugin {
    namespace Flightplan {
        class FlightPlanEventHandlerInterface;
    }  // namespace Flightplan
    namespace Airfield {
        class AirfieldOwnershipManager;
    }  // namespace Airfield

    namespace Controller {
        class ActiveCallsignCollection;
        class Login;
    }  // namespace Controller
    namespace InitialAltitude {
        class InitialAltitudeGenerator;
    }  // namespace InitialAltitude
    namespace TimedEvent {
        class DeferredEventHandler;
    }  // namespace TimedEvent
    namespace Euroscope {
        class EuroscopePluginLoopbackInterface;
    }  // namespace Euroscope
}  // namespace UKControllerPlugin

namespace UKControllerPlugin {
    namespace InitialAltitude {

        /*
            Class that responds to events related to initial altitudes.
        */
        class InitialAltitudeEventHandler : public UKControllerPlugin::Flightplan::FlightPlanEventHandlerInterface
        {
            public:
                InitialAltitudeEventHandler(
                    const UKControllerPlugin::InitialAltitude::InitialAltitudeGenerator & generator,
                    const UKControllerPlugin::Controller::ActiveCallsignCollection & activeCallsigns,
                    const UKControllerPlugin::Airfield::AirfieldOwnershipManager & airfieldOwnership,
                    const UKControllerPlugin::Controller::Login & login,
                    UKControllerPlugin::TimedEvent::DeferredEventHandler & deferredEvents,
                    UKControllerPlugin::Euroscope::EuroscopePluginLoopbackInterface & plugin
                );
                void FlightPlanEvent(
                    UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface & flightPlan,
                    UKControllerPlugin::Euroscope::EuroScopeCRadarTargetInterface & radarTarget
                ) override;
                ~InitialAltitudeEventHandler(void);
                void FlightPlanDisconnectEvent(
                    UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface & flightPlan
                ) override;
                void ControllerFlightPlanDataEvent(
                    UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface & flightPlan,
                    int dataType
                ) override;

                // The maximum distance from the airfield that an aircraft can be untracked
                // to be considered for an altitude update.
                const double assignmentMaxDistanceFromOrigin = 3.0;

                // Then maximum speed that for assigning initial altitudes.
                const int assignmentMaxSpeed = 40;

                // How long we should wait after logging in before assigning
                const std::chrono::seconds minimumLoginTimeBeforeAssignment;

            private:
                bool MeetsAssignmentConditions(
                    UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface & flightPlan,
                    UKControllerPlugin::Euroscope::EuroScopeCRadarTargetInterface & radarTarget
                );

                // Used to generate initial altitudes.
                const UKControllerPlugin::InitialAltitude::InitialAltitudeGenerator & generator;

                // Used to find out the users callsign.
                const UKControllerPlugin::Controller::ActiveCallsignCollection & activeCallsigns;

                // Used to find out if the user owns a particular airfield.
                const UKControllerPlugin::Airfield::AirfieldOwnershipManager & airfieldOwnership;

                // So we can defer loading IAs on first login
                UKControllerPlugin::TimedEvent::DeferredEventHandler & deferredEvents;

                // For checking how long we've been logged in
                const UKControllerPlugin::Controller::Login & login;

                // Class for parsing SIDs and removing deprecation warnings.
                const UKControllerPlugin::Airfield::NormaliseSid normalise;

                // So we can get flightplans after deferred events
                UKControllerPlugin::Euroscope::EuroscopePluginLoopbackInterface & plugin;
        };
    }  // namespace InitialAltitude
}  // namespace UKControllerPlugin
