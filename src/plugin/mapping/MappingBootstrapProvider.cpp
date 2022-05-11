#include "MappingBootstrapProvider.h"
#include "MappingModuleFactory.h"
#include "MappingOptionsRenderer.h"
#include "MappingRenderer.h"
#include "MappingRenderOptions.h"
#include "ToggleMappingOptionsRender.h"
#include "bootstrap/ModuleFactories.h"
#include "bootstrap/PersistenceContainer.h"
#include "radarscreen/RadarRenderableCollection.h"

namespace UKControllerPlugin::Mapping {
    void MappingBootstrapProvider::BootstrapRadarScreen(
        const Bootstrap::PersistenceContainer& container,
        RadarScreen::RadarRenderableCollection& radarRenderables,
        RadarScreen::ConfigurableDisplayCollection& configurables,
        Euroscope::AsrEventHandlerCollection& asrHandlers,
        const RadarScreen::MenuToggleableDisplayFactory& toggleableDisplayFactory)
    {
        // Renders the mapping elements
        radarRenderables.RegisterRenderer(
            radarRenderables.ReserveRendererIdentifier(),
            std::make_shared<MappingRenderer>(
                container.moduleFactories->Mapping().ElementManager(*container.dependencyLoader)),
            RadarScreen::RadarRenderableCollection::beforeTags);

        // The rendering options
        auto renderOptions = std::make_shared<MappingRenderOptions>();

        // Renders the options
        auto optionsRendererId = radarRenderables.ReserveRendererIdentifier();
        radarRenderables.RegisterRenderer(
            optionsRendererId,
            std::make_shared<MappingOptionsRenderer>(
                renderOptions, radarRenderables.ReserveScreenObjectIdentifier(optionsRendererId)),
            RadarScreen::RadarRenderableCollection::afterLists);

        // Toggles the options
        auto toggleOptionsRendererId = radarRenderables.ReserveRendererIdentifier();
        radarRenderables.RegisterRenderer(
            toggleOptionsRendererId,
            std::make_shared<ToggleMappingOptionsRender>(
                renderOptions, radarRenderables.ReserveScreenObjectIdentifier(toggleOptionsRendererId)),
            RadarScreen::RadarRenderableCollection::afterLists);
    }
} // namespace UKControllerPlugin::Mapping
