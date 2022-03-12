#pragma once

namespace UKControllerPlugin {
    namespace Euroscope {
        class AsrEventHandlerCollection;
    } // namespace Euroscope
    namespace RadarScreen {
        class RadarRenderableCollection;
        class ConfigurableDisplayCollection;
        class MenuToggleableDisplayFactory;
    } // namespace RadarScreen
} // namespace UKControllerPlugin

namespace UKControllerPlugin::Bootstrap {
    struct PersistenceContainer;
    class BootstrapProviderInterface;

    /**
     * A collection of providers that perform the plugin
     * bootstrap process so that they can be called at the appropriate times.
     */
    class BootstrapProviderCollection
    {
        public:
        ~BootstrapProviderCollection();
        void AddProvider(std::shared_ptr<BootstrapProviderInterface> provider);
        auto HasProvider(const std::function<bool(const BootstrapProviderInterface&)> predicate) const -> bool;
        [[nodiscard]] auto Count() const -> size_t;
        void BootstrapPlugin(PersistenceContainer& container);
        void BootstrapRadarScreen(
            const PersistenceContainer& container,
            RadarScreen::RadarRenderableCollection& radarRenderables,
            RadarScreen::ConfigurableDisplayCollection& configurables,
            Euroscope::AsrEventHandlerCollection& asrHandlers,
            const RadarScreen::MenuToggleableDisplayFactory& toggleableDisplayFactory);

        private:
        // All the providers
        std::set<std::shared_ptr<BootstrapProviderInterface>> providers;
    };
} // namespace UKControllerPlugin::Bootstrap
