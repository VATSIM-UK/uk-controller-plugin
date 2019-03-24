#pragma once
#include "plugin/PopupMenuItem.h"

namespace UKControllerPlugin {
    namespace RadarScreen {

        /*
            An interface for displays that want to be "configurable" - ie they want to provide
            some method that can be called to open up some form of configuration prompt.
        */
        class ConfigurableDisplayInterface
        {
            public:

                /*
                    When this method is called, the module should provide some method of configuring it -
                    possibly a dialog box or a visibility toggle.
                */
                virtual void Configure(int functionId, std::string subject, RECT screenObjectArea) = 0;

                /*
                    Returns a popup menu item for the configurable display. This is used to populate
                    configuration menues.
                */
                virtual UKControllerPlugin::Plugin::PopupMenuItem GetConfigurationMenuItem(void) const = 0;
        };
    }  // namespace RadarScreen
}  // namespace UKControllerPlugin
