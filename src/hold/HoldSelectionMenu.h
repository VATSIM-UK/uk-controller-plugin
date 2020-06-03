#pragma once
#include "api/ApiInterface.h"
#include "task/TaskRunnerInterface.h"

namespace UKControllerPlugin {
    namespace Euroscope {
        class EuroscopePluginLoopbackInterface;
        class EuroScopeCFlightPlanInterface;
        class EuroScopeCRadarTargetInterface;
    }  // namespace Euroscope
    namespace Hold {
        class HoldManager;
        class HoldDisplayManager;
    }  // namespace Hold
}  // namespace UKControllerPlugin

namespace UKControllerPlugin {
    namespace Hold {
        class HoldSelectionMenu
        {
            public:
                HoldSelectionMenu(
                    UKControllerPlugin::Hold::HoldManager & holdManager,
                    UKControllerPlugin::Euroscope::EuroscopePluginLoopbackInterface & plugin,
                    const UKControllerPlugin::Api::ApiInterface& api,
                    UKControllerPlugin::TaskManager::TaskRunnerInterface& taskRunner,
                    unsigned int callbackId
                );
                void AddDisplayManager(
                    const std::shared_ptr<const UKControllerPlugin::Hold::HoldDisplayManager> manager
                );
                size_t CountDisplayManagers(void) const;
                void DisplayMenu(
                    UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface & flightplan,
                    UKControllerPlugin::Euroscope::EuroScopeCRadarTargetInterface & radarTarget,
                    std::string context,
                    const POINT & mousePos
                );
                void MenuItemClicked(int functionId, std::string context);

                // The callback id of the first hold
                const unsigned int callbackId;

                const std::string noHold = "--";

            private:

                // A set of all the open ASR's display managers.
                std::set<std::shared_ptr<const UKControllerPlugin::Hold::HoldDisplayManager>> displays;

                // The hold manager
                UKControllerPlugin::Hold::HoldManager & holdManager;

                // The plugin, used to display menus
                UKControllerPlugin::Euroscope::EuroscopePluginLoopbackInterface & plugin;

                // To run hold assignments
                const UKControllerPlugin::Api::ApiInterface& api;
                
                // To run async API tasks
                UKControllerPlugin::TaskManager::TaskRunnerInterface& taskRunner;
        };

    }  // namespace Hold
}  // namespace UKControllerPlugin
