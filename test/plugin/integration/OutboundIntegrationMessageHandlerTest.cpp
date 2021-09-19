#include "integration/OutboundIntegrationMessageHandler.h"
#include "integration/IntegrationClientManager.h"
#include "integration/IntegrationClient.h"
#include "integration/IntegrationConnection.h"
#include "integration/InboundMessage.h"
#include "integration/MessageType.h"

using UKControllerPlugin::Integration::IntegrationClient;
using UKControllerPlugin::Integration::IntegrationClientManager;
using UKControllerPlugin::Integration::IntegrationConnection;
using UKControllerPlugin::Integration::MessageType;
using UKControllerPlugin::Integration::OutboundIntegrationMessageHandler;

namespace UKControllerPluginTest::Integration {
    class OutboundIntegrationMessageHandlerTest : public testing::Test
    {
        public:
        OutboundIntegrationMessageHandlerTest() : clientManager(new IntegrationClientManager), handler(clientManager)
        {
            this->mockConnection1 = std::make_shared<testing::NiceMock<MockConnection>>();
            this->mockConnection2 = std::make_shared<testing::NiceMock<MockConnection>>();

            this->client1 = std::make_shared<IntegrationClient>(
                1, "foo", "bar", std::make_shared<IntegrationConnection>(this->mockConnection1));

            this->client2 = std::make_shared<IntegrationClient>(
                2, "foo", "bar", std::make_shared<IntegrationConnection>(this->mockConnection2));

            this->parsedTestMessage =
                UKControllerPlugin::Integration::InboundMessage::FromJson(this->GetTestJson("test1", 2));

            type1.reset(new MessageType{"test1", 2});
            type2.reset(new MessageType{"test2", 3});
            type3.reset(new MessageType{"test3", 4});
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
                {"id", "foo"},
                {"data", nlohmann::json::object({{"foo", "bar"}})}};
        }

        std::shared_ptr<MessageType> type1;
        std::shared_ptr<MessageType> type2;
        std::shared_ptr<MessageType> type3;
        std::shared_ptr<UKControllerPlugin::Integration::InboundMessage> parsedTestMessage;
        std::shared_ptr<IntegrationClient> client1;
        std::shared_ptr<IntegrationClient> client2;
        std::shared_ptr<testing::NiceMock<MockConnection>> mockConnection1;
        std::shared_ptr<testing::NiceMock<MockConnection>> mockConnection2;
        std::shared_ptr<IntegrationClientManager> clientManager;
        OutboundIntegrationMessageHandler handler;
    };

    TEST_F(OutboundIntegrationMessageHandlerTest, ItSendsMessagesToInterestedClients)
    {
        this->clientManager->AddClient(this->client1);
        this->clientManager->AddClient(this->client2);
        this->client1->AddInterestedMessage(this->type1);
        this->client1->AddInterestedMessage(this->type2);
        this->client2->AddInterestedMessage(this->type3);

        EXPECT_CALL(*this->mockConnection1, Send(testing::_)).Times(1);

        EXPECT_CALL(*this->mockConnection2, Send(testing::_)).Times(0);

        this->handler.SendEvent(this->parsedTestMessage);
    }
} // namespace UKControllerPluginTest::Integration
