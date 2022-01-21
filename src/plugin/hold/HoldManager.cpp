#include "HoldingAircraft.h"
#include "HoldManager.h"
#include "api/ApiException.h"
#include "api/ApiInterface.h"
#include "euroscope/EuroScopeCFlightPlanInterface.h"
#include "euroscope/EuroScopeCRadarTargetInterface.h"
#include "euroscope/EuroscopePluginLoopbackInterface.h"
#include "task/TaskRunnerInterface.h"

using UKControllerPlugin::Api::ApiException;
using UKControllerPlugin::Api::ApiInterface;
using UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface;
using UKControllerPlugin::Euroscope::EuroScopeCRadarTargetInterface;
using UKControllerPlugin::Euroscope::EuroscopePluginLoopbackInterface;
using UKControllerPlugin::TaskManager::TaskRunnerInterface;

namespace UKControllerPlugin::Hold {

    HoldManager::HoldManager(const ApiInterface& api, TaskRunnerInterface& taskRunner)
        : api(api), taskRunner(taskRunner)
    {
    }

    /*
        Add an aircraft to a "hold" because it's within proximity to the fix
    */
    void HoldManager::AddAircraftToProximityHold(const std::string& callsign, const std::string& hold)
    {
        std::shared_ptr<HoldingAircraft> holdingAircraft;
        if (this->aircraft.count(callsign) != 0) {
            holdingAircraft = *this->aircraft.find(callsign);
            holdingAircraft->AddProximityHold(hold);
        } else {
            holdingAircraft = std::make_shared<HoldingAircraft>(callsign, std::set<std::string>({hold}));
            this->aircraft.insert(holdingAircraft);
        }

        this->holds[hold].insert(holdingAircraft);
    }

    /*
        Assign an aircraft to a given hold
    */
    void HoldManager::AssignAircraftToHold(const std::string& callsign, const std::string& hold, bool updateApi)
    {

        // Add it to the aircraft list or fetch it if needed
        std::shared_ptr<HoldingAircraft> holdingAircraft;
        if (this->aircraft.count(callsign) != 0) {
            holdingAircraft = *this->aircraft.find(callsign);
        } else {
            holdingAircraft = std::make_shared<HoldingAircraft>(callsign, hold);
            this->aircraft.insert(holdingAircraft);
        }

        // Remove the aircraft from its previous hold entirely if its no longer needed
        std::string previousHold = holdingAircraft->GetAssignedHold();
        holdingAircraft->SetAssignedHold(hold);
        if (previousHold != hold && !holdingAircraft->IsInHold(previousHold)) {
            this->holds[previousHold].erase(holdingAircraft);
        }

        // Add it to the right hold list
        this->holds[hold].insert(holdingAircraft);

        // Sync with the API if we need to
        if (!updateApi) {
            return;
        }

        this->taskRunner.QueueAsynchronousTask([this, callsign, hold]() {
            try {
                this->api.AssignAircraftToHold(callsign, hold);
            } catch (ApiException& api) {
                LogError("Failed to add aircraft to the API hold: " + callsign + "/" + hold);
            }
        });
    }

    /*
        Gets all the aircraft in a hold
    */
    auto HoldManager::GetAircraftForHold(const std::string& hold) const
        -> const std::set<std::shared_ptr<HoldingAircraft>, CompareHoldingAircraft>&
    {
        return this->holds.count(hold) != 0 ? this->holds.find(hold)->second : this->invalidHolds;
    }

    auto HoldManager::GetHoldingAircraft(const std::string& callsign) -> const std::shared_ptr<HoldingAircraft>&
    {
        auto aircraft = this->aircraft.find(callsign);
        return aircraft != this->aircraft.cend() ? *aircraft : this->invalidAircraft;
    }

    /*
        Unassign an aircrafts hold
    */
    void HoldManager::UnassignAircraftFromHold(const std::string& callsign, bool updateApi)
    {
        if (this->aircraft.find(callsign) == this->aircraft.cend()) {
            return;
        }

        auto aircraft = *this->aircraft.find(callsign);
        std::string previousHold = aircraft->GetAssignedHold();

        aircraft->RemoveAssignedHold();

        // Remove the aircraft from its previous hold entirely if its no longer needed
        if (!aircraft->IsInHold(previousHold)) {
            this->holds[previousHold].erase(aircraft);
        }

        // Remove the aircraft from the manager if not holding anywhere
        if (!aircraft->IsInAnyHold()) {
            this->aircraft.erase(aircraft);
        }

        // Sync with the API if we need to
        if (!updateApi) {
            return;
        }

        this->taskRunner.QueueAsynchronousTask([this, callsign]() {
            try {
                this->api.UnassignAircraftHold(callsign);
            } catch (ApiException& api) {
                LogError("Failed to remove aircraft from the API hold: " + callsign);
            }
        });
    }

    /*
        Remove an aircraft from the proximity hold
    */
    void HoldManager::RemoveAircraftFromProximityHold(const std::string& callsign, const std::string& hold)
    {
        if (this->aircraft.find(callsign) == this->aircraft.cend()) {
            return;
        }

        auto aircraft = *this->aircraft.find(callsign);

        aircraft->RemoveProximityHold(hold);

        // Remove the aircraft from its previous hold entirely if its no longer needed
        if (!aircraft->IsInHold(hold)) {
            this->holds[hold].erase(aircraft);
        }

        // Remove the aircraft from the manager if not holding anywhere
        if (!aircraft->IsInAnyHold()) {
            this->aircraft.erase(aircraft);
        }
    }

    auto HoldManager::CountHoldingAircraft() const -> size_t
    {
        return this->aircraft.size();
    }
} // namespace UKControllerPlugin::Hold
