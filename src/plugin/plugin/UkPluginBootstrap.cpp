#include "pch/stdafx.h"
#include "plugin/UkPluginBootstrap.h"
#include "bootstrap/PersistenceContainer.h"
#include "plugin/UKPlugin.h"
#include "radarscreen/RadarScreenFactory.h"

using UKControllerPlugin::Bootstrap::PersistenceContainer;
using UKControllerPlugin::UKPlugin;
using UKControllerPlugin::RadarScreen::RadarScreenFactory;
namespace UKControllerPlugin {
    namespace Bootstrap {

        /*
            Add the plugin to the persistence container.
        */
        void UkPluginBootstrap::BootstrapPlugin(PersistenceContainer & persistence)
        {
            persistence.plugin.reset(
                new UKPlugin(
                    *persistence.radarTargetHandler,
                    *persistence.flightplanHandler,
                    *persistence.controllerHandler,
                    *persistence.timedHandler,
                    *persistence.tagHandler,
                    RadarScreenFactory(persistence),
                    *persistence.metarEventHandler,
                    *persistence.pluginFunctionHandlers,
                    *persistence.commandHandlers,
                    *persistence.runwayDialogEventHandlers,
                    *persistence.controllerHandoffHandlers
                )
            );
        }
    }  // namespace Bootstrap
}  // namespace UKControllerPlugin
