#include "MappingBootstrapProvider.h"
#include "MappingModuleFactory.h"
#include "bootstrap/PersistenceContainer.h"
#include "bootstrap/ModuleFactories.h"

namespace UKControllerPlugin::Mapping {

    void MappingBootstrapProvider::BootstrapPlugin(Bootstrap::PersistenceContainer& container)
    {
        static_cast<void>(container.moduleFactories->Mapping().ElementManager(*container.dependencyLoader));
    }

    void MappingBootstrapProvider::BootstrapRadarScreen(
        const Bootstrap::PersistenceContainer& container,
        RadarScreen::RadarRenderableCollection& radarRenderables,
        RadarScreen::ConfigurableDisplayCollection& configurables,
        Euroscope::AsrEventHandlerCollection& asrHandlers,
        const RadarScreen::MenuToggleableDisplayFactory& toggleableDisplayFactory)
    {
        BootstrapProviderInterface::BootstrapRadarScreen(
            container, radarRenderables, configurables, asrHandlers, toggleableDisplayFactory);
    }
} // namespace UKControllerPlugin::Mapping
