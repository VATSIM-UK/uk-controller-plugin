#pragma once
#include "SquawkGeneratorInterface.h"
#include "SquawkRequest.h"
#include "squawk/SquawkGeneratorInterface.h"
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
    class SquawkGenerator : public SquawkGeneratorInterface
    {
        public:
        SquawkGenerator(
            const UKControllerPlugin::Api::ApiInterface& api,
            UKControllerPlugin::TaskManager::TaskRunnerInterface* taskRunner,
            const UKControllerPlugin::Squawk::SquawkAssignment& assignmentRules,
            const UKControllerPlugin::Controller::ActiveCallsignCollection& callsigns,
            const UKControllerPlugin::Flightplan::StoredFlightplanCollection& storedFlightplans,
            const std::shared_ptr<UKControllerPlugin::Squawk::ApiSquawkAllocationHandler>& allocations);
        auto AssignCircuitSquawkForAircraft(
            UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface& flightplan,
            UKControllerPlugin::Euroscope::EuroScopeCRadarTargetInterface& radarTarget) const -> bool;
        auto DeleteApiSquawkAndSetTo(
            const std::string& squawk, UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface& flightplan)
            -> bool override;
        auto ForceGeneralSquawkForAircraft(
            UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface& flightplan,
            UKControllerPlugin::Euroscope::EuroScopeCRadarTargetInterface& radarTarget) -> bool override;
        auto ForceLocalSquawkForAircraft(
            UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface& flightplan,
            UKControllerPlugin::Euroscope::EuroScopeCRadarTargetInterface& radarTarget) -> bool override;
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
        auto StartSquawkUpdate(
            UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface& flightplan,
            const std::string& processSquawk = "7000") -> bool;

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
    };
} // namespace UKControllerPlugin::Squawk
