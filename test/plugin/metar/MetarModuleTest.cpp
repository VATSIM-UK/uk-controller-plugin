#include "bootstrap/PersistenceContainer.h"
#include "metar/MetarEventHandlerCollection.h"
#include "metar/MetarModule.h"
#include "push/PushEventProcessorCollection.h"

using UKControllerPlugin::Bootstrap::PersistenceContainer;
using UKControllerPlugin::Metar::BootstrapPlugin;
using UKControllerPlugin::Push::PushEventProcessorCollection;

namespace UKControllerPluginTest::Metar {
    class MetarModuleTest : public testing::Test
    {
        public:
        MetarModuleTest()
        {
            container.pushEventProcessors = std::make_unique<PushEventProcessorCollection>();
        }

        PersistenceContainer container;
    };

    TEST_F(MetarModuleTest, BootstrapPluginCreatesMetarEventHandlerCollection)
    {
        BootstrapPlugin(container);
        EXPECT_EQ(0, container.metarEventHandler->CountHandlers());
    }

    TEST_F(MetarModuleTest, BootstrapPluginRegistersMetarPushEvents)
    {
        BootstrapPlugin(container);
        EXPECT_EQ(1, container.pushEventProcessors->CountProcessorsForEvent("metars.updated"));
    }
} // namespace UKControllerPluginTest::Metar
