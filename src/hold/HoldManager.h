#pragma once
#include "pch/stdafx.h"
#include "hold/HoldManager.h"
#include "hold/HoldingAircraft.h"
#include "hold/HoldingData.h"
#include "hold/CompareHoldingAircraft.h"

namespace UKControllerPlugin {
    namespace Euroscope {
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
                void AddHold(std::string hold);
                void AddAircraftToHold(
                    UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface & flightplan,
                    UKControllerPlugin::Euroscope::EuroScopeCRadarTargetInterface & radarTarget,
                    std::string hold
                );
                bool AircraftIsInHold(std::string hold, std::string callsign) const;
                bool HasHold(std::string hold) const;
                std::set<UKControllerPlugin::Hold::HoldingAircraft, UKControllerPlugin::Hold::CompareHoldingAircraft>
                    GetAircraftInHold(std::string hold) const;

                void RemoveAircraftFromAnyHold(std::string callsign);
                void RemoveHold(std::string hold);

            private:
                // A map of holds to aircraft
                std::map<
                    std::string,
                    std::set<
                        UKControllerPlugin::Hold::HoldingAircraft,
                        UKControllerPlugin::Hold::CompareHoldingAircraft
                    >
                > holdData;
        };

    }  // namespace Hold
}  // namespace UKControllerPlugin
