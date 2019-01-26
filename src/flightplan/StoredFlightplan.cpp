#include "pch/stdafx.h"
#include "flightplan/StoredFlightplan.h"
#include "squawk/SquawkValidator.h"
#include "helper/HelperFunctions.h"

using UKControllerPlugin::Squawk::SquawkValidator;
using UKControllerPlugin::HelperFunctions;

namespace UKControllerPlugin {
    namespace Flightplan {

        const std::string StoredFlightplan::noSquawkAllocated = "0000";

        StoredFlightplan::StoredFlightplan(
            const UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface & euroscopePlan
        ) :timeout(this->defaultTime)
        {
            this->callsign = euroscopePlan.GetCallsign();
            this->origin = euroscopePlan.GetOrigin();
            this->destination = euroscopePlan.GetDestination();
            this->assignedSquawk = euroscopePlan.HasAssignedSquawk() ? euroscopePlan.GetAssignedSquawk()
                : StoredFlightplan::noSquawkAllocated;
            std::chrono::system_clock::time_point edt = HelperFunctions::GetTimeFromNumberString(
                euroscopePlan.GetExpectedDepartureTime()
            );

            if (edt != (std::chrono::system_clock::time_point::max)()) {
                this->expectedOffBlockTime = HelperFunctions::GetTimeFromNumberString(
                    euroscopePlan.GetExpectedDepartureTime()
                ) - std::chrono::minutes(15);
            } else {
                this->expectedOffBlockTime = edt;
            }
        }

        StoredFlightplan::StoredFlightplan(std::string callsign, std::string origin, std::string destination)
            :timeout(this->defaultTime)
        {
            this->callsign = callsign;
            this->origin = origin;
            this->destination = destination;
            this->assignedSquawk = StoredFlightplan::noSquawkAllocated;
        }

        /*
            Returns the actual off block time of the flightplan.
        */
        std::chrono::system_clock::time_point StoredFlightplan::GetActualOffBlockTime(void) const
        {
            return this->actualOffBlockTime;
        }

        /*
            Returns the callsign associated with the flightplan.
        */
        std::string StoredFlightplan::GetCallsign(void) const
        {
            return this->callsign;
        }

        /*
            Returns the destination.
        */
        std::string StoredFlightplan::GetDestination(void) const
        {
            return this->destination;
        }

        /*
            Get the estimated departure time
        */
        std::chrono::system_clock::time_point StoredFlightplan::GetEstimatedDepartureTime(void) const
        {
            return this->estimatedDepartureTime;
        }

        /*
            Get the system calculated estimated off block time
        */
        std::chrono::system_clock::time_point StoredFlightplan::GetExpectedOffBlockTime(void) const
        {
            return this->expectedOffBlockTime;
        }

        /*
            Returns the origin.
        */
        std::string StoredFlightplan::GetOrigin(void) const
        {
            return this->origin;
        }

        /*
            Returns the previously assigned squawk code.
        */
        std::string StoredFlightplan::GetPreviouslyAssignedSquawk(void) const
        {
            return this->assignedSquawk;
        }

        /*
            Returns the time at which we say the flightplan has timed out.
        */
        std::time_t StoredFlightplan::GetTimeout(void) const
        {
            return this->timeout;
        }

        /*
            Returns whether or not the plugin has assigned a squawk for this aircraft.
        */
        bool StoredFlightplan::HasPreviouslyAssignedSquawk(void) const
        {
            return this->assignedSquawk != this->noSquawkAllocated;
        }

        /*
            Returns true if the flightplan has "timed out", false otherwise.
        */
        bool StoredFlightplan::HasTimedOut(void) const
        {
            time_t now = time(0);
            return this->timeout != this->defaultTime && this->timeout < time(0);
        }

        /*
            Sets the off block time
        */
        void StoredFlightplan::SetActualOffBlockTime(std::chrono::system_clock::time_point time)
        {
            this->actualOffBlockTime = time;
        }

        /*
            Sets the callsign.
        */
        void StoredFlightplan::SetCallsign(std::string callsign)
        {
            this->callsign = callsign;
        }

        /*
            Sets the destination.
        */
        void StoredFlightplan::SetDestination(std::string destination)
        {
            this->destination = destination;
        }

        /*
            Set the estimated departure time
        */
        void StoredFlightplan::SetEstimatedDepartureTime(std::chrono::system_clock::time_point time)
        {
            this->estimatedDepartureTime = time;
        }

        /*
            Sets the origin.
        */
        void StoredFlightplan::SetOrigin(std::string origin)
        {
            this->origin = origin;
        }

        /*
            Sets the previously assigned squawk for the flightplan
        */
        void StoredFlightplan::SetPreviouslyAssignedSquawk(std::string squawk)
        {
            this->assignedSquawk = squawk;
        }

        /*
            Sets the time at which we say the flightplan has timed out.
        */
        void StoredFlightplan::SetTimeout(int offset)
        {
            this->timeout = time(0) + offset;
        }

        /*
            Unsets the timeout.
        */
        void StoredFlightplan::ResetTimeout(void)
        {
            this->timeout = this->defaultTime;
        }

        /*
            Returns true only if the callsign, origin and destination match.
        */
        bool StoredFlightplan::operator==(const StoredFlightplan & compare) const
        {
            return this->callsign == compare.callsign &&
                this->destination == compare.destination &&
                this->origin == compare.origin;
        }

        /*
            Compares the flightplan against a Euroscope counterpart and determines if the two are equal.
        */
        bool StoredFlightplan::operator==(
            const UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface & compare
        ) const {
            return this->callsign == compare.GetCallsign() &&
                this->destination == compare.GetDestination() &&
                this->origin == compare.GetOrigin();
        }

        /*
            Returns the opposite of the equality operator.
        */
        bool StoredFlightplan::operator!=(const StoredFlightplan & compare) const
        {
            return !this->operator==(compare);
        }

        /*
            Returns the opposite of the equality operator.
        */
        bool StoredFlightplan::operator!=(
            const UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface & compare
        ) const {
            return !this->operator==(compare);
        }

        /*
            Assigns this object based on the EuroScope flightplan.
        */
        void StoredFlightplan::operator=(
            const UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface & euroscopePlan
        ) {
            this->callsign = euroscopePlan.GetCallsign();
            this->origin = euroscopePlan.GetOrigin();
            this->destination = euroscopePlan.GetDestination();
        }

        /*
            Assigns this object based on another flightplan
        */
        void StoredFlightplan::operator=(const StoredFlightplan & flightplan)
        {
            this->callsign = flightplan.callsign;
            this->origin = flightplan.origin;
            this->destination = flightplan.destination;
            this->timeout = flightplan.timeout;
            this->assignedSquawk = flightplan.assignedSquawk;
            this->expectedOffBlockTime = flightplan.expectedOffBlockTime;
            this->estimatedDepartureTime = flightplan.estimatedDepartureTime;
        }
    }  // namespace Flightplan
}  // namespace UKControllerPlugin
