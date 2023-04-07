#include "integration/IntegrationClient.h"
#include "integration/IntegrationConnection.h"
#include "integration/IntegrationDataInitialisers.h"
#include "integration/MessageType.h"
#include <gmock/gmock-matchers.h>

namespace UKControllerPluginTest::Integration {

    class IntegrationDataInitialisersTest : public testing::Test
    {
        public:
        IntegrationDataInitialisersTest()
            : mock1(std::make_shared<testing::NiceMock<MockIntegrationDataInitialiser>>()),
              mock2(std::make_shared<testing::NiceMock<MockIntegrationDataInitialiser>>())
        {
        }

        testing::NiceMock<std::shared_ptr<MockIntegrationDataInitialiser>> mock1;
        testing::NiceMock<std::shared_ptr<MockIntegrationDataInitialiser>> mock2;
        UKControllerPlugin::Integration::IntegrationDataInitialisers initialisers;
    };

    TEST_F(IntegrationDataInitialisersTest, ItStartsEmpty)
    {
        EXPECT_EQ(0, initialisers.Count());
    }

    TEST_F(IntegrationDataInitialisersTest, ItAddsInitialisers)
    {
        initialisers.Add(mock1);
        initialisers.Add(mock2);
        EXPECT_EQ(2, initialisers.Count());
    }

    TEST_F(IntegrationDataInitialisersTest, ItDoesntAddDuplicateInitialisers)
    {
        initialisers.Add(mock1);
        initialisers.Add(mock1);
        initialisers.Add(mock1);
        initialisers.Add(mock2);
        initialisers.Add(mock2);
        initialisers.Add(mock2);
        EXPECT_EQ(2, initialisers.Count());
    }

    TEST_F(IntegrationDataInitialisersTest, ItInitialisesAClientIfInitialiserIsRelevant)
    {
        const auto message1 = UKControllerPlugin::Integration::MessageType{"type_1", 1};
        const auto message2 = UKControllerPlugin::Integration::MessageType{"type_2", 3};

        const auto connection = std::make_shared<testing::NiceMock<MockConnection>>();
        const auto integrationConnection =
            std::make_shared<UKControllerPlugin::Integration::IntegrationConnection>(connection);
        auto client = UKControllerPlugin::Integration::IntegrationClient(1, "1", "1", integrationConnection);
        client.AddInterestedMessage(std::make_shared<UKControllerPlugin::Integration::MessageType>(message1));

        initialisers.Add(mock1);
        initialisers.Add(mock2);

        EXPECT_CALL(*mock1, InitialisesFor()).Times(1).WillOnce(testing::Return(message1));

        EXPECT_CALL(*mock1, Initialise(testing::Ref(client))).Times(1);

        EXPECT_CALL(*mock2, InitialisesFor()).Times(1).WillOnce(testing::Return(message2));

        EXPECT_CALL(*mock2, Initialise).Times(0);

        initialisers.InitialiseClient(client);
    }
} // namespace UKControllerPluginTest::Integration
