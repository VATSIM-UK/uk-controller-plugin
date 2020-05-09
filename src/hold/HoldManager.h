#pragma once
#include "pch/stdafx.h"
#include "hold/HoldManager.h"
#include "hold/HoldingAircraft.h"
#include "hold/HoldingData.h"
#include "hold/CompareHoldingAircraft.h"
#include "hold/CompareHolds.h"
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
                void AddPublishedHold(UKControllerPlugin::Hold::HoldingData hold);
                void AddAircraftToHold(
                    UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface & flightplan,
                    UKControllerPlugin::Euroscope::EuroScopeCRadarTargetInterface & radarTarget,
                    unsigned int holdId
                );
                void AddAircraftToHold(
                    UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface& flightplan,
                    UKControllerPlugin::Euroscope::EuroScopeCRadarTargetInterface& radarTarget,
                    std::string hold
                );
                size_t CountHolds(void) const;
                UKControllerPlugin::Hold::ManagedHold * const GetAircraftHold(std::string callsign) const;
                const UKControllerPlugin::Hold::ManagedHold * const GetManagedHold(unsigned int holdId) const;
                const std::set<HoldingAircraft, CompareHoldingAircraft> GetHoldingAircraft(std::string navaid) const;
                const std::set<UKControllerPlugin::Hold::HoldingData, UKControllerPlugin::Hold::CompareHolds>
                    GetPublishedHolds(std::string navaid) const;
                void RemoveAircraftFromAnyHold(std::string callsign);
                void HoldManager::UpdateHoldingAircraft(
                    UKControllerPlugin::Euroscope::EuroscopePluginLoopbackInterface & plugin
                );

                // The value returned when the aircraft is not holding
                const unsigned int noAircraftHold = 9999999;

            private:

                // A map of aircraft callsign -> hold id
                std::map<std::string, unsigned int> holdingAircraft;
                std::map<std::string, std::set<HoldingAircraft, CompareHoldingAircraft>> holds;
                std::map<std::string, std::string> aircraft;

                // A map of hold id -> managed hold
                std::map<unsigned int, std::unique_ptr<UKControllerPlugin::Hold::ManagedHold>> holdData;

                // All the published holds
                std::map<
                    std::string,
                    std::set<UKControllerPlugin::Hold::HoldingData, UKControllerPlugin::Hold::CompareHolds>
                > publishedHolds;
        };

    }  // namespace Hold
}  // namespace UKControllerPlugin
