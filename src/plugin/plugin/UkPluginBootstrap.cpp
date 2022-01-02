#include "UKPlugin.h"
#include "UkPluginBootstrap.h"
#include "bootstrap/PersistenceContainer.h"
#include "radarscreen/RadarScreenFactory.h"

using UKControllerPlugin::UKPlugin;
using UKControllerPlugin::Bootstrap::PersistenceContainer;
using UKControllerPlugin::RadarScreen::RadarScreenFactory;
namespace UKControllerPlugin::Bootstrap {

    /*
        Add the plugin to the persistence container.
    */
    void UkPluginBootstrap::BootstrapPlugin(PersistenceContainer& persistence)
    {
        persistence.plugin.reset(new UKPlugin(
            *persistence.radarTargetHandler,
            *persistence.flightplanHandler,
            *persistence.controllerHandler,
            *persistence.timedHandler,
            *persistence.tagHandler,
            RadarScreenFactory(persistence),
            *persistence.pluginFunctionHandlers,
            *persistence.commandHandlers,
            *persistence.runwayDialogEventHandlers,
            *persistence.controllerHandoffHandlers));
    }
} // namespace UKControllerPlugin::Bootstrap
