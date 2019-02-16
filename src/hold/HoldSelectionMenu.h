#pragma once

namespace UKControllerPlugin {
    namespace Euroscope {
        class EuroscopePluginLoopbackInterface;
        class EuroScopeCFlightPlanInterface;
        class EuroScopeCRadarTargetInterface;
    }  // namespace Euroscope
    namespace Hold {
        class HoldManager;
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
                    unsigned int callbackIdFirstHold
                );
                void AddHoldToMenu(unsigned int holdId);
                size_t CountHolds(void) const;
                void RemoveHoldFromMenu(unsigned int holdId);
                void DisplayMenu(
                    UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface & flightplan,
                    UKControllerPlugin::Euroscope::EuroScopeCRadarTargetInterface & radarTarget,
                    std::string context,
                    const POINT & mousePos
                );
                void MenuItemClicked(void);

                // The callback id of the first hold
                const unsigned int callbackIdFirstHold;

            private:

                // The hold manager
                UKControllerPlugin::Hold::HoldManager & holdManager;
                
                // The holds to include in the menu
                std::set<unsigned int> menuHolds;
                
                // The plugin, used to display menus
                UKControllerPlugin::Euroscope::EuroscopePluginLoopbackInterface & plugin;
        };

    }  // namespace Hold
}  // namespace UKControllerPlugin
