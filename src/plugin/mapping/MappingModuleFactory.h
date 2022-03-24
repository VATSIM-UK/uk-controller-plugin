#pragma once

namespace UKControllerPlugin::Dependency {
    class DependencyLoaderInterface;
} // namespace UKControllerPlugin::Dependency

namespace UKControllerPlugin::Mapping {
    class MappingElementManager;
    class VisualReferencePoint;

    /**
     * Bootstraps up the mapping module
     */
    class MappingModuleFactory
    {
        public:
        MappingModuleFactory();
        ~MappingModuleFactory();
        [[nodiscard]] auto VrpDrawer(std::string label, EuroScopePlugIn::CPosition position) ->
            std::shared_ptr<VisualReferencePoint>;
        [[nodiscard]] auto ElementManager(Dependency::DependencyLoaderInterface& dependencyLoader) ->
            MappingElementManager&;

        private:
        // Manages mapping elements
        std::unique_ptr<MappingElementManager> elementManager;
    };
} // namespace UKControllerPlugin::Mapping
