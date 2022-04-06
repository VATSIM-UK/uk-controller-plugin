#include "MappingBootstrapProvider.h"
#include "MappingModuleFactory.h"
#include "MappingRenderer.h"
#include "bootstrap/ModuleFactories.h"
#include "bootstrap/PersistenceContainer.h"
#include "radarscreen/RadarRenderableCollection.h"

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
        radarRenderables.RegisterRenderer(
            radarRenderables.ReserveRendererIdentifier(),
            std::make_shared<MappingRenderer>(
                container.moduleFactories->Mapping().ElementManager(*container.dependencyLoader)),
            RadarScreen::RadarRenderableCollection::beforeTags);
    }
} // namespace UKControllerPlugin::Mapping
