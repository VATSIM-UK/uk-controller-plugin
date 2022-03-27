#include "ModuleFactories.h"
#include "approach/ApproachModuleFactory.h"

namespace UKControllerPlugin::Bootstrap {

    ModuleFactories::~ModuleFactories() = default;

    auto ModuleFactories::Approach() -> Approach::ApproachModuleFactory&
    {
        if (!approach) {
            approach = std::make_unique<Approach::ApproachModuleFactory>();
        }

        return *approach;
    }
} // namespace UKControllerPlugin::Bootstrap
