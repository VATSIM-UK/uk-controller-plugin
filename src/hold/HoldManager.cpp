#include "pch/stdafx.h"
#include "hold/HoldManager.h"
#include "euroscope/EuroScopeCFlightPlanInterface.h"
#include "euroscope//EuroScopeCRadarTargetInterface.h"

using UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface;
using UKControllerPlugin::Euroscope::EuroScopeCRadarTargetInterface;

namespace UKControllerPlugin {
    namespace Hold {

        HoldManager::HoldManager()
        {

        }

        /*
            Add a hold to the manager, doesnt add duplicates
        */
        void HoldManager::AddHold(UKControllerPlugin::Hold::ManagedHold hold)
        {
            auto existingHold = std::find_if(
                this->holdData.begin(),
                this->holdData.end(),
                [&hold] (std::pair<unsigned int, const std::unique_ptr<ManagedHold> &> compare) -> bool {
                    return hold.holdParameters == compare.second->holdParameters;
                }
            );
             
            if (existingHold != this->holdData.end()) {
                LogWarning("Tried to add duplicate hold " + std::to_string(hold.holdParameters.identifier));
                return;
            }

            this->holdData[hold.holdParameters.identifier] = std::make_unique<ManagedHold>(hold);
        }

        /*
            Add an aircraft to the hold, removing from any other holds.
        */
        void HoldManager::AddAircraftToHold(
            EuroScopeCFlightPlanInterface & flightplan,
            EuroScopeCRadarTargetInterface & radarTarget,
            unsigned int holdId
        ) {
            auto managedHold = this->holdData.find(holdId);
            if (managedHold == this->holdData.end()) {
                LogWarning("Tried to add aircraft to non existant hold " + std::to_string(holdId));
                return;
            }

            this->RemoveAircraftFromAnyHold(flightplan.GetCallsign());

            managedHold->second->AddHoldingAircraft(
                {
                    flightplan.GetCallsign(),
                    std::chrono::system_clock::now()
                }
            );
            this->holdingAircraft[flightplan.GetCallsign()] = managedHold->first;
        }

        /*
            Count the number of holds
        */
        size_t HoldManager::CountHolds(void) const
        {
            return this->holdData.size();
        }

        /*
            Return one of the managed holds
        */
        UKControllerPlugin::Hold::ManagedHold * const HoldManager::GetManagedHold(unsigned int holdId) const
        {
            auto managedHold = this->holdData.find(holdId);
            if (managedHold == this->holdData.cend()) {
                LogWarning("Tried to access invalid hold " + std::to_string(holdId));
                return nullptr;
            }

            return &(*managedHold->second);
        }

        /*
            Remove aircarft from any holds that they are in
        */
        void HoldManager::RemoveAircraftFromAnyHold(std::string callsign)
        {
            auto aircraft = this->holdingAircraft.find(callsign);
            if (aircraft == this->holdingAircraft.cend()) {
                LogWarning("Tried to remove " + callsign + " from holds, it isnt holding");
                return;
            }

            this->holdData[aircraft->second]->RemoveHoldingAircraft(aircraft->first);
            this->holdingAircraft.erase(aircraft);
        }
    }  // namespace Hold
}  // namespace UKControllerPlugin
