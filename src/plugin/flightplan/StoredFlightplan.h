#pragma once
#include "euroscope/EuroScopeCFlightPlanInterface.h"

namespace UKControllerPlugin {
    namespace Flightplan {

        /*
            A class that represents, in the broadest sense, a flightplan.
            Does not contain most of the information that EuroScope can provide us,
            just enough to perform functions such as squawk assignment - and also to provide
            a copy for retention if an aircraft logs off. Unlike EuroScopes flightplans, this can persist
            outside the scope of a single function call.
        */
        class StoredFlightplan
        {
            public:
                explicit StoredFlightplan(
                    const UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface & euroscopePlan
                );
                StoredFlightplan(std::string callsign, std::string origin, std::string destination);
                std::chrono::system_clock::time_point GetActualOffBlockTime(void) const;
                std::string GetCallsign(void) const;
                std::string GetDestination(void) const;
                std::chrono::system_clock::time_point GetEstimatedDepartureTime(void) const;
                std::chrono::system_clock::time_point GetExpectedOffBlockTime(void) const;
                std::string GetOrigin(void) const;
                std::string GetPreviouslyAssignedSquawk(void) const;
                std::time_t GetTimeout(void) const;
                bool HasPreviouslyAssignedSquawk(void) const;
                bool HasTimedOut(void) const;
                void SetActualOffBlockTime(std::chrono::system_clock::time_point time);
                void SetCallsign(std::string callsign);
                void SetDestination(std::string destination);
                void SetEstimatedDepartureTime(std::chrono::system_clock::time_point time);
                void SetOrigin(std::string origin);
                void SetPreviouslyAssignedSquawk(std::string squawk);
                void SetTimeout(int offset);
                void ResetTimeout(void);
                bool operator==(const StoredFlightplan & compare) const;
                bool operator==(const UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface & compare) const;
                bool operator!=(const UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface & compare) const;
                bool operator!=(const StoredFlightplan & compare) const;
                void operator=(const UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface & euroscopePlan);
                void operator=(const StoredFlightplan & flightplan);

                // The squawk code assigned if none allocated by the plugin.
                static const std::string noSquawkAllocated;

                // Default time, when no timeout is set.
                const std::time_t defaultTime = 0;

            private:

                // The callsign for the aircraft
                std::string callsign;

                // The origin airport
                std::string origin;

                // The destination airport
                std::string destination;

                // The time the flightplan is deemed to "time out"
                std::time_t timeout;

                // The squawk code that has been previously assigned to this flightplan
                std::string assignedSquawk;

                // The expected off block time
                std::chrono::system_clock::time_point expectedOffBlockTime =
                    (std::chrono::system_clock::time_point::max)();

                // The estimated departure time
                std::chrono::system_clock::time_point estimatedDepartureTime =
                    (std::chrono::system_clock::time_point::max)();

                // The time at which the aircraft was reported to have left the blocks
                std::chrono::system_clock::time_point actualOffBlockTime =
                    (std::chrono::system_clock::time_point::max)();
        };
    }  // namespace Flightplan
}  // namespace UKControllerPlugin
