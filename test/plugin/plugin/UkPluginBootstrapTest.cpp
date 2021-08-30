#include "bootstrap/PersistenceContainer.h"
#include "plugin/UKPlugin.h"
#include "plugin/UkPluginBootstrap.h"

using UKControllerPlugin::Bootstrap::PersistenceContainer;
using UKControllerPlugin::Bootstrap::UkPluginBootstrap;

namespace UKControllerPluginTest::Bootstrap {

    TEST(UkPluginBootstrap, BootstrapPluginCreatesPlugin)
    {
        PersistenceContainer container;
        UkPluginBootstrap::BootstrapPlugin(container);

        EXPECT_NO_THROW(container.plugin->GetPlugInName());
    }
} // namespace UKControllerPluginTest::Bootstrap
