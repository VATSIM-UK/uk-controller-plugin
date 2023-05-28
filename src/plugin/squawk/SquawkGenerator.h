#pragma once
#include "SquawkRequest.h"
#include "task/TaskRunnerInterface.h"

namespace UKControllerPlugin {
    namespace Api {
        class ApiResponse;
        class ApiInterface;
    } // namespace Api
    namespace Euroscope {
        class EuroScopeCFlightPlanInterface;
        class EuroScopeCRadarTargetInterface;
    } // namespace Euroscope
    namespace Flightplan {
        class StoredFlightplanCollection;
    } // namespace Flightplan
    namespace Controller {
        class ActiveCallsignCollection;
    } // namespace Controller
    namespace Squawk {
        class SquawkAssignment;
        class ApiSquawkAllocationHandler;
    } // namespace Squawk
} // namespace UKControllerPlugin

namespace UKControllerPlugin::Squawk {

    /*
        Makes the relevant API calls to generate a squawk for an aircraft.
    */
    class SquawkGenerator
    {
        public:
        SquawkGenerator(
            const UKControllerPlugin::Api::ApiInterface& api,
            UKControllerPlugin::TaskManager::TaskRunnerInterface* taskRunner,
            const UKControllerPlugin::Squawk::SquawkAssignment& assignmentRules,
            const UKControllerPlugin::Controller::ActiveCallsignCollection& callsigns,
            const UKControllerPlugin::Flightplan::StoredFlightplanCollection& storedFlightplans,
            const std::shared_ptr<UKControllerPlugin::Squawk::ApiSquawkAllocationHandler>& allocations);
        auto
        AssignConspicuitySquawkForAircraft(UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface& flightplan)
            -> bool;
        auto AssignCircuitSquawkForAircraft(
            UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface& flightplan,
            UKControllerPlugin::Euroscope::EuroScopeCRadarTargetInterface& radarTarget) const -> bool;
        auto ForceGeneralSquawkForAircraft(
            UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface& flightplan,
            UKControllerPlugin::Euroscope::EuroScopeCRadarTargetInterface& radarTarget) -> bool;
        auto ForceLocalSquawkForAircraft(
            UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface& flightplan,
            UKControllerPlugin::Euroscope::EuroScopeCRadarTargetInterface& radarTarget) -> bool;
        auto ReassignPreviousSquawkToAircraft(
            UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface& flightplan,
            UKControllerPlugin::Euroscope::EuroScopeCRadarTargetInterface& radarTarget) const -> bool;
        auto RequestGeneralSquawkForAircraft(
            UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface& flightplan,
            UKControllerPlugin::Euroscope::EuroScopeCRadarTargetInterface& radarTarget) -> bool;
        auto RequestLocalSquawkForAircraft(
            UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface& flightplan,
            UKControllerPlugin::Euroscope::EuroScopeCRadarTargetInterface& radarTarget) -> bool;

        private:
        [[nodiscard]] auto GetSquawkAssignment(const std::string& callsign) const -> bool;
        [[nodiscard]] auto
        CreateGeneralSquawkAssignment(const std::string& callsign, std::string origin, std::string destination) const
            -> bool;
        [[nodiscard]] auto
        CreateLocalSquawkAssignment(const std::string& callsign, std::string unit, std::string flightRules) const
            -> bool;
        void EndSquawkUpdate(std::string callsign);
        auto StartSquawkUpdate(UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface& flightplan) -> bool;

        // Callsigns of logged in controllers
        const UKControllerPlugin::Controller::ActiveCallsignCollection& activeCallsigns;

        // Squawk assignment rules
        const UKControllerPlugin::Squawk::SquawkAssignment& assignmentRules;

        // Data about stored flightplans
        const UKControllerPlugin::Flightplan::StoredFlightplanCollection& storedFlightplans;

        // Communicates with the web API
        const UKControllerPlugin::Api::ApiInterface& api;

        // Runs tasks asynchronously from the rest of the plugin
        UKControllerPlugin::TaskManager::TaskRunnerInterface* const taskRunner;

        // A class for thread-safe tracking of squawk requests
        UKControllerPlugin::Squawk::SquawkRequest squawkRequests;

        // Receives API squawk allocations, so that they may be assigned to flightplans on the main thread
        const std::shared_ptr<UKControllerPlugin::Squawk::ApiSquawkAllocationHandler> allocations;

        // The squawk we set when a squawk is being generated
        const std::string PROCESS_SQUAWK = "7000";
    };
} // namespace UKControllerPlugin::Squawk
