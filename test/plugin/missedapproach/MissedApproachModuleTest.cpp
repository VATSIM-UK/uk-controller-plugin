#include "bootstrap/PersistenceContainer.h"
#include "missedapproach/MissedApproachModule.h"
#include "timedevent/TimedEventCollection.h"

using UKControllerPlugin::Bootstrap::PersistenceContainer;
using UKControllerPlugin::MissedApproach::BootstrapPlugin;
using UKControllerPlugin::TimedEvent::TimedEventCollection;

namespace UKControllerPluginTest::MissedApproach {
    class MissedApproachModuleTest : public testing::Test
    {
        public:
        MissedApproachModuleTest()
        {
            container.timedHandler = std::make_unique<TimedEventCollection>();
        }

        PersistenceContainer container;
    };

    TEST_F(MissedApproachModuleTest, ItRegistersTheExpiredApproachRemover)
    {
        BootstrapPlugin(container);
        EXPECT_EQ(1, container.timedHandler->CountHandlers());
        EXPECT_EQ(1, container.timedHandler->CountHandlersForFrequency(30));
    }
} // namespace UKControllerPluginTest::MissedApproach
