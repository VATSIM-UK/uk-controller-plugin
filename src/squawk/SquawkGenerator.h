#pragma once
#include "task/TaskRunnerInterface.h"
#include "squawk/SquawkRequest.h"

namespace UKControllerPlugin {
    namespace Api {
        class ApiResponse;
        class ApiInterface;
    }  // namespace Api
    namespace Euroscope {
        class EuroScopeCFlightPlanInterface;
        class EuroScopeCRadarTargetInterface;
        class EuroscopePluginLoopbackInterface;
    }  // namespace Euroscope
    namespace Flightplan {
        class StoredFlightplanCollection;
    }  // namespace Flightplan
    namespace Controller {
        class ActiveCallsignCollection;
    }  // namespace Controller
    namespace Squawk {
        class SquawkAssignment;
    }  // namespace Squawk
}  // namespace UKControllerPlugin

namespace UKControllerPlugin {
    namespace Squawk {

        /*
            Makes the relevant API calls to generate a squawk for an aircraft.
        */
        class SquawkGenerator
        {
            public:
                SquawkGenerator(
                    const UKControllerPlugin::Api::ApiInterface & api,
                    UKControllerPlugin::TaskManager::TaskRunnerInterface * const taskRunner,
                    const UKControllerPlugin::Euroscope::EuroscopePluginLoopbackInterface * const plugin,
                    const UKControllerPlugin::Squawk::SquawkAssignment & assignmentRules,
                    const UKControllerPlugin::Controller::ActiveCallsignCollection & callsigns,
                    const UKControllerPlugin::Flightplan::StoredFlightplanCollection & storedFlightplans
                );
                bool ForceGeneralSquawkForAircraft(
                    UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface & flightplan,
                    UKControllerPlugin::Euroscope::EuroScopeCRadarTargetInterface & radarTarget
                );
                bool ForceLocalSquawkForAircraft(
                    UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface & flightplan,
                    UKControllerPlugin::Euroscope::EuroScopeCRadarTargetInterface & radarTarget
                );
                bool ReassignPreviousSquawkToAircraft(
                    UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface & flightplan,
                    UKControllerPlugin::Euroscope::EuroScopeCRadarTargetInterface & radarTarget
                ) const;
                bool RequestGeneralSquawkForAircraft(
                    UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface & flightplan,
                    UKControllerPlugin::Euroscope::EuroScopeCRadarTargetInterface & radarTarget
                );
                bool RequestLocalSquawkForAircraft(
                    UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface & flightplan,
                    UKControllerPlugin::Euroscope::EuroScopeCRadarTargetInterface & radarTarget
                );

                // The squawk we set when a squawk is being generated
                const std::string PROCESS_SQUAWK = "7000";

            private:

                bool GetSquawkAssignment(std::string callsign) const;
                bool CreateGeneralSquawkAssignment(
                    std::string callsign,
                    std::string origin,
                    std::string destination
                ) const;
                bool CreateLocalSquawkAssignment(
                    std::string callsign,
                    std::string unit,
                    std::string flightRules
                ) const;
                void EndSquawkUpdate(std::string callsign);
                bool StartSquawkUpdate(std::string callsign);

                // Callsigns of logged in controllers
                const UKControllerPlugin::Controller::ActiveCallsignCollection & activeCallsigns;

                // Squawk assignment rules
                const UKControllerPlugin::Squawk::SquawkAssignment & assignmentRules;

                // Data about stored flightplans
                const UKControllerPlugin::Flightplan::StoredFlightplanCollection & storedFlightplans;

                // Communicates with the web API
                const UKControllerPlugin::Api::ApiInterface & api;

                // Runs tasks asynchronously from the rest of the plugin
                UKControllerPlugin::TaskManager::TaskRunnerInterface * const taskRunner;

                // The main plugin, for loopbacks so squawks may be assigned.
                const UKControllerPlugin::Euroscope::EuroscopePluginLoopbackInterface * const plugin;

                // A class for thread-safe tracking of squawk requests
                UKControllerPlugin::Squawk::SquawkRequest squawkRequests;
        };
    }  // namespace Squawk
}  // namespace UKControllerPlugin
