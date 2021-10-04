#pragma once
#include "airfield/NormaliseSid.h"
#include "controller/ActiveCallsignEventHandlerInterface.h"
#include "euroscope/UserSettingAwareInterface.h"
#include "flightplan/FlightPlanEventHandlerInterface.h"
#include "timedevent/AbstractTimedEvent.h"

// Forward declarations

namespace UKControllerPlugin {
    namespace Flightplan {
        class FlightPlanEventHandlerInterface;
    } // namespace Flightplan
    namespace Ownership {
        class AirfieldServiceProviderCollection;
    } // namespace Ownership

    namespace Controller {
        class ActiveCallsignCollection;
        class Login;
    } // namespace Controller
    namespace Sid {
        class SidCollection;
        class StandardInstrumentDeparture;
    } // namespace Sid
    namespace Euroscope {
        class EuroscopePluginLoopbackInterface;
    } // namespace Euroscope
} // namespace UKControllerPlugin

namespace UKControllerPlugin::InitialAltitude {

    /*
        Class that responds to events related to initial altitudes.
    */
    class InitialAltitudeEventHandler : public Flightplan::FlightPlanEventHandlerInterface,
                                        public Euroscope::UserSettingAwareInterface,
                                        public Controller::ActiveCallsignEventHandlerInterface,
                                        public TimedEvent::AbstractTimedEvent
    {
        public:
        InitialAltitudeEventHandler(
            const Sid::SidCollection& sids,
            const Controller::ActiveCallsignCollection& activeCallsigns,
            const Ownership::AirfieldServiceProviderCollection& airfieldOwnership,
            const Controller::Login& login,
            Euroscope::EuroscopePluginLoopbackInterface& plugin);
        void FlightPlanEvent(
            Euroscope::EuroScopeCFlightPlanInterface& flightPlan,
            Euroscope::EuroScopeCRadarTargetInterface& radarTarget) override;
        void FlightPlanDisconnectEvent(Euroscope::EuroScopeCFlightPlanInterface& flightPlan) override;
        void ControllerFlightPlanDataEvent(Euroscope::EuroScopeCFlightPlanInterface& flightPlan, int dataType) override;
        void RecycleInitialAltitude(
            Euroscope::EuroScopeCFlightPlanInterface& flightPlan,
            Euroscope::EuroScopeCRadarTargetInterface& radarTarget,
            const std::string& context,
            const POINT& mousePos);
        [[nodiscard]] auto UserAutomaticAssignmentsAllowed() const -> bool;
        void UserSettingsUpdated(Euroscope::UserSetting& userSettings) override;

        // Inherited via ActiveCallsignEventHandlerInterface
        void ActiveCallsignAdded(const Controller::ActiveCallsign& callsign) override;
        void ActiveCallsignRemoved(const Controller::ActiveCallsign& callsign) override;
        void TimedEventTrigger() override;

        private:
        void CheckAllFlightplansForAssignment();
        auto MeetsAssignmentConditions(
            Euroscope::EuroScopeCFlightPlanInterface& flightPlan,
            Euroscope::EuroScopeCRadarTargetInterface& radarTarget) const -> bool;

        static auto MeetsForceAssignmentConditions(
            Euroscope::EuroScopeCFlightPlanInterface& flightplan,
            Euroscope::EuroScopeCRadarTargetInterface& radarTarget) -> bool;

        auto GetSidForFlight(Euroscope::EuroScopeCFlightPlanInterface& flightplan)
            -> std::shared_ptr<Sid::StandardInstrumentDeparture>;

        // Used to generate initial altitudes.
        const Sid::SidCollection& sids;

        // Used to find out the users callsign.
        const Controller::ActiveCallsignCollection& activeCallsigns;

        // Used to find out if the user owns a particular airfield.
        const Ownership::AirfieldServiceProviderCollection& airfieldOwnership;

        // For checking how long we've been logged in
        const Controller::Login& login;

        // Class for parsing SIDs and removing deprecation warnings.
        const Airfield::NormaliseSid normalise;

        // So we can get flightplans after deferred events
        Euroscope::EuroscopePluginLoopbackInterface& plugin;

        // Has the user enabled automatic assignments
        bool userAutomaticAssignmentsAllowed = true;

        // Map of callsigns vs which SID their altitude has been set for
        std::map<std::string, std::string> alreadySetMap;

        // The maximum distance from the airfield that an aircraft can be untracked
        // to be considered for an altitude update.
        const double assignmentMaxDistanceFromOrigin = 3.0;

        // Then maximum speed that for assigning initial altitudes.
        const int assignmentMaxSpeed = 40;

        // Then current altitude assigning initial altitudes.
        const int assignmentMaxAltitude = 1000;

        // How long we should wait after logging in before assigning
        const std::chrono::seconds minimumLoginTimeBeforeAssignment = std::chrono::seconds(5);
    };
} // namespace UKControllerPlugin::InitialAltitude
