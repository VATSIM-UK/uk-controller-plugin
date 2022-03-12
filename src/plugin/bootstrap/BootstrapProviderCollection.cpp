#include "BootstrapProviderCollection.h"
#include "BootstrapProviderInterface.h"

namespace UKControllerPlugin::Bootstrap {

    BootstrapProviderCollection::~BootstrapProviderCollection() = default;

    void BootstrapProviderCollection::AddProvider(std::shared_ptr<BootstrapProviderInterface> provider)
    {
        auto inserted = providers.insert(std::move(provider)).second;
        assert(inserted && "Duplicate bootstrap provider added");
    }

    auto BootstrapProviderCollection::Count() const -> size_t
    {
        return providers.size();
    }

    void BootstrapProviderCollection::BootstrapPlugin(PersistenceContainer& container)
    {
        for (const auto& provider : providers) {
            provider->BootstrapPlugin(container);
        }
    }

    void BootstrapProviderCollection::BootstrapRadarScreen(
        const PersistenceContainer& container,
        RadarScreen::RadarRenderableCollection& radarRenderables,
        RadarScreen::ConfigurableDisplayCollection& configurables,
        Euroscope::AsrEventHandlerCollection& asrHandlers,
        const RadarScreen::MenuToggleableDisplayFactory& toggleableDisplayFactory)
    {
        for (const auto& provider : providers) {
            provider->BootstrapRadarScreen(
                container, radarRenderables, configurables, asrHandlers, toggleableDisplayFactory);
        }
    }

    auto BootstrapProviderCollection::HasProvider(
        const std::function<bool(const BootstrapProviderInterface&)> predicate) const -> bool
    {
        for (const auto& provider : providers) {
            if (predicate(*provider)) {
                return true;
            }
        }

        return false;
    }
} // namespace UKControllerPlugin::Bootstrap
