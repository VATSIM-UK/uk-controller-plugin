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
                void SetHoldManaged(unsigned int holdId);
                void SetHoldUnmanaged(unsigned int holdId);
                void RemoveAircraftFromAnyHold(std::string callsign);
                void HoldManager::UpdateHoldingAircraft(
                    UKControllerPlugin::Euroscope::EuroscopePluginLoopbackInterface & plugin
                );

                // Public type definitions for a custom iterator over the class.
                typedef std::set<unsigned int> ManagedHolds;
                typedef ManagedHolds::const_iterator const_iterator;
                const_iterator cbegin(void) const { return activeHolds.cbegin(); }
                const_iterator cend(void) const { return activeHolds.cend(); }

            private:

                // A map of aircraft callsign -> hold id
                std::map<std::string, unsigned int> holdingAircraft;

                // A map of hold id -> managed hold
                std::map<unsigned int, std::unique_ptr<UKControllerPlugin::Hold::ManagedHold>> holdData;

                // The holds that we're actively managing
                std::set<unsigned int> activeHolds;
        };

    }  // namespace Hold
}  // namespace UKControllerPlugin
