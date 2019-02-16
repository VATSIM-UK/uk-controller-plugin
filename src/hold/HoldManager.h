#pragma once
#include "pch/stdafx.h"
#include "hold/HoldManager.h"
#include "hold/HoldingAircraft.h"
#include "hold/HoldingData.h"
#include "hold/CompareHoldingAircraft.h"
#include "hold/ManagedHold.h"

namespace UKControllerPlugin {
    namespace Euroscope {
        class EuroscopePluginLoopbackInterface;
        class EuroScopeCFlightPlanInterface;
        class EuroScopeCRadarTargetInterface;
    }  // namespace Euroscope
}  // namespace UKControllerPlugin

namespace UKControllerPlugin {
    namespace Hold {

        /*
            A class that manages which aircraft are in which holds
        */
        class HoldManager
        {
            public:

                HoldManager(void);
                void AddHold(UKControllerPlugin::Hold::ManagedHold hold);
                void AddAircraftToHold(
                    UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface & flightplan,
                    UKControllerPlugin::Euroscope::EuroScopeCRadarTargetInterface & radarTarget,
                    unsigned int holdId
                );
                size_t CountHolds(void) const;
                UKControllerPlugin::Hold::ManagedHold * const GetManagedHold(unsigned int holdId) const;
                void RemoveAircraftFromAnyHold(std::string callsign);
                void HoldManager::UpdateHoldingAircraft(
                    UKControllerPlugin::Euroscope::EuroscopePluginLoopbackInterface & plugin
                );

            private:

                // A map of aircraft callsign -> hold id
                std::map<std::string, unsigned int> holdingAircraft;

                // A map of hold id -> managed hold
                std::map<unsigned int, std::unique_ptr<UKControllerPlugin::Hold::ManagedHold>> holdData;
        };

    }  // namespace Hold
}  // namespace UKControllerPlugin
