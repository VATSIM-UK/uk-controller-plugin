#include "push/ProxyPushDataSync.h"
#include "push/PushEventProcessorCollection.h"
#include "push/PushEventSubscription.h"

using testing::NiceMock;
using testing::Test;
using UKControllerPlugin::Push::ProxyPushDataSync;
using UKControllerPlugin::Push::PushEventProcessorCollection;
using UKControllerPlugin::Push::PushEventSubscription;

namespace UKControllerPluginTest::Push {

    class ProxyPushDataSyncTest : public Test
    {
        public:
        ProxyPushDataSyncTest() : eventProcessor(std::make_shared<NiceMock<MockPushEventProcessor>>()), sync(collection)
        {
            this->collection.AddProcessor(this->eventProcessor);
        }

        PushEventProcessorCollection collection;
        std::shared_ptr<NiceMock<MockPushEventProcessor>> eventProcessor;
        ProxyPushDataSync sync;
    };

    TEST_F(ProxyPushDataSyncTest, ItSyncsProxyDataOnFirstTimedEvent)
    {
        EXPECT_CALL(*this->eventProcessor, PluginEventsSynced).Times(1);

        sync.TimedEventTrigger();
    }

    TEST_F(ProxyPushDataSyncTest, ItDoesntSyncOnSubsequentTimedEvents)
    {
        EXPECT_CALL(*this->eventProcessor, PluginEventsSynced).Times(1);

        sync.TimedEventTrigger();
        sync.TimedEventTrigger();
        sync.TimedEventTrigger();
    }
} // namespace UKControllerPluginTest::Push
