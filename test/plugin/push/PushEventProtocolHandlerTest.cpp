#include "pch/pch.h"
#include "push/PushEventProtocolHandler.h"
#include "push/PushEventProcessorCollection.h"
#include "mock/MockPushEventProcessor.h"
#include "mock/MockPushEventConnection.h"
#include "push/PushEventSubscription.h"

using ::testing::_;
using ::testing::NiceMock;
using ::testing::Return;
using ::testing::Test;
using UKControllerPlugin::Push::PushEventProcessorCollection;
using UKControllerPlugin::Push::PushEventProtocolHandler;
using UKControllerPlugin::Push::PushEventSubscription;
using UKControllerPluginTest::Push::MockPushEventConnection;
using UKControllerPluginTest::Push::MockPushEventProcessor;

namespace UKControllerPluginTest {
    namespace Push {

        class PushEventProtocolHandlerTest : public Test
        {
            public:
            PushEventProtocolHandlerTest()
                : pushEvent(std::make_shared<NiceMock<MockPushEventConnection>>()),
                  handler(this->pushEvent, this->processorCollection)
            {
                mockEventProcessor.reset(new NiceMock<MockPushEventProcessor>);

                std::set channelSubs = {subChannel1, subChannel2};

                ON_CALL(*this->mockEventProcessor, GetPushEventSubscriptions()).WillByDefault(Return(channelSubs));

                this->processorCollection.AddProcessor(this->mockEventProcessor);
            }

            std::shared_ptr<NiceMock<MockPushEventConnection>> pushEvent;
            PushEventProcessorCollection processorCollection;

            std::shared_ptr<NiceMock<MockPushEventProcessor>> mockEventProcessor;
            PushEventSubscription subChannel1 = {PushEventSubscription::SUB_TYPE_CHANNEL, "channel1"};
            PushEventSubscription subChannel2 = {PushEventSubscription::SUB_TYPE_CHANNEL, "channel2"};

            PushEventProtocolHandler handler;
        };

        TEST_F(PushEventProtocolHandlerTest, ItDoesNothingOnNoMessage)
        {
            EXPECT_CALL(*this->mockEventProcessor, ProcessPushEvent(_)).Times(0);

            ON_CALL(*this->pushEvent, GetNextMessage).WillByDefault(Return(this->pushEvent->noMessage));

            this->handler.TimedEventTrigger();
        }

        TEST_F(PushEventProtocolHandlerTest, ItDoesNothingOnInvalidMessage)
        {
            EXPECT_CALL(*this->mockEventProcessor, ProcessPushEvent(_)).Times(0);

            EXPECT_CALL(*this->pushEvent, GetNextMessage)
                .Times(2)
                .WillOnce(Return("{]"))
                .WillOnce(Return(this->pushEvent->noMessage));

            this->handler.TimedEventTrigger();
        }

        TEST_F(PushEventProtocolHandlerTest, ItHandlesMessages)
        {
            EXPECT_CALL(*this->mockEventProcessor, ProcessPushEvent(_)).Times(1);

            nlohmann::json eventMessage;
            eventMessage["channel"] = "channel1";
            eventMessage["event"] = "test-event";
            eventMessage["data"] = {{"foo", "bar"}};

            EXPECT_CALL(*this->pushEvent, GetNextMessage)
                .Times(2)
                .WillOnce(Return(eventMessage.dump()))
                .WillOnce(Return(this->pushEvent->noMessage));

            this->handler.TimedEventTrigger();
        }
    } // namespace Push
} // namespace UKControllerPluginTest
