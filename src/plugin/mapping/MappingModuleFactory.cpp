#include "MappingElementManager.h"
#include "MappingElementManagerFactory.h"
#include "MappingModuleFactory.h"
#include "VisualReferencePoint.h"

namespace UKControllerPlugin::Mapping {

    MappingModuleFactory::MappingModuleFactory() = default;
    MappingModuleFactory::~MappingModuleFactory() = default;

    auto MappingModuleFactory::VrpDrawer(std::string label, EuroScopePlugIn::CPosition position)
        -> std::shared_ptr<VisualReferencePoint>
    {
        return std::make_shared<VisualReferencePoint>(label, position);
    }

    auto MappingModuleFactory::ElementManager(Dependency::DependencyLoaderInterface& dependencyLoader)
        -> MappingElementManager&
    {
        if (!elementManager) {
            elementManager = MakeMappingElementManager(*this, dependencyLoader);
        }

        return *elementManager;
    }
} // namespace UKControllerPlugin::Mapping
