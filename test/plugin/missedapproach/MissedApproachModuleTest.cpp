#include "bootstrap/PersistenceContainer.h"
#include "missedapproach/MissedApproachModule.h"
#include "push/PushEventProcessorCollection.h"
#include "timedevent/TimedEventCollection.h"

using UKControllerPlugin::Bootstrap::PersistenceContainer;
using UKControllerPlugin::MissedApproach::BootstrapPlugin;
using UKControllerPlugin::Push::PushEventProcessorCollection;
using UKControllerPlugin::TimedEvent::TimedEventCollection;

namespace UKControllerPluginTest::MissedApproach {
    class MissedApproachModuleTest : public testing::Test
    {
        public:
        MissedApproachModuleTest()
        {
            container.timedHandler = std::make_unique<TimedEventCollection>();
            container.pushEventProcessors = std::make_shared<PushEventProcessorCollection>();
        }

        PersistenceContainer container;
    };

    TEST_F(MissedApproachModuleTest, ItRegistersTheExpiredApproachRemover)
    {
        BootstrapPlugin(container);
        EXPECT_EQ(1, container.timedHandler->CountHandlers());
        EXPECT_EQ(1, container.timedHandler->CountHandlersForFrequency(30));
    }

    TEST_F(MissedApproachModuleTest, ItRegistersTheNewMissedApproachPushEvent)
    {
        BootstrapPlugin(container);
        EXPECT_EQ(1, container.pushEventProcessors->CountProcessorsForEvent("missed-approach.created"));
    }
} // namespace UKControllerPluginTest::MissedApproach
