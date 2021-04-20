#pragma once
#include "flightplan/FlightPlanEventHandlerInterface.h"
#include "airfield/NormaliseSid.h"
#include "timedevent/DeferredEventHandler.h"
#include "euroscope/UserSettingAwareInterface.h"
#include "controller/ActiveCallsignEventHandlerInterface.h"
#include "flightplan/StoredFlightplanCollection.h"

// Forward declarations

namespace UKControllerPlugin {
    namespace Flightplan {
        class FlightPlanEventHandlerInterface;
    }  // namespace Flightplan
    namespace Ownership {
        class AirfieldOwnershipManager;
    }  // namespace Ownership

    namespace Controller {
        class ActiveCallsignCollection;
        class Login;
    }  // namespace Controller
    namespace Sid {
        class SidCollection;
        class StandardInstrumentDeparture;
    } // namespace Sid
    namespace TimedEvent {
        class DeferredEventHandler;
    }  // namespace TimedEvent
    namespace Euroscope {
        class EuroscopePluginLoopbackInterface;
    }  // namespace Euroscope
}  // namespace UKControllerPlugin

namespace UKControllerPlugin {
    namespace InitialHeading {

        /*
            Class that responds to events related to initial headings.
        */
        class InitialHeadingEventHandler : public Flightplan::FlightPlanEventHandlerInterface,
                                           public Euroscope::UserSettingAwareInterface,
                                           public Controller::ActiveCallsignEventHandlerInterface
        {
            public:
                InitialHeadingEventHandler(
                    const Sid::SidCollection& sids,
                    const Controller::ActiveCallsignCollection& activeCallsigns,
                    const Ownership::AirfieldOwnershipManager& airfieldOwnership,
                    const Controller::Login& login,
                    TimedEvent::DeferredEventHandler& deferredEvents,
                    Euroscope::EuroscopePluginLoopbackInterface& plugin,
                    const Flightplan::StoredFlightplanCollection& storedFlightplans
                );
                void FlightPlanEvent(
                    Euroscope::EuroScopeCFlightPlanInterface& flightPlan,
                    Euroscope::EuroScopeCRadarTargetInterface& radarTarget
                ) override;
                ~InitialHeadingEventHandler(void);
                void FlightPlanDisconnectEvent(
                    Euroscope::EuroScopeCFlightPlanInterface& flightPlan
                ) override;
                void ControllerFlightPlanDataEvent(
                    Euroscope::EuroScopeCFlightPlanInterface& flightPlan,
                    int dataType
                ) override;
                void RecycleInitialHeading(
                    Euroscope::EuroScopeCFlightPlanInterface& flightPlan,
                    Euroscope::EuroScopeCRadarTargetInterface& radarTarget,
                    std::string context,
                    const POINT & mousePos
                );
                bool UserAutomaticAssignmentsAllowed(void) const;
                void UserSettingsUpdated(Euroscope::UserSetting& userSettings) override;

                // Inherited via ActiveCallsignEventHandlerInterface
                void ActiveCallsignAdded(
                    const Controller::ActiveCallsign& callsign,
                    bool userCallsign
                ) override;
                void ActiveCallsignRemoved(
                    const Controller::ActiveCallsign& callsign,
                    bool userCallsign
                ) override;
                void CallsignsFlushed(void) override;

                // The maximum distance from the airfield that an aircraft can be untracked
                // to be considered for an heading update.
                const double assignmentMaxDistanceFromOrigin = 3.0;

                // Then maximum speed that for assigning initial headings.
                const int assignmentMaxSpeed = 40;

                // Then current altitude assigning initial headings.
                const int assignmentMaxAltitude = 1000;

                // How long we should wait after logging in before assigning
                const std::chrono::seconds minimumLoginTimeBeforeAssignment;

            private:
                bool MeetsAssignmentConditions(
                    Euroscope::EuroScopeCFlightPlanInterface& flightPlan,
                    Euroscope::EuroScopeCRadarTargetInterface& radarTarget
                ) const;

                static bool MeetsForceAssignmentConditions(
                    Euroscope::EuroScopeCFlightPlanInterface& flightplan,
                    Euroscope::EuroScopeCRadarTargetInterface& radarTarget
                );

                std::shared_ptr<Sid::StandardInstrumentDeparture> GetSidForFlight(
                    Euroscope::EuroScopeCFlightPlanInterface& flightplan
                );

                // Used to generate initial headings.
                const Sid::SidCollection& sids;

                // Used to find out the users callsign.
                const Controller::ActiveCallsignCollection& activeCallsigns;

                // Used to find out if the user owns a particular airfield.
                const Ownership::AirfieldOwnershipManager& airfieldOwnership;

                // So we can defer loading IAs on first login
                TimedEvent::DeferredEventHandler& deferredEvents;

                // For checking how long we've been logged in
                const Controller::Login& login;

                // Class for parsing SIDs and removing deprecation warnings.
                const Airfield::NormaliseSid normalise;

                // Stored flightplans
                const Flightplan::StoredFlightplanCollection& storedFlightplans;

                // So we can get flightplans after deferred events
                Euroscope::EuroscopePluginLoopbackInterface& plugin;

                // Has the user enabled automatic assignments
                bool userAutomaticAssignmentsAllowed = true;

                // Map of callsigns vs which SID their heading has been set for
                std::map<std::string, std::string> alreadySetMap;
        };
    } // namespace InitialHeading
}  // namespace UKControllerPlugin
