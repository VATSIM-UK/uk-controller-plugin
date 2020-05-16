#pragma once
#include "pch/stdafx.h"
#include "hold/HoldManager.h"
#include "hold/HoldingAircraft.h"
#include "hold/HoldingData.h"
#include "hold/CompareHoldingAircraft.h"
#include "hold/CompareHolds.h"

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
                void AddAircraftToProximityHold(
                    UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface& flightplan,
                    std::string hold
                );
                void AssignAircraftToHold(
                    UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface& flightplan,
                    std::string hold
                );

                const std::set<std::shared_ptr<HoldingAircraft>, CompareHoldingAircraft>&
                    GetAircraftForHold(std::string hold) const;
                void UnassignAircraftFromHold(std::string callsign);
                void RemoveAircraftFromProximityHold(std::string callsign, std::string hold);
                void RemoveAircraftFromAnyHold(std::string callsign);

            private:

                // All the possible holds
                std::map<std::string, std::set<std::shared_ptr<HoldingAircraft>, CompareHoldingAircraft>> holds;

                // The aircraft in the holds
                std::set<std::shared_ptr<HoldingAircraft>, CompareHoldingAircraft> aircraft;
        };

    }  // namespace Hold
}  // namespace UKControllerPlugin
