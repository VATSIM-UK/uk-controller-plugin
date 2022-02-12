#include "aircraft/CallsignSelectionListFactoryBootstrap.h"
#include "bootstrap/PersistenceContainer.h"

using UKControllerPlugin::Aircraft::BootstrapPlugin;
using UKControllerPlugin::Bootstrap::PersistenceContainer;

namespace UKControllerPluginTest::Aircraft {
    class CallsignSelectionListFactoryBootstrapTest : public testing::Test
    {
        public:
        PersistenceContainer container;
    };

    TEST_F(CallsignSelectionListFactoryBootstrapTest, ItSetsFactoryOnContainer)
    {
        BootstrapPlugin(container);
        EXPECT_NE(nullptr, container.callsignSelectionListFactory);
    }
} // namespace UKControllerPluginTest::Aircraft
