#include "BootstrapProviderCollection.h"
#include "BootstrapProviderCollectionFactory.h"
#include "ModuleBootstrap.h"
#include "ModuleFactories.h"
#include "PersistenceContainer.h"
#include "approach/ApproachModuleFactory.h"

namespace UKControllerPlugin::Bootstrap {

    void ModuleBootstrap(PersistenceContainer& container)
    {
        container.bootstrapProviders = Make();
        container.moduleFactories = std::make_unique<ModuleFactories>();
    }
} // namespace UKControllerPlugin::Bootstrap
