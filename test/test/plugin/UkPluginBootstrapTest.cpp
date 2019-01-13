#include "pch/pch.h"
#include "plugin/UkPluginBootstrap.h"
#include "bootstrap/PersistenceContainer.h"

using UKControllerPlugin::Bootstrap::UkPluginBootstrap;
using UKControllerPlugin::Bootstrap::PersistenceContainer;

namespace UKcontrollPluginCoreTest {
    namespace Bootstrap {

        TEST(UkPluginBootstrap, BootstrapPluginCreatesPlugin)
        {
            PersistenceContainer container;
            UkPluginBootstrap::BootstrapPlugin(container);

            EXPECT_NO_THROW(container.plugin->GetPlugInName());
        }
    }  // namespace Bootstrap
}  // namespace UKcontrollPluginCoreTest
