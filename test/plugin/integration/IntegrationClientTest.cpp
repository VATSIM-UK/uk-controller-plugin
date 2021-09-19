#include "integration/IntegrationClient.h"
#include "integration/IntegrationConnection.h"
#include "integration/MessageType.h"

using testing::Test;
using UKControllerPlugin::Integration::IntegrationClient;
using UKControllerPlugin::Integration::IntegrationConnection;
using UKControllerPlugin::Integration::MessageType;
using UKControllerPluginTest::Integration::MockConnection;

namespace UKControllerPluginTest::Integration {

    class IntegrationClientTest : public Test
    {
        public:
        IntegrationClientTest()
            : mockConnection(new testing::NiceMock<MockConnection>()),
              connection(new IntegrationConnection(mockConnection)), client(34, "testytest", "v1.5", connection)
        {
            type1.reset(new MessageType{"a", 1});
            type2.reset(new MessageType{"b", 2});
            type3.reset(new MessageType{"c", 3});
        }

        std::shared_ptr<MessageType> type1;
        std::shared_ptr<MessageType> type2;
        std::shared_ptr<MessageType> type3;
        std::shared_ptr<testing::NiceMock<MockConnection>> mockConnection;
        std::shared_ptr<IntegrationConnection> connection;
        IntegrationClient client;
    };

    TEST_F(IntegrationClientTest, ItHasAnId)
    {
        EXPECT_EQ(34, client.Id());
    }

    TEST_F(IntegrationClientTest, ItHasADescriptiveString)
    {
        EXPECT_EQ("testytest/v1.5", client.GetIntegrationString());
    }

    TEST_F(IntegrationClientTest, ItHasAConnection)
    {
        EXPECT_EQ(connection, client.Connection());
    }

    TEST_F(IntegrationClientTest, InterestedMessagesCanBeAdded)
    {
        client.AddInterestedMessage(type1);
        client.AddInterestedMessage(type2);
        client.AddInterestedMessage(type3);
        auto messages = client.InterestedMessages();
        EXPECT_EQ(3, messages.size());
        EXPECT_TRUE(messages.find(type1) != messages.cend());
        EXPECT_TRUE(messages.find(type2) != messages.cend());
        EXPECT_TRUE(messages.find(type3) != messages.cend());
    }

    TEST_F(IntegrationClientTest, DuplicateMessagesCannotBeAdded)
    {
        client.AddInterestedMessage(type1);
        client.AddInterestedMessage(type1);
        client.AddInterestedMessage(type2);
        client.AddInterestedMessage(type2);
        client.AddInterestedMessage(type3);
        client.AddInterestedMessage(type3);
        auto messages = client.InterestedMessages();
        EXPECT_EQ(3, messages.size());
    }

    TEST_F(IntegrationClientTest, InterestedMessagesCanBeCleared)
    {
        client.AddInterestedMessage(type1);
        client.AddInterestedMessage(type2);
        client.AddInterestedMessage(type3);
        client.ClearInterestedMessages();
        auto messages = client.InterestedMessages();
        EXPECT_EQ(0, messages.size());
    }

    TEST_F(IntegrationClientTest, ItCanBeInterestedInAMessage)
    {
        client.AddInterestedMessage(type1);
        client.AddInterestedMessage(type2);
        EXPECT_TRUE(client.InterestedInMessage(*type1));
        EXPECT_TRUE(client.InterestedInMessage(*type2));
    }

    TEST_F(IntegrationClientTest, ItCanBeUninterestedInAMessage)
    {
        client.AddInterestedMessage(type1);
        client.AddInterestedMessage(type2);
        EXPECT_FALSE(client.InterestedInMessage(*type3));
    }
} // namespace UKControllerPluginTest::Integration
