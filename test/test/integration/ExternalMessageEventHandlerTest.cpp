#include "pch/pch.h"
#include "integration/ExternalMessageEventHandler.h"
#include "mock/MockExternalMessageHandlerInterface.h"

using ::testing::Test;
using UKControllerPlugin::Integration::ExternalMessageEventHandler;

namespace UKControllerPluginTest {
    namespace Integration {

        class ExternalMessageEventHandlerTest : public Test
        {
            public:
                ExternalMessageEventHandler eventHandler = ExternalMessageEventHandler(true);
        };

        TEST_F(ExternalMessageEventHandlerTest, ItStartsWithNoHandlers)
        {
            EXPECT_EQ(0, this->eventHandler.CountHandlers());
        }

        TEST_F(ExternalMessageEventHandlerTest, ItAddsHandlers)
        {
            this->eventHandler.AddHandler(std::make_shared<MockExternalMessageHandlerInterface>(false));
            this->eventHandler.AddHandler(std::make_shared<MockExternalMessageHandlerInterface>(false));
            this->eventHandler.AddHandler(std::make_shared<MockExternalMessageHandlerInterface>(false));
            EXPECT_EQ(3, this->eventHandler.CountHandlers());
        }

        TEST_F(ExternalMessageEventHandlerTest, ItDoesntAddDuplicateHandlers)
        {
            std::shared_ptr<MockExternalMessageHandlerInterface> handler;
            this->eventHandler.AddHandler(handler);
            this->eventHandler.AddHandler(handler);
            this->eventHandler.AddHandler(handler);
            EXPECT_EQ(1, this->eventHandler.CountHandlers());
        }

        TEST_F(ExternalMessageEventHandlerTest, ItTriesHandlersWithMessage)
        {
            std::shared_ptr<MockExternalMessageHandlerInterface> handler1 =
                std::make_shared<MockExternalMessageHandlerInterface>(false);
            std::shared_ptr<MockExternalMessageHandlerInterface> handler2 =
                std::make_shared<MockExternalMessageHandlerInterface>(false);
            this->eventHandler.AddHandler(handler1);
            this->eventHandler.AddHandler(handler2);

            this->eventHandler.AddMessageToQueue("foo");
            this->eventHandler.TimedEventTrigger();

            EXPECT_TRUE(handler1->hasBeenCalled);
            EXPECT_EQ("foo", handler1->receivedMessage);
            EXPECT_TRUE(handler2->hasBeenCalled);
            EXPECT_EQ("foo", handler2->receivedMessage);
        }

        TEST_F(ExternalMessageEventHandlerTest, ItShortCircuitsIfProcessorHandlesEvent)
        {
            std::shared_ptr<MockExternalMessageHandlerInterface> handler1 =
                std::make_shared<MockExternalMessageHandlerInterface>(true);
            std::shared_ptr<MockExternalMessageHandlerInterface> handler2 =
                std::make_shared<MockExternalMessageHandlerInterface>(true);

            this->eventHandler.AddHandler(handler1);
            this->eventHandler.AddHandler(handler2);
            this->eventHandler.AddMessageToQueue("foo");
            this->eventHandler.TimedEventTrigger();

            if (handler1 < handler2)
            {
                EXPECT_TRUE(handler1->hasBeenCalled);
                EXPECT_FALSE(handler2->hasBeenCalled);
            } else {
                EXPECT_TRUE(handler2->hasBeenCalled);
                EXPECT_FALSE(handler1->hasBeenCalled);
            }
        }
    }  // namespace Integration
}  // namespace UKControllerPluginTest
