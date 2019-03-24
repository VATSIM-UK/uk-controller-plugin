#pragma once
#include "radarscreen/ConfigurableDisplayInterface.h"

namespace UKControllerPlugin {
    namespace Hold {
        class HoldProfileManager;
        class HoldRenderer;
    }  // namespace Hold
    namespace Euroscope {
        class EuroscopePluginLoopbackInterface;
    }  // namespace Euroscope
}  // namespace UKControllerPlugin

namespace UKControllerPlugin {
    namespace Hold {

        /*
            Represents a EuroScope popup menu that selects the hold profile that should be used.
        */
        class HoldProfileSelector : UKControllerPlugin::RadarScreen::ConfigurableDisplayInterface
        {
            public:
                HoldProfileSelector(
                    const UKControllerPlugin::Hold::HoldProfileManager & profileManager,
                    UKControllerPlugin::Euroscope::EuroscopePluginLoopbackInterface & plugin,
                    UKControllerPlugin::Hold::HoldRenderer & renderer,
                    unsigned int selectorMenuOpenCallbackId,
                    unsigned int firstProfileCallbackId
                );

                // Inherited via ConfigurableDisplayInterface
                void Configure(int functionId, std::string subject, RECT screenObjectArea) override;
                UKControllerPlugin::Plugin::PopupMenuItem GetConfigurationMenuItem(void) const override;

                void ItemSelected(int itemId, std::string itemText);

                // What to display in the configuration menu
                const std::string menuItemDescription = "Select Hold Profile";

                // The callback function id for the menu being opened
                const unsigned int selectorMenuOpenCallbackId;
                
                // The callback function id to give to the first profile
                const unsigned int firstProfileCallbackId;

                // The string to put in the first column if no profile
                const std::string noProfileFirstColumn = "";

            private:

                // The profile that has been selected
                unsigned int selectedProfile = 0;

                // Manages the hold profiles
                const UKControllerPlugin::Hold::HoldProfileManager & profileManager;

                // Used to open popup menus
                UKControllerPlugin::Euroscope::EuroscopePluginLoopbackInterface & plugin;

                // The renderer to which to send selection commands
                UKControllerPlugin::Hold::HoldRenderer & renderer;
        };
    }  // namespace Hold
}  // namespace UKControllerPlugin
