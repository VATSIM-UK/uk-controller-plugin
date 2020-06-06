#pragma once
#include "pch/stdafx.h"
#include "hold/HoldManager.h"
#include "hold/HoldingAircraft.h"
#include "hold/HoldingData.h"
#include "hold/CompareHoldingAircraft.h"
#include "hold/CompareHolds.h"
#include "api/ApiInterface.h"
#include "task/TaskRunnerInterface.h"

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

                HoldManager(
                    const UKControllerPlugin::Api::ApiInterface& api,
                    UKControllerPlugin::TaskManager::TaskRunnerInterface& taskRunner
                );
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
                const std::shared_ptr<HoldingAircraft>& GetHoldingAircraft(std::string callsign);
                void UnassignAircraftFromHold(std::string callsign);
                void RemoveAircraftFromProximityHold(std::string callsign, std::string hold);
                void RemoveAircraftFromAnyHold(std::string callsign);

                const std::shared_ptr<HoldingAircraft> invalidAircraft = nullptr;

            private:

                // For running async tasks
                UKControllerPlugin::TaskManager::TaskRunnerInterface& taskRunner;

                // Api for syncing with the rest of the controllers
                const UKControllerPlugin::Api::ApiInterface& api;

                // All the possible holds
                std::map<std::string, std::set<std::shared_ptr<HoldingAircraft>, CompareHoldingAircraft>> holds;

                // The hold to return if nobody is holding there
                const std::set<std::shared_ptr<HoldingAircraft>, CompareHoldingAircraft> invalidHolds;

                // The aircraft in the holds
                std::set<std::shared_ptr<HoldingAircraft>, CompareHoldingAircraft> aircraft;
        };

    }  // namespace Hold
}  // namespace UKControllerPlugin
