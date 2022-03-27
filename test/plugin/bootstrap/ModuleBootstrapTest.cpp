#include "bootstrap/ModuleBootstrap.h"
#include "bootstrap/PersistenceContainer.h"

using UKControllerPlugin::Bootstrap::ModuleBootstrap;
using UKControllerPlugin::Bootstrap::PersistenceContainer;

namespace UKControllerPluginTest::Bootstrap {
    class ModuleBootstrapTest : public testing::Test
    {
        public:
        PersistenceContainer container;
    };

    TEST_F(ModuleBootstrapTest, ItBootstrapsProviderCollection)
    {
        ModuleBootstrap(container);
        EXPECT_NE(nullptr, container.bootstrapProviders);
    }

    TEST_F(ModuleBootstrapTest, ItBootstrapsModuleFactories)
    {
        ModuleBootstrap(container);
        EXPECT_NE(nullptr, container.moduleFactories);
    }
} // namespace UKControllerPluginTest::Bootstrap
