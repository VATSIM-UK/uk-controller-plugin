#include "pch/stdafx.h"
#include "hold/HoldManager.h"
#include "euroscope/EuroScopeCFlightPlanInterface.h"
#include "euroscope/EuroScopeCRadarTargetInterface.h"
#include "euroscope/EuroscopePluginLoopbackInterface.h"
#include "HoldManager.h"

using UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface;
using UKControllerPlugin::Euroscope::EuroScopeCRadarTargetInterface;
using UKControllerPlugin::Euroscope::EuroscopePluginLoopbackInterface;

namespace UKControllerPlugin {
    namespace Hold {

        HoldManager::HoldManager()
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
            std::string hold
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
            return aircraft != this->aircraft.cend() ? *aircraft : nullptr;
        }

        /*
            Unassign an aircrafts hold
        */
        void HoldManager::UnassignAircraftFromHold(std::string callsign)
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

        /*
            Remove aircraft from any holds that they are in, regardless of status
        */
        void HoldManager::RemoveAircraftFromAnyHold(std::string callsign)
        {
            auto aircraft = this->aircraft.find(callsign);
            if (aircraft == this->aircraft.cend()) {
                return;
            }

            const std::set<std::string> proximityHolds = (*aircraft)->GetProximityHolds();
            for (
                std::set<std::string>::const_iterator it = proximityHolds.cbegin();
                it != proximityHolds.cend();
                ++it
            ) {
                this->holds[*it].erase(this->holds[*it].find(callsign));
            }

            this->aircraft.erase(this->aircraft.find(callsign));
        }
    }  // namespace Hold
}  // namespace UKControllerPlugin
