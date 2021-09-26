#include "bootstrap/PersistenceContainer.h"
#include "missedapproach/MissedApproachModule.h"
#include "plugin/FunctionCallEventHandler.h"
#include "push/PushEventProcessorCollection.h"
#include "timedevent/TimedEventCollection.h"

using UKControllerPlugin::Bootstrap::PersistenceContainer;
using UKControllerPlugin::MissedApproach::BootstrapPlugin;
using UKControllerPlugin::Plugin::FunctionCallEventHandler;
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
            container.pluginFunctionHandlers = std::make_unique<FunctionCallEventHandler>();
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

    TEST_F(MissedApproachModuleTest, ItRegistersTheTriggerMissedApproachTagFunction)
    {
        BootstrapPlugin(container);
        EXPECT_TRUE(container.pluginFunctionHandlers->HasTagFunction(9020));
    }
} // namespace UKControllerPluginTest::MissedApproach
