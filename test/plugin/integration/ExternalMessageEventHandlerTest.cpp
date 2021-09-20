#include "integration/ExternalMessageEventHandler.h"

using ::testing::Test;
using UKControllerPlugin::Integration::ExternalMessageEventHandler;

namespace UKControllerPluginTest {
    namespace Integration {

        class ExternalMessageEventHandlerTest : public Test
        {
            public:
            std::shared_ptr<ExternalMessageEventHandler> eventHandler;

            protected:
            void SetHandler(bool duplicatePlugin)
            {
                this->eventHandler = std::make_shared<ExternalMessageEventHandler>(duplicatePlugin);
            }
        };

        TEST_F(ExternalMessageEventHandlerTest, ItStartsWithNoHandlers)
        {
            this->SetHandler(true);
            EXPECT_EQ(0, this->eventHandler->CountHandlers());
        }

        TEST_F(ExternalMessageEventHandlerTest, ItAddsHandlers)
        {
            this->SetHandler(true);
            this->eventHandler->AddHandler(std::make_shared<MockExternalMessageHandlerInterface>(false));
            this->eventHandler->AddHandler(std::make_shared<MockExternalMessageHandlerInterface>(false));
            this->eventHandler->AddHandler(std::make_shared<MockExternalMessageHandlerInterface>(false));
            EXPECT_EQ(3, this->eventHandler->CountHandlers());
        }

        TEST_F(ExternalMessageEventHandlerTest, ItDoesntAddDuplicateHandlers)
        {
            this->SetHandler(true);
            std::shared_ptr<MockExternalMessageHandlerInterface> handler;
            this->eventHandler->AddHandler(handler);
            this->eventHandler->AddHandler(handler);
            this->eventHandler->AddHandler(handler);
            EXPECT_EQ(1, this->eventHandler->CountHandlers());
        }

        TEST_F(ExternalMessageEventHandlerTest, ItTriesHandlersWithMessage)
        {
            this->SetHandler(true);
            std::shared_ptr<MockExternalMessageHandlerInterface> handler1 =
                std::make_shared<MockExternalMessageHandlerInterface>(false);
            std::shared_ptr<MockExternalMessageHandlerInterface> handler2 =
                std::make_shared<MockExternalMessageHandlerInterface>(false);
            this->eventHandler->AddHandler(handler1);
            this->eventHandler->AddHandler(handler2);

            this->eventHandler->AddMessageToQueue("foo");
            this->eventHandler->TimedEventTrigger();

            EXPECT_TRUE(handler1->hasBeenCalled);
            EXPECT_EQ("foo", handler1->receivedMessage);
            EXPECT_TRUE(handler2->hasBeenCalled);
            EXPECT_EQ("foo", handler2->receivedMessage);
        }

        TEST_F(ExternalMessageEventHandlerTest, ItShortCircuitsIfProcessorHandlesEvent)
        {
            this->SetHandler(true);
            std::shared_ptr<MockExternalMessageHandlerInterface> handler1 =
                std::make_shared<MockExternalMessageHandlerInterface>(true);
            std::shared_ptr<MockExternalMessageHandlerInterface> handler2 =
                std::make_shared<MockExternalMessageHandlerInterface>(true);

            this->eventHandler->AddHandler(handler1);
            this->eventHandler->AddHandler(handler2);
            this->eventHandler->AddMessageToQueue("foo");
            this->eventHandler->TimedEventTrigger();

            if (handler1 < handler2) {
                EXPECT_TRUE(handler1->hasBeenCalled);
                EXPECT_FALSE(handler2->hasBeenCalled);
            } else {
                EXPECT_TRUE(handler2->hasBeenCalled);
                EXPECT_FALSE(handler1->hasBeenCalled);
            }
        }

        TEST_F(ExternalMessageEventHandlerTest, ItDoesntLoadWindowInDuplicatePluginMode)
        {
            this->SetHandler(true);
            EXPECT_EQ(nullptr, FindWindowA("UKControllerPluginHiddenWindowClass", "UKControllerPluginHiddenWindow"));
        }

        TEST_F(ExternalMessageEventHandlerTest, ItLoadsHiddenWindow)
        {
            this->SetHandler(false);
            EXPECT_NE(nullptr, FindWindowA("UKControllerPluginHiddenWindowClass", "UKControllerPluginHiddenWindow"));
        }

        TEST_F(ExternalMessageEventHandlerTest, ProcessCommandReturnsTrueIfValidCommand)
        {
            this->SetHandler(true);
            EXPECT_TRUE(this->eventHandler->ProcessCommand(".ukcp msg foobarbaz"));
        }

        TEST_F(ExternalMessageEventHandlerTest, ProcessCommandReturnsFalseIfEuroscopeCommand)
        {
            this->SetHandler(true);
            EXPECT_FALSE(this->eventHandler->ProcessCommand(".distance"));
        }

        TEST_F(ExternalMessageEventHandlerTest, ProcessCommandReturnsFalseIfNotCorrectUkcpCommand)
        {
            this->SetHandler(true);
            EXPECT_FALSE(this->eventHandler->ProcessCommand(".ukcp msg2 test"));
        }
    } // namespace Integration
} // namespace UKControllerPluginTest
