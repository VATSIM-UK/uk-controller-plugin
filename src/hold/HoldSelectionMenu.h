#pragma once

namespace UKControllerPlugin {
    namespace Euroscope {
        class EuroscopePluginLoopbackInterface;
        class EuroScopeCFlightPlanInterface;
        class EuroScopeCRadarTargetInterface;
    }  // namespace Euroscope
    namespace Hold {
        class HoldManager;
        class HoldProfileManager;
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
                    UKControllerPlugin::Hold::HoldProfileManager & profileManager,
                    UKControllerPlugin::Euroscope::EuroscopePluginLoopbackInterface & plugin,
                    unsigned int callbackIdFirstHold
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
                const unsigned int callbackIdFirstHold;

            private:

                // A set of all the open ASR's display managers.
                std::set<std::shared_ptr<const UKControllerPlugin::Hold::HoldDisplayManager>> displays;

                // Manages hold profiles
                UKControllerPlugin::Hold::HoldProfileManager & profileManager;

                // The hold manager
                UKControllerPlugin::Hold::HoldManager & holdManager;

                // The plugin, used to display menus
                UKControllerPlugin::Euroscope::EuroscopePluginLoopbackInterface & plugin;
        };

    }  // namespace Hold
}  // namespace UKControllerPlugin
