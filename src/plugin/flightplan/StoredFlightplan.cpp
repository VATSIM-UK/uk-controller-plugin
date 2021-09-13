#include "StoredFlightplan.h"
#include "euroscope/EuroScopeCFlightPlanInterface.h"
#include "helper/HelperFunctions.h"
#include "squawk/SquawkValidator.h"

using UKControllerPlugin::HelperFunctions;
using UKControllerPlugin::Squawk::SquawkValidator;

namespace UKControllerPlugin::Flightplan {

    StoredFlightplan::StoredFlightplan(
        const UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface& euroscopePlan)
        : timeout(defaultTime)
    {
        this->callsign = euroscopePlan.GetCallsign();
        this->origin = euroscopePlan.GetOrigin();
        this->destination = euroscopePlan.GetDestination();
        this->assignedSquawk =
            euroscopePlan.HasAssignedSquawk() ? euroscopePlan.GetAssignedSquawk() : StoredFlightplan::noSquawkAllocated;
        std::chrono::system_clock::time_point edt =
            HelperFunctions::GetTimeFromNumberString(euroscopePlan.GetExpectedDepartureTime());

        if (edt != (std::chrono::system_clock::time_point::max)()) {
            this->expectedOffBlockTime = edt - std::chrono::minutes(EDT_MINUTES);
        } else {
            this->expectedOffBlockTime = edt;
        }
    }

    StoredFlightplan::StoredFlightplan(std::string callsign, std::string origin, std::string destination)
        : callsign(std::move(callsign)),  origin(std::move(origin)), destination(std::move(destination)),
          timeout(defaultTime), assignedSquawk(noSquawkAllocated)
    {
    }

    StoredFlightplan::~StoredFlightplan() = default;
    StoredFlightplan::StoredFlightplan(const StoredFlightplan&) = default;
    StoredFlightplan::StoredFlightplan(StoredFlightplan&&) noexcept = default;
    auto StoredFlightplan::operator=(StoredFlightplan&&) noexcept -> StoredFlightplan& = default;

    /*
        Returns the actual off block time of the flightplan.
    */
    auto StoredFlightplan::GetActualOffBlockTime() const -> std::chrono::system_clock::time_point
    {
        return this->actualOffBlockTime;
    }

    /*
        Returns the callsign associated with the flightplan.
    */
    auto StoredFlightplan::GetCallsign() const -> std::string
    {
        return this->callsign;
    }

    /*
        Returns the destination.
    */
    auto StoredFlightplan::GetDestination() const -> std::string
    {
        return this->destination;
    }

    /*
        Get the estimated departure time
    */
    auto StoredFlightplan::GetEstimatedDepartureTime() const -> std::chrono::system_clock::time_point
    {
        return this->estimatedDepartureTime;
    }

    /*
        Get the system calculated estimated off block time
    */
    auto StoredFlightplan::GetExpectedOffBlockTime() const -> std::chrono::system_clock::time_point
    {
        return this->expectedOffBlockTime;
    }

    /*
        Returns the origin.
    */
    auto StoredFlightplan::GetOrigin() const -> std::string
    {
        return this->origin;
    }

    /*
        Returns the previously assigned squawk code.
    */
    auto StoredFlightplan::GetPreviouslyAssignedSquawk() const -> std::string
    {
        return this->assignedSquawk;
    }

    /*
        Returns the time at which we say the flightplan has timed out.
    */
    auto StoredFlightplan::GetTimeout() const -> std::time_t
    {
        return this->timeout;
    }

    /*
        Returns whether or not the plugin has assigned a squawk for this aircraft.
    */
    auto StoredFlightplan::HasPreviouslyAssignedSquawk() const -> bool
    {
        return this->assignedSquawk != UKControllerPlugin::Flightplan::StoredFlightplan::noSquawkAllocated;
    }

    /*
        Returns true if the flightplan has "timed out", false otherwise.
    */
    auto StoredFlightplan::HasTimedOut() const -> bool
    {
        return this->timeout != UKControllerPlugin::Flightplan::StoredFlightplan::defaultTime &&
               this->timeout < time(nullptr);
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
        this->callsign = std::move(callsign);
    }

    /*
        Sets the destination.
    */
    void StoredFlightplan::SetDestination(std::string destination)
    {
        this->destination = std::move(destination);
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
        this->origin = std::move(origin);
    }

    /*
        Sets the previously assigned squawk for the flightplan
    */
    void StoredFlightplan::SetPreviouslyAssignedSquawk(std::string squawk)
    {
        this->assignedSquawk = std::move(squawk);
    }

    /*
        Sets the time at which we say the flightplan has timed out.
    */
    void StoredFlightplan::SetTimeout(int offset)
    {
        this->timeout = time(nullptr) + offset;
    }

    /*
        Unsets the timeout.
    */
    void StoredFlightplan::ResetTimeout()
    {
        this->timeout = UKControllerPlugin::Flightplan::StoredFlightplan::defaultTime;
    }

    /*
        Returns true only if the callsign, origin and destination match.
    */
    auto StoredFlightplan::operator==(const StoredFlightplan& compare) const -> bool
    {
        return this->callsign == compare.callsign && this->destination == compare.destination &&
               this->origin == compare.origin;
    }

    /*
        Compares the flightplan against a Euroscope counterpart and determines if the two are equal.
    */
    auto StoredFlightplan::operator==(const UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface& compare) const
        -> bool
    {
        return this->callsign == compare.GetCallsign() && this->destination == compare.GetDestination() &&
               this->origin == compare.GetOrigin();
    }

    /*
        Returns the opposite of the equality operator.
    */
    auto StoredFlightplan::operator!=(const StoredFlightplan& compare) const -> bool
    {
        return !this->operator==(compare);
    }

    /*
        Returns the opposite of the equality operator.
    */
    auto StoredFlightplan::operator!=(const UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface& compare) const
        -> bool
    {
        return !this->operator==(compare);
    }

    /*
        Assigns this object based on the EuroScope flightplan.
    */
    auto StoredFlightplan::operator=(const UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface& euroscopePlan)
        -> StoredFlightplan&
    {
        this->callsign = euroscopePlan.GetCallsign();
        this->origin = euroscopePlan.GetOrigin();
        this->destination = euroscopePlan.GetDestination();
        return *this;
    }

    /*
        Assigns this object based on another flightplan
    */
    auto StoredFlightplan::operator=(const StoredFlightplan& flightplan) -> StoredFlightplan&
    {
        if (this == &flightplan) {
            return *this;
        }

        this->callsign = flightplan.callsign;
        this->origin = flightplan.origin;
        this->destination = flightplan.destination;
        this->timeout = flightplan.timeout;
        this->assignedSquawk = flightplan.assignedSquawk;
        this->expectedOffBlockTime = flightplan.expectedOffBlockTime;
        this->estimatedDepartureTime = flightplan.estimatedDepartureTime;
        return *this;
    }
} // namespace UKControllerPlugin::Flightplan
