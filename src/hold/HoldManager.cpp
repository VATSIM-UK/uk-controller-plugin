#include "pch/stdafx.h"
#include "hold/HoldManager.h"
#include "euroscope/EuroScopeCFlightPlanInterface.h"
#include "euroscope/EuroScopeCRadarTargetInterface.h"
#include "euroscope/EuroscopePluginLoopbackInterface.h"
#include "HoldManager.h"
#include "api/ApiException.h"

using UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface;
using UKControllerPlugin::Euroscope::EuroScopeCRadarTargetInterface;
using UKControllerPlugin::Euroscope::EuroscopePluginLoopbackInterface;
using UKControllerPlugin::Api::ApiInterface;
using UKControllerPlugin::Api::ApiException;
using UKControllerPlugin::TaskManager::TaskRunnerInterface;

namespace UKControllerPlugin {
    namespace Hold {

        HoldManager::HoldManager(const ApiInterface& api, TaskRunnerInterface& taskRunner)
            : api(api), taskRunner(taskRunner)
        {

        }

        /*
            Add an aircraft to a "hold" because it's within proximity to the fix
        */
        void HoldManager::AddAircraftToProximityHold(EuroScopeCFlightPlanInterface& flightplan, std::string hold)
        {
            std::shared_ptr<HoldingAircraft> holdingAircraft;
            if (this->aircraft.count(flightplan.GetCallsign())) {
                holdingAircraft = *this->aircraft.find(flightplan.GetCallsign());
                holdingAircraft->AddProximityHold(hold);
            } else {
                holdingAircraft = std::make_shared<HoldingAircraft>(
                    flightplan.GetCallsign(),
                    std::set<std::string>({ hold })
                );
                this->aircraft.insert(holdingAircraft);
            }

            this->holds[hold].insert(holdingAircraft);
        }

        /*
            Assign an aircraft to a given hold
        */
        void HoldManager::AssignAircraftToHold(
            EuroScopeCFlightPlanInterface& flightplan,
            std::string hold,
            bool updateApi
        ) {

            // Add it to the aircraft list or fetch it if needed
            std::shared_ptr<HoldingAircraft> holdingAircraft;
            if (this->aircraft.count(flightplan.GetCallsign())) {
                holdingAircraft = *this->aircraft.find(flightplan.GetCallsign());
            } else {
                holdingAircraft = std::make_shared<HoldingAircraft>(
                    flightplan.GetCallsign(),
                    hold
                );
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

            std::string callsign = flightplan.GetCallsign();
            this->taskRunner.QueueAsynchronousTask([this, callsign, hold]() {
                try {
                    this->api.AssignAircraftToHold(callsign, hold);
                }
                catch (ApiException api) {
                    LogError("Failed to add aircraft to the API hold: " + callsign + "/" + hold);
                }
            });
        }

        /*
            Gets all the aircraft in a hold
        */
        const std::set<std::shared_ptr<HoldingAircraft>, CompareHoldingAircraft>&
            HoldManager::GetAircraftForHold(std::string hold) const
        {
            return this->holds.count(hold)
                ? this->holds.find(hold)->second
                : this->invalidHolds;
        }

        const std::shared_ptr<HoldingAircraft>& HoldManager::GetHoldingAircraft(std::string callsign)
        {
            auto aircraft = this->aircraft.find(callsign);
            return aircraft != this->aircraft.cend() ? *aircraft : this->invalidAircraft;
        }

        /*
            Unassign an aircrafts hold
        */
        void HoldManager::UnassignAircraftFromHold(std::string callsign, bool updateApi)
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
                }
                catch (ApiException api) {
                    LogError("Failed to remove aircraft from the API hold: " + callsign);
                }
            });
        }

        /*
            Remove an aircraft from the proximity hold    
        */
        void HoldManager::RemoveAircraftFromProximityHold(std::string callsign, std::string hold)
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

        size_t HoldManager::CountHoldingAircraft(void) const
        {
            return this->aircraft.size();
        }
    }  // namespace Hold
}  // namespace UKControllerPlugin
