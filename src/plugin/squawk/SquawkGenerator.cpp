#include "ApiSquawkAllocationHandler.h"
#include "SquawkAssignment.h"
#include "SquawkAssignmentDeleteForConspicuityFailedEvent.h"
#include "SquawkGenerator.h"
#include "api/ApiException.h"
#include "api/ApiInterface.h"
#include "api/ApiNotFoundException.h"
#include "eventhandler/EventBus.h"
#include "controller/ActiveCallsign.h"
#include "controller/ActiveCallsignCollection.h"
#include "controller/ControllerPosition.h"
#include "euroscope/EuroScopeCFlightPlanInterface.h"
#include "euroscope/EuroScopeCRadarTargetInterface.h"
#include "flightplan/StoredFlightplan.h"
#include "flightplan/StoredFlightplanCollection.h"
#include "helper/HelperFunctions.h"
#include "log/LoggerFunctions.h"

using UKControllerPlugin::HelperFunctions;
using UKControllerPlugin::Api::ApiException;
using UKControllerPlugin::Api::ApiInterface;
using UKControllerPlugin::Api::ApiNotFoundException;
using UKControllerPlugin::Controller::ControllerPosition;
using UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface;
using UKControllerPlugin::Euroscope::EuroScopeCRadarTargetInterface;
using UKControllerPlugin::Flightplan::StoredFlightplan;
using UKControllerPlugin::Flightplan::StoredFlightplanCollection;
using UKControllerPlugin::Squawk::ApiSquawkAllocation;
using UKControllerPlugin::Squawk::ApiSquawkAllocationHandler;
using UKControllerPlugin::Squawk::SquawkAssignment;
using UKControllerPlugin::TaskManager::TaskRunnerInterface;

namespace UKControllerPlugin::Squawk {

    SquawkGenerator::SquawkGenerator(
        const ApiInterface& api,
        TaskRunnerInterface* const taskRunner,
        const UKControllerPlugin::Squawk::SquawkAssignment& assignmentRules,
        const UKControllerPlugin::Controller::ActiveCallsignCollection& activeCallsigns,
        const UKControllerPlugin::Flightplan::StoredFlightplanCollection& storedFlightplans,
        const std::shared_ptr<ApiSquawkAllocationHandler>& allocations)
        : activeCallsigns(activeCallsigns), assignmentRules(assignmentRules), storedFlightplans(storedFlightplans),
          api(api), taskRunner(taskRunner), allocations(allocations)
    {
    }

    /*
        Assigns circuit squawks where appropriate.
    */
    auto SquawkGenerator::AssignCircuitSquawkForAircraft(
        EuroScopeCFlightPlanInterface& flightplan, EuroScopeCRadarTargetInterface& radarTarget) const -> bool
    {
        if (!assignmentRules.CircuitAssignmentNeeded(flightplan, radarTarget)) {
            return false;
        }

        LogInfo("Assigned circuit squawk to " + flightplan.GetCallsign());
        flightplan.SetSquawk("7010");
        return true;
    }

    /**
     * Removes the API squawk assignment and deletes the local
     */
    auto SquawkGenerator::AssignConspicuitySquawkForAircraft(EuroScopeCFlightPlanInterface& flightplan) -> bool
    {
        if (!this->assignmentRules.AssignConspicuityAllowed(flightplan)) {
            LogWarning("Cannot assign conspicuity squawk to " + flightplan.GetCallsign() + " - not allowed");
            return false;
        }

        const auto currentSquawk = flightplan.GetAssignedSquawk();
        if (!this->StartSquawkUpdate(flightplan)) {
            return false;
        }

        const auto callsign = flightplan.GetCallsign();
        if (storedFlightplans.HasFlightplanForCallsign(callsign)) {
            storedFlightplans.GetFlightplanForCallsign(callsign).SetPreviouslyAssignedSquawk("7000");
        }

        this->taskRunner->QueueAsynchronousTask([this, callsign, currentSquawk]() {
            try {
                this->api.DeleteSquawkAssignment(callsign);
            } catch (const ApiException& e) {
                LogWarning("Cannot delete squawk for " + callsign + " - " + e.what());
                UKControllerPluginUtils::EventHandler::EventBus::Bus()
                    .OnEvent<SquawkAssignmentDeleteForConspicuityFailedEvent>({callsign, currentSquawk});
            }

            this->EndSquawkUpdate(callsign);
        });

        return true;
    }

    /*
        Forces a squawk to be assigned for the given aircraft
    */
    auto SquawkGenerator::ForceGeneralSquawkForAircraft(
        EuroScopeCFlightPlanInterface& flightplan, EuroScopeCRadarTargetInterface& radarTarget) -> bool
    {
        if (!this->assignmentRules.ForceAssignmentAllowed(flightplan)) {
            return false;
        }

        if (!this->StartSquawkUpdate(flightplan)) {
            return false;
        }

        // Request squawk update - copy some flightplan data locally incase it goes away.
        std::string callsign = flightplan.GetCallsign();
        std::string origin = flightplan.GetOrigin();
        std::string destination = flightplan.GetDestination();

        this->taskRunner->QueueAsynchronousTask([this, callsign, origin, destination]() {
            static_cast<void>(this->CreateGeneralSquawkAssignment(callsign, origin, destination));
            this->EndSquawkUpdate(callsign);
        });
        return true;
    }

    /*
        Forces the a local squawk update for the given aircraft.
    */
    auto SquawkGenerator::ForceLocalSquawkForAircraft(
        EuroScopeCFlightPlanInterface& flightplan, EuroScopeCRadarTargetInterface& radarTarget) -> bool
    {
        if (!this->assignmentRules.ForceAssignmentAllowed(flightplan)) {
            return false;
        }

        if (!this->activeCallsigns.UserHasCallsign()) {
            return false;
        }

        if (!this->StartSquawkUpdate(flightplan)) {
            return false;
        }

        // Get some data incase the flightplan goes away
        std::string callsign = flightplan.GetCallsign();
        std::string unit = this->activeCallsigns.GetUserCallsign().GetNormalisedPosition().GetUnit();
        std::string flightRules = flightplan.GetFlightRules();

        // Make the request
        this->taskRunner->QueueAsynchronousTask([this, callsign, unit, flightRules]() {
            static_cast<void>(this->CreateLocalSquawkAssignment(callsign, unit, flightRules));
            this->EndSquawkUpdate(callsign);
        });
        return true;
    }

    /*
        If an aircraft has a previously assigned squawk but it's gone somewhere (e.g. disconnect), reassign it.
    */
    auto SquawkGenerator::ReassignPreviousSquawkToAircraft(
        EuroScopeCFlightPlanInterface& flightplan, EuroScopeCRadarTargetInterface& radarTarget) const -> bool
    {
        if (!this->assignmentRules.PreviousSquawkNeedsReassignment(flightplan, radarTarget)) {
            return false;
        }

        flightplan.SetSquawk(
            this->storedFlightplans.GetFlightplanForCallsign(flightplan.GetCallsign()).GetPreviouslyAssignedSquawk());

        return true;
    }

    /*
        Assign a general squawk (not tied to a particular unit) to a given aircraft. Do this out of sync
        so that we don't hold the plugin up waiting for HTTP requests.
    */
    auto SquawkGenerator::RequestGeneralSquawkForAircraft(
        EuroScopeCFlightPlanInterface& flightplan, EuroScopeCRadarTargetInterface& radarTarget) -> bool
    {
        if (!this->assignmentRules.GeneralAssignmentNeeded(flightplan, radarTarget)) {
            return false;
        }

        if (!this->StartSquawkUpdate(flightplan)) {
            return false;
        }

        // Request squawk update - copy some flightplan data locally incase it goes away.
        std::string callsign = flightplan.GetCallsign();
        std::string origin = flightplan.GetOrigin();
        std::string destination = flightplan.GetDestination();

        // Force update required.
        if (this->assignmentRules.ForceAssignmentNeeded(flightplan)) {
            this->taskRunner->QueueAsynchronousTask([this, callsign, origin, destination]() {
                static_cast<void>(this->CreateGeneralSquawkAssignment(callsign, origin, destination));
                this->EndSquawkUpdate(callsign);
            });
            return true;
        }

        // Search for an existing assignment, create if necessary
        this->taskRunner->QueueAsynchronousTask([this, callsign, origin, destination]() {
            if (!this->GetSquawkAssignment(callsign)) {
                static_cast<void>(this->CreateGeneralSquawkAssignment(callsign, origin, destination));
            }
            this->EndSquawkUpdate(callsign);
        });
        return true;
    }

    /*
        Assign a local squawk (to a particular unit) to a given aircraft. Do this out of sync
        so that we don't hold the plugin up waiting for HTTP requests.
    */
    auto SquawkGenerator::RequestLocalSquawkForAircraft(
        EuroScopeCFlightPlanInterface& flightplan, EuroScopeCRadarTargetInterface& radarTarget) -> bool
    {
        if (!this->assignmentRules.LocalAssignmentNeeded(flightplan, radarTarget)) {
            return false;
        }

        if (!this->StartSquawkUpdate(flightplan)) {
            return false;
        }

        // Get the variables out now, just incase anything goes away.
        std::string callsign = flightplan.GetCallsign();
        std::string unit = this->activeCallsigns.GetUserCallsign().GetNormalisedPosition().GetUnit();
        std::string flightRules = flightplan.GetFlightRules();

        // Check for existing squawk assignment, create if necessary
        this->taskRunner->QueueAsynchronousTask([this, callsign, unit, flightRules]() {
            if (!this->GetSquawkAssignment(callsign)) {
                static_cast<void>(this->CreateLocalSquawkAssignment(callsign, unit, flightRules));
            }
            this->EndSquawkUpdate(callsign);
        });
        return true;
    }

    /*
        Checks for a squawk assignment on the API for the given aircraft. Returns true after assigning the squawk
        if one is found, false otherwise.

        THIS FUNCTION SHOULD ONLY BE USED ON AN ASYNCHRONOUS THREAD.
    */
    auto SquawkGenerator::GetSquawkAssignment(const std::string& callsign) const -> bool
    {
        // Assign the squawk to our aircraft
        try {
            ApiSquawkAllocation allocation = this->api.GetAssignedSquawk(callsign);
            this->allocations->AddAllocationToQueue(allocation);
            LogInfo("Found existing API squawk allocation of " + allocation.squawk + " for " + callsign);
            return true;
        } catch (ApiNotFoundException&) {
            // We don't need to log here, as this is a legitimate thing
            return false;
        } catch (ApiException& exception) {
            LogInfo(
                "Error when searching for sqawk assignement, API threw exception: " + std::string(exception.what()));
            return false;
        }
    }

    /*
        Calls the API to create a new local squawk assignment or force update an existing one.

        THIS FUNCTION SHOULD ONLY BE USED ON AN ASYNCHRONOUS THREAD.
    */
    auto SquawkGenerator::CreateGeneralSquawkAssignment(
        const std::string& callsign, std::string origin, std::string destination) const -> bool
    {
        // Assign the squawk to our aircraft
        try {
            ApiSquawkAllocation allocation =
                this->api.CreateGeneralSquawkAssignment(callsign, std::move(origin), std::move(destination));
            this->allocations->AddAllocationToQueue(allocation);
            LogInfo("API allocated general squawk " + allocation.squawk + " to " + callsign);
            return true;
        } catch (ApiException& exception) {
            LogInfo(
                "Error when create general squawk assignement, API threw exception: " + std::string(exception.what()));
            return false;
        }
    }

    /*
        Calls the API to create a new general squawk assignment or force update an existing one.

        THIS FUNCTION SHOULD ONLY BE USED ON AN ASYNCHRONOUS THREAD.
    */
    auto SquawkGenerator::CreateLocalSquawkAssignment(
        const std::string& callsign, std::string unit, std::string flightRules) const -> bool
    {
        try {
            ApiSquawkAllocation allocation =
                this->api.CreateLocalSquawkAssignment(callsign, std::move(unit), std::move(flightRules));
            this->allocations->AddAllocationToQueue(allocation);
            LogInfo("API allocated local squawk " + allocation.squawk + " to " + callsign);
            return true;
        } catch (ApiException& exception) {
            LogInfo(
                "Error when create general squawk assignement, API threw exception: " + std::string(exception.what()));
            return false;
        }
    }

    /*
        Places a request in progress to prevent duplicate requests
    */
    auto SquawkGenerator::StartSquawkUpdate(EuroScopeCFlightPlanInterface& flightplan) -> bool
    {
        // Lock the requests queue and mark the request as in progress. Set a holding squawk.
        if (!this->squawkRequests.Start(flightplan.GetCallsign())) {
            return false;
        }

        flightplan.SetSquawk(this->PROCESS_SQUAWK);
        return true;
    }

    /*
        End of squawk update, remove the in progress flag
    */
    void SquawkGenerator::EndSquawkUpdate(std::string callsign)
    {
        this->squawkRequests.End(std::move(callsign));
    }
} // namespace UKControllerPlugin::Squawk
