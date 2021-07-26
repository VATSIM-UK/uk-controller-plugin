#include "pch/pch.h"
#include "integration/InitialisationSuccessMessage.h"
#include "integration/IntegrationClient.h"
#include "integration/IntegrationConnection.h"
#include "integration/MessageType.h"
#include "mock/MockConnection.h"

using testing::Test;
using UKControllerPlugin::Integration::IntegrationClient;
using UKControllerPlugin::Integration::IntegrationConnection;
using UKControllerPlugin::Integration::MessageType;
using UKControllerPluginTest::Integration::MockConnection;
using UKControllerPlugin::Integration::InitialisationSuccessMessage;

namespace UKControllerPluginTest::Integration {

    class IntegrationConnectionTest : public Test
    {
        public:
            IntegrationConnectionTest()
                : mockConnection(new testing::NiceMock<MockConnection>()),
                  connection(mockConnection)
            {}

            std::shared_ptr<testing::NiceMock<MockConnection>> mockConnection;
            IntegrationConnection connection;
    };

    TEST_F(IntegrationConnectionTest, ItIsActiveIfItsConnectionIsActive)
    {
        ON_CALL(*this->mockConnection, Active)
            .WillByDefault(testing::Return(true));

        EXPECT_TRUE(connection.Active());
    }

    TEST_F(IntegrationConnectionTest, ItIsInactiveIfItsConnectionIsNotActive)
    {
        ON_CALL(*this->mockConnection, Active)
            .WillByDefault(testing::Return(false));

        EXPECT_FALSE(connection.Active());
    }

    TEST_F(IntegrationConnectionTest, ItSendsAMessage)
    {
        const auto message = std::make_shared<InitialisationSuccessMessage>();

        EXPECT_CALL(*mockConnection, Send(message->ToJson().dump()))
            .Times(1);

        connection.Send(message);
    }

    TEST_F(IntegrationConnectionTest, ItReturnsNoParsedMessages)
    {
        EXPECT_CALL(*mockConnection, Receive)
            .Times(1)
            .WillOnce(testing::Return(std::queue<std::string>()));

        EXPECT_EQ(0, connection.Receive().size());
    }

    TEST_F(IntegrationConnectionTest, ItReturnsParsedMessages)
    {
        const auto message = std::make_shared<InitialisationSuccessMessage>();
        const nlohmann::json message1Data = message->ToJson();
        nlohmann::json message2Data = message->ToJson();
        message2Data["version"] = 2;

        std::queue<std::string> messages;
        messages.push(message1Data.dump());
        messages.push(message2Data.dump());

        EXPECT_CALL(*mockConnection, Receive)
            .Times(1)
            .WillOnce(testing::Return(messages));

        auto receivedMessages = connection.Receive();
        EXPECT_EQ(2, receivedMessages.size());
        EXPECT_EQ(message1Data, receivedMessages.front()->ToJson());
        receivedMessages.pop();
        EXPECT_EQ(message2Data, receivedMessages.front()->ToJson());
    }

    TEST_F(IntegrationConnectionTest, ItHandlesInvalidInboundMessages)
    {
        const auto message = std::make_shared<InitialisationSuccessMessage>();
        nlohmann::json messageData = message->ToJson();
        messageData["version"] = "invalid";

        std::queue<std::string> messages;
        messages.push(messageData.dump());

        EXPECT_CALL(*mockConnection, Receive)
            .Times(1)
            .WillOnce(testing::Return(messages));

        const auto receivedMessages = connection.Receive();
        EXPECT_EQ(0, receivedMessages.size());
    }

    TEST_F(IntegrationConnectionTest, ItHandlesInvalidJsonInInboundMessages)
    {
        std::queue<std::string> messages;
        messages.push("{]");

        EXPECT_CALL(*mockConnection, Receive)
            .Times(1)
            .WillOnce(testing::Return(messages));

        const auto receivedMessages = connection.Receive();
        EXPECT_EQ(0, receivedMessages.size());
    }
} // namespace UKControllerPluginTest::Integration
