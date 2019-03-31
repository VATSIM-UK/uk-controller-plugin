#pragma once
#include "radarscreen/ConfigurableDisplayInterface.h"
#include "euroscope/AsrEventHandlerInterface.h"

namespace UKControllerPlugin {
    namespace Hold {
        class HoldRenderer;
        class HoldDisplayManager;
    }  // namespace Hold
    namespace Euroscope {
        class EuroscopePluginLoopbackInterface;
    }  // namespace Euroscope
    namespace Dialog {
        class DialogManager;
    }  // namespace Dialog
}  // namespace UKControllerPlugin

namespace UKControllerPlugin {
    namespace Hold {

        /*
            Represents a EuroScope popup menu that selects the hold profile that should be used.
        */
        class HoldConfigurationMenuItem : public UKControllerPlugin::RadarScreen::ConfigurableDisplayInterface
        {
            public:
                HoldConfigurationMenuItem(
                    const UKControllerPlugin::Dialog::DialogManager & dialogManager,
                    const std::shared_ptr<UKControllerPlugin::Hold::HoldDisplayManager> displayManager,
                    unsigned int selectorMenuOpenCallbackId
                );

                // Inherited via ConfigurableDisplayInterface
                void Configure(int functionId, std::string subject, RECT screenObjectArea) override;
                UKControllerPlugin::Plugin::PopupMenuItem GetConfigurationMenuItem(void) const override;

                void InvalidateProfile(unsigned int profileId);
                void SelectProfile(unsigned int profileId);

                // What to display in the configuration menu
                const std::string menuItemDescription = "Hold Profile Configuration";

                // The callback function id for the menu being opened
                const unsigned int selectorMenuOpenCallbackId;

            private:

                // The display manager for this ASR
                const std::shared_ptr<UKControllerPlugin::Hold::HoldDisplayManager> displayManager;

                // Manages dialogs
                const UKControllerPlugin::Dialog::DialogManager & dialogManager;
        };
    }  // namespace Hold
}  // namespace UKControllerPlugin
