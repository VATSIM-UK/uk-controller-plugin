#include "pch/pch.h"
#include "integration/IntegrationClient.h"
#include "integration/IntegrationClientManager.h"
#include "integration/IntegrationConnection.h"
#include "integration/MessageType.h"
#include "mock/MockConnection.h"

using testing::Test;
using UKControllerPlugin::Integration::IntegrationClient;
using UKControllerPlugin::Integration::IntegrationClientManager;
using UKControllerPlugin::Integration::IntegrationConnection;
using UKControllerPlugin::Integration::MessageType;
using UKControllerPluginTest::Integration::MockConnection;
using testing::NiceMock;

namespace UKControllerPluginTest::Integration {

    class IntegrationClientManagerTest : public Test
    {
        public:
            IntegrationClientManagerTest()
            {
                this->mockConnection1 = std::make_shared<NiceMock<MockConnection>>();
                this->mockConnection2 = std::make_shared<NiceMock<MockConnection>>();
                this->connection1 = std::make_shared<IntegrationConnection>(mockConnection1);
                this->connection2 = std::make_shared<IntegrationConnection>(mockConnection2);
                this->client1 = std::make_shared<IntegrationClient>(
                    1,
                    "1",
                    "1",
                    this->connection1
                );
                this->client2 = std::make_shared<IntegrationClient>(
                    2,
                    "2",
                    "2",
                    this->connection2
                );

                type1.reset(new MessageType{"a", 1});
                type2.reset(new MessageType{"b", 2});
                type3.reset(new MessageType{"c", 3});
            }

            std::shared_ptr<MessageType> type1;
            std::shared_ptr<MessageType> type2;
            std::shared_ptr<MessageType> type3;
            std::shared_ptr<NiceMock<MockConnection>> mockConnection1;
            std::shared_ptr<NiceMock<MockConnection>> mockConnection2;
            std::shared_ptr<IntegrationConnection> connection1;
            std::shared_ptr<IntegrationConnection> connection2;
            std::shared_ptr<IntegrationClient> client1;
            std::shared_ptr<IntegrationClient> client2;
            IntegrationClientManager manager;
    };

    TEST_F(IntegrationClientManagerTest, ItStartsWithNoClients)
    {
        EXPECT_EQ(0, manager.CountClients());
    }

    TEST_F(IntegrationClientManagerTest, ItAddsClients)
    {
        manager.AddClient(this->client1);
        manager.AddClient(this->client2);
        EXPECT_EQ(2, manager.CountClients());
    }

    TEST_F(IntegrationClientManagerTest, ItDoesntAddDuplicateClients)
    {
        manager.AddClient(this->client1);
        manager.AddClient(this->client1);
        manager.AddClient(this->client1);
        manager.AddClient(this->client2);
        manager.AddClient(this->client2);
        manager.AddClient(this->client2);
        EXPECT_EQ(2, manager.CountClients());
    }

    TEST_F(IntegrationClientManagerTest, ItRemovesAllInactiveClients)
    {
        manager.AddClient(this->client1);
        manager.AddClient(this->client2);

        ON_CALL(*this->mockConnection1, Active)
            .WillByDefault(testing::Return(false));

        ON_CALL(*this->mockConnection2, Active)
            .WillByDefault(testing::Return(false));

        manager.TimedEventTrigger();
        EXPECT_EQ(0, manager.CountClients());
    }

    TEST_F(IntegrationClientManagerTest, ItRemovesSingleInactiveClients)
    {
        manager.AddClient(this->client1);
        manager.AddClient(this->client2);

        ON_CALL(*this->mockConnection1, Active)
            .WillByDefault(testing::Return(false));

        ON_CALL(*this->mockConnection2, Active)
            .WillByDefault(testing::Return(true));

        manager.TimedEventTrigger();
        EXPECT_EQ(1, manager.CountClients());
    }

    TEST_F(IntegrationClientManagerTest, ItFetchesAClientById)
    {
        manager.AddClient(this->client1);
        manager.AddClient(this->client2);

        EXPECT_EQ(this->client2, manager.GetById(2));
    }

    TEST_F(IntegrationClientManagerTest, ItReturnsNullPtrIfNoClientFoundForId)
    {
        manager.AddClient(this->client1);
        manager.AddClient(this->client2);

        EXPECT_EQ(nullptr, manager.GetById(33));
    }
} // namespace UKControllerPluginTest::Integration
