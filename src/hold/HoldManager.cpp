#include "pch/stdafx.h"
#include "hold/HoldManager.h"
#include "euroscope/EuroscopePluginLoopbackInterface.h"
#include "euroscope/EuroScopeCFlightPlanInterface.h"
#include "euroscope//EuroScopeCRadarTargetInterface.h"

using UKControllerPlugin::Euroscope::EuroscopePluginLoopbackInterface;
using UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface;
using UKControllerPlugin::Euroscope::EuroScopeCRadarTargetInterface;

namespace UKControllerPlugin {
    namespace Hold {

        HoldManager::HoldManager()
        {

        }

        /*
            Add a hold to the manager
        */
        void HoldManager::AddHold(std::string hold)
        {
            auto managedHold = this->holdData.find(hold);
            if (managedHold != this->holdData.end()) {
                return;
            }

            this->holdData[hold] = std::set<
                UKControllerPlugin::Hold::HoldingAircraft,
                UKControllerPlugin::Hold::CompareHoldingAircraft
            >();
        }

        /*
            Add an aircraft to the hold, removing from any other holds.
        */
        void HoldManager::AddAircraftToHold(
            EuroScopeCFlightPlanInterface & flightplan,
            EuroScopeCRadarTargetInterface & radarTarget,
            std::string hold
        ) {
            auto managedHold = this->holdData.find(hold);
            if (managedHold == this->holdData.end()) {
                return;
            }

            this->RemoveAircraftFromAnyHold(flightplan.GetCallsign());

            managedHold->second.insert(
                {
                    flightplan.GetCallsign(),
                    hold,
                    radarTarget.GetFlightLevel(),
                    flightplan.GetClearedAltitude(),
                    std::chrono::system_clock::now()
                }
            );
        }

        /*
            Returns true if an aircfraft is in a given hold
        */
        bool HoldManager::AircraftIsInHold(std::string hold, std::string callsign) const
        {
            auto managedHold = this->holdData.find(hold);

            return managedHold != this->holdData.cend() &&
                managedHold->second.find(callsign) != managedHold->second.cend();
        }

        /*
            Returns whether or not a hold is managed
        */
        bool HoldManager::HasHold(std::string hold) const
        {
            return this->holdData.find(hold) != this->holdData.cend();
        }

        /*
            Returns a set of aircraft for a given hold
        */
        std::set<UKControllerPlugin::Hold::HoldingAircraft, UKControllerPlugin::Hold::CompareHoldingAircraft>
            HoldManager::GetAircraftInHold(std::string hold) const
        {
            auto managedHold = this->holdData.find(hold);

            if (managedHold == this->holdData.cend()) {
                return std::set<
                    UKControllerPlugin::Hold::HoldingAircraft,
                    UKControllerPlugin::Hold::CompareHoldingAircraft
                >();
            }

            return managedHold->second;
        }

        /*
            Remove aircarft from any holds
        */
        void HoldManager::RemoveAircraftFromAnyHold(std::string callsign)
        {
            for (
                std::map<
                    std::string,
                    std::set<
                        UKControllerPlugin::Hold::HoldingAircraft,
                        UKControllerPlugin::Hold::CompareHoldingAircraft
                    >
                >::iterator it = this->holdData.begin();
                it != this->holdData.end();
                ++it
            ) {
                auto holdingAircraft = it->second.find(callsign);
                if (holdingAircraft != it->second.end()) {
                    it->second.erase(holdingAircraft);
                    return;
                }
            }
        }

        /*
            Removes a hold
        */
        void HoldManager::RemoveHold(std::string hold)
        {
            this->holdData.erase(hold);
        }

        /*
            Update every aircraftin the holds, namely its cleared level and its actual level
        */
        void HoldManager::UpdateHoldingAircraft(EuroscopePluginLoopbackInterface & plugin)
        {
            // Iterate the active holds
            for (
                std::map<std::string, std::set<HoldingAircraft, CompareHoldingAircraft>>::iterator itHold =
                    this->holdData.begin();
                itHold != this->holdData.end();
                ++itHold
            ) {

                // Iterate the aircraft in the holds
                for (
                    std::set<HoldingAircraft, CompareHoldingAircraft>::iterator itAircraft = itHold->second.begin();
                    itAircraft != itHold->second.end();
                    ++itAircraft
                ) {
                    try {
                        // Update the flightplan
                        itAircraft->clearedAltitude = plugin.GetFlightplanForCallsign(
                            itAircraft->callsign
                        )->GetClearedAltitude();

                        itAircraft->reportedAltitude = plugin.GetRadarTargetForCallsign(
                            itAircraft->callsign
                        )->GetFlightLevel();
                    } catch (std::invalid_argument) {
                        // Cant update, no FP.
                    }
                }
            }
        }
    }  // namespace Hold
}  // namespace UKControllerPlugin
