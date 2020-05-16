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
                void AddAircrafToProximityHold(
                    UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface& flightplan,
                    std::string hold
                );
                void AssignAircraftToHold(
                    UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface& flightplan,
                    std::string hold
                );
                void UnassignAircraftFromHold(std::string callsign);
                void RemoveAircraftFromProximityHold(std::string callsign, std::string hold);
                void RemoveAircraftFromAnyHold(std::string callsign);

            private:

                // A map of aircraft callsign -> hold id
                std::map<std::string, std::set<std::shared_ptr<HoldingAircraft>, CompareHoldingAircraft>> holds;
                std::set<std::shared_ptr<HoldingAircraft>, CompareHoldingAircraft> aircraft;
        };

    }  // namespace Hold
}  // namespace UKControllerPlugin
