#pragma once
#include "dialog/DialogManager.h"
#include "radarscreen/ConfigurableDisplayInterface.h"

namespace UKControllerPlugin {
    namespace Srd {

        /*
            A handler that triggers the SRD searching dialog
        */
        class SrdSearchHandler : public UKControllerPlugin::RadarScreen::ConfigurableDisplayInterface
        {
            public:
                SrdSearchHandler(
                    const int menuCallbackId,
                    const UKControllerPlugin::Dialog::DialogManager& dialog
                );

                // Inherited via ConfigurableDisplayInterface
                void Configure(int functionId, std::string subject, RECT screenObjectArea) override;
                UKControllerPlugin::Plugin::PopupMenuItem GetConfigurationMenuItem(void) const override;

                const std::string itemDescription = "Open SRD Search Dialog";

            private:

                // Manages the dialogs
                const UKControllerPlugin::Dialog::DialogManager& dialog;

                // The callback id for the menu
                const int menuCallbackId;
        };
    }  // namespace Srd
}  // UKControllerPlugin
