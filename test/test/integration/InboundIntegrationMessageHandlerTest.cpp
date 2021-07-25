#pragma once
#include "integration/InboundIntegrationMessageHandler.h"
#include "integration/IntegrationClientManager.h"
#include "integration/IntegrationClient.h"
#include "integration/IntegrationConnection.h"
#include "integration/InboundMessage.h"
#include "mock/MockConnection.h"
#include "mock/MockInboundIntegrationMessageProcessor.h"

using UKControllerPlugin::Integration::InboundIntegrationMessageHandler;
using UKControllerPlugin::Integration::IntegrationClientManager;
using UKControllerPlugin::Integration::IntegrationClient;
using UKControllerPlugin::Integration::IntegrationConnection;

MATCHER_P(MatchMessageInterface, message, "")
{
    return *arg == *message;
}

namespace UKControllerPluginTest::Integration {
    class InboundIntegrationMessageHandlerTest : public testing::Test
    {
        public:
            InboundIntegrationMessageHandlerTest()
                : clientManager(new IntegrationClientManager),
                  handler(clientManager)
            {
                this->processor1 = std::make_shared<testing::NiceMock<MockInboundIntegrationMessageProcessor>>();
                this->processor2 = std::make_shared<testing::NiceMock<MockInboundIntegrationMessageProcessor>>();

                ON_CALL(*this->processor1, MessageToProcess)
                    .WillByDefault(testing::Return(MessageType{"test1", 2}));

                ON_CALL(*this->processor2, MessageToProcess)
                    .WillByDefault(testing::Return(MessageType{"test2", 3}));

                this->mockConnection1 = std::make_shared<testing::NiceMock<MockConnection>>();
                this->mockConnection2 = std::make_shared<testing::NiceMock<MockConnection>>();

                this->client1 = std::make_shared<IntegrationClient>(
                    1,
                    "foo",
                    "bar",
                    std::make_shared<IntegrationConnection>(this->mockConnection1)
                );

                this->client2 = std::make_shared<IntegrationClient>(
                    2,
                    "foo",
                    "bar",
                    std::make_shared<IntegrationConnection>(this->mockConnection2)
                );

                this->parsedTestMessage1 = UKControllerPlugin::Integration::InboundMessage::FromJson(
                    this->GetTestJson("test1", 2));
                this->parsedTestMessage2 = UKControllerPlugin::Integration::InboundMessage::FromJson(
                    this->GetTestJson("test2", 3));
            }

            static std::string GetTestMessage(std::string type, int version)
            {
                return GetTestJson(type, version).dump();
            }

            static nlohmann::json GetTestJson(std::string type, int version)
            {
                return nlohmann::json{
                    {"type", type},
                    {"version", version},
                    {"data", nlohmann::json::object({{"foo", "bar"}})}
                };
            }

            std::shared_ptr<UKControllerPlugin::Integration::InboundMessage> parsedTestMessage1;
            std::shared_ptr<UKControllerPlugin::Integration::InboundMessage> parsedTestMessage2;
            std::shared_ptr<IntegrationClient> client1;
            std::shared_ptr<IntegrationClient> client2;
            std::shared_ptr<testing::NiceMock<MockConnection>> mockConnection1;
            std::shared_ptr<testing::NiceMock<MockConnection>> mockConnection2;
            std::shared_ptr<testing::NiceMock<MockInboundIntegrationMessageProcessor>> processor1;
            std::shared_ptr<testing::NiceMock<MockInboundIntegrationMessageProcessor>> processor2;
            std::shared_ptr<IntegrationClientManager> clientManager;
            InboundIntegrationMessageHandler handler;
    };

    TEST_F(InboundIntegrationMessageHandlerTest, ItStartsWithNoProcessors)
    {
        EXPECT_EQ(0, handler.CountProcessors());
    }

    TEST_F(InboundIntegrationMessageHandlerTest, ItDoesntAddDuplicateProcessors)
    {
        handler.AddProcessor(processor1);
        handler.AddProcessor(processor1);
        handler.AddProcessor(processor1);
        handler.AddProcessor(processor2);
        handler.AddProcessor(processor2);
        handler.AddProcessor(processor2);
        EXPECT_EQ(2, handler.CountProcessors());
    }

    TEST_F(InboundIntegrationMessageHandlerTest, ItProcessesMessagesForNoClients)
    {
        EXPECT_NO_THROW(handler.TimedEventTrigger());
    }

    TEST_F(InboundIntegrationMessageHandlerTest, ItCallsHandlerForMessage)
    {
        handler.AddProcessor(processor1);
        this->clientManager->AddClient(client1);

        ON_CALL(*mockConnection1, Receive)
            .WillByDefault(testing::Return(std::queue<std::string>({this->GetTestMessage("test1", 2)})));

        EXPECT_CALL(*mockConnection1, Send(parsedTestMessage1->ToJson().dump()))
            .Times(1);

        handler.TimedEventTrigger();
    }

    TEST_F(InboundIntegrationMessageHandlerTest, ItCallsHandlersForAllMessages)
    {
        handler.AddProcessor(processor1);
        handler.AddProcessor(processor2);
        this->clientManager->AddClient(client1);

        std::queue<std::string> messages;
        messages.push(this->GetTestMessage("test1", 2));
        messages.push(this->GetTestMessage("test2", 3));
        ON_CALL(*mockConnection1, Receive)
            .WillByDefault(testing::Return(messages));

        EXPECT_CALL(*mockConnection1, Send(parsedTestMessage1->ToJson().dump()))
            .Times(1);

        EXPECT_CALL(*mockConnection1, Send(parsedTestMessage2->ToJson().dump()))
            .Times(1);

        handler.TimedEventTrigger();
    }

    TEST_F(InboundIntegrationMessageHandlerTest, ItHandlesNoProcessorsBeingAvailableForMessages)
    {
        handler.AddProcessor(processor1);
        this->clientManager->AddClient(client1);

        std::queue<std::string> messages;
        messages.push(this->GetTestMessage("test1", 2));
        messages.push(this->GetTestMessage("test2", 3));
        ON_CALL(*mockConnection1, Receive)
            .WillByDefault(testing::Return(messages));

        EXPECT_CALL(*mockConnection1, Send(parsedTestMessage1->ToJson().dump()))
            .Times(1);

        EXPECT_CALL(*mockConnection2, Send(testing::_))
            .Times(0);

        handler.TimedEventTrigger();
    }

    TEST_F(InboundIntegrationMessageHandlerTest, ItProcessesAllConnections)
    {
        handler.AddProcessor(processor1);
        handler.AddProcessor(processor2);
        this->clientManager->AddClient(client1);
        this->clientManager->AddClient(client2);

        std::queue<std::string> messages1;
        messages1.push(this->GetTestMessage("test1", 2));

        ON_CALL(*mockConnection1, Receive)
            .WillByDefault(testing::Return(messages1));

        std::queue<std::string> messages2;
        messages2.push(this->GetTestMessage("test2", 3));

        ON_CALL(*mockConnection2, Receive)
            .WillByDefault(testing::Return(messages2));

        EXPECT_CALL(*mockConnection1, Send(parsedTestMessage1->ToJson().dump()))
            .Times(1);

        EXPECT_CALL(*mockConnection2, Send(parsedTestMessage2->ToJson().dump()))
            .Times(1);

        handler.TimedEventTrigger();
    }
} // namespace UKControllerPluginTest::Integration
