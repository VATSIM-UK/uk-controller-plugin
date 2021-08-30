#include "bootstrap/PersistenceContainer.h"
#include "push/PushEventBootstrap.h"
#include "push/PushEventProcessorCollection.h"
#include "timedevent/TimedEventCollection.h"
#include "memory"

using testing::Test;
using UKControllerPlugin::Bootstrap::PersistenceContainer;
using UKControllerPlugin::Push::BootstrapPlugin;
using UKControllerPlugin::TimedEvent::TimedEventCollection;

namespace UKControllerPluginTest::Push {

    class PushEventBootstrapTest : public Test
    {
        public:
        PushEventBootstrapTest()
        {
            container.timedHandler = std::make_unique<TimedEventCollection>();
        }

        PersistenceContainer container;
    };

    TEST_F(PushEventBootstrapTest, ItSetsUpEventProcessorCollectionOnNonDuplicatePlugin)
    {
        BootstrapPlugin(this->container, false);
        EXPECT_NE(nullptr, this->container.pushEventProcessors);
    }

    TEST_F(PushEventBootstrapTest, ItSetsUpEventProcessorCollectionOnDuplicatePlugin)
    {
        BootstrapPlugin(this->container, true);
        EXPECT_NE(nullptr, this->container.pushEventProcessors);
    }

    TEST_F(PushEventBootstrapTest, ItSetsUpTimedEventsOnNonDuplicatePlugin)
    {
        BootstrapPlugin(this->container, false);
        EXPECT_EQ(2, this->container.timedHandler->CountHandlers());
        EXPECT_EQ(2, this->container.timedHandler->CountHandlersForFrequency(1));
    }

    TEST_F(PushEventBootstrapTest, ItSetsUpProtocolHandlerForTimedEventsOnDuplicatePlugin)
    {
        BootstrapPlugin(this->container, true);
        EXPECT_EQ(1, this->container.timedHandler->CountHandlers());
        EXPECT_EQ(1, this->container.timedHandler->CountHandlersForFrequency(1));
    }

    TEST_F(PushEventBootstrapTest, ItSetsUpProxyHandlerForPushEventsOnNonDuplicatePlugin)
    {
        BootstrapPlugin(this->container, false);
        EXPECT_EQ(1, this->container.pushEventProcessors->CountProcessorsForAll());
    }

    TEST_F(PushEventBootstrapTest, ItDoesntSetUpProxyHandlerForPushEventsOnDuplicatePlugin)
    {
        BootstrapPlugin(this->container, true);
        EXPECT_EQ(0, this->container.pushEventProcessors->CountProcessorsForAll());
    }
} // namespace UKControllerPluginTest::Push
