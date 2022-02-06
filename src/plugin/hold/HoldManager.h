#pragma once
#include "CompareHoldingAircraft.h"

namespace UKControllerPlugin {
    namespace Api {
        class ApiInterface;
    } // namespace Api
    namespace Euroscope {
        class EuroscopePluginLoopbackInterface;
        class EuroScopeCFlightPlanInterface;
        class EuroScopeCRadarTargetInterface;
    } // namespace Euroscope
    namespace TaskManager {
        class TaskRunnerInterface;
    } // namespace TaskManager
} // namespace UKControllerPlugin

namespace UKControllerPlugin::Hold {
    class HoldingAircraft;
    struct ProximityHold;

    /*
        A class that manages which aircraft are in which holds
    */
    class HoldManager
    {
        public:
        HoldManager(
            const UKControllerPlugin::Api::ApiInterface& api,
            UKControllerPlugin::TaskManager::TaskRunnerInterface& taskRunner);
        void AddAircraftToProximityHold(const std::shared_ptr<ProximityHold>& hold);
        void AssignAircraftToHold(const std::string& callsign, const std::string& hold, bool updateApi);
        size_t CountHoldingAircraft() const;
        const std::set<std::shared_ptr<HoldingAircraft>, CompareHoldingAircraft>&
        GetAircraftForHold(const std::string& hold) const;
        const std::shared_ptr<HoldingAircraft>& GetHoldingAircraft(const std::string& callsign);
        void UnassignAircraftFromHold(const std::string& callsign, bool updateApi);
        void RemoveAircraftFromProximityHold(const std::string& callsign, const std::string& hold);

        const std::shared_ptr<HoldingAircraft> invalidAircraft = nullptr;

        private:
        // Api for syncing with the rest of the controllers
        const UKControllerPlugin::Api::ApiInterface& api;

        // For running async tasks
        UKControllerPlugin::TaskManager::TaskRunnerInterface& taskRunner;

        // All the possible holds
        std::map<std::string, std::set<std::shared_ptr<HoldingAircraft>, CompareHoldingAircraft>> holds;

        // The hold to return if nobody is holding there
        const std::set<std::shared_ptr<HoldingAircraft>, CompareHoldingAircraft> invalidHolds;

        // The aircraft in the holds
        std::set<std::shared_ptr<HoldingAircraft>, CompareHoldingAircraft> aircraft;

        // A mutex for async access
        std::mutex dataMutex;
    };
} // namespace UKControllerPlugin::Hold
