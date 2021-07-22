#include "pch/pch.h"
#include "integration/ClientInitialisationManager.h"
#include "integration/InitialistionSuccessMessage.h"
#include "integration/IntegrationClient.h"
#include "integration/IntegrationClientManager.h"
#include "integration/IntegrationConnection.h"
#include "integration/MessageType.h"
#include "mock/MockConnection.h"
#include "time/SystemClock.h"

using UKControllerPlugin::Integration::IntegrationClientManager;
using UKControllerPlugin::Integration::ClientInitialisationManager;
using UKControllerPlugin::Time::SetTestNow;

namespace UKControllerPluginTest::Integration {
    class ClientInitialisationManagerTest : public testing::Test
    {
        public:
            ClientInitialisationManagerTest()
                : clientManager(new IntegrationClientManager),
                  initialisationManager(clientManager)
            {
                mockConnection1 = std::make_shared<testing::NiceMock<MockConnection>>();
                mockConnection2 = std::make_shared<testing::NiceMock<MockConnection>>();
                integration1 = std::make_shared<UKControllerPlugin::Integration::IntegrationConnection>(
                    mockConnection1
                );
                integration2 = std::make_shared<UKControllerPlugin::Integration::IntegrationConnection>(
                    mockConnection2
                );
            }

            void SetUp() override
            {
                SetTestNow(std::chrono::system_clock::now());
            }

            std::shared_ptr<UKControllerPlugin::Integration::IntegrationConnection> integration1;
            std::shared_ptr<UKControllerPlugin::Integration::IntegrationConnection> integration2;
            std::shared_ptr<testing::NiceMock<MockConnection>> mockConnection1;
            std::shared_ptr<testing::NiceMock<MockConnection>> mockConnection2;
            std::shared_ptr<IntegrationClientManager> clientManager;
            ClientInitialisationManager initialisationManager;
    };

    TEST_F(ClientInitialisationManagerTest, ItOnlyAddsEachIntegrationOnce)
    {
        initialisationManager.AddConnection(integration1);
        initialisationManager.AddConnection(integration1);
        initialisationManager.AddConnection(integration1);
        initialisationManager.AddConnection(integration2);
        initialisationManager.AddConnection(integration2);
        initialisationManager.AddConnection(integration2);
        EXPECT_EQ(2, initialisationManager.CountConnections());
    }

    TEST_F(ClientInitialisationManagerTest, ItDoesNothingIfNoConnections)
    {
        EXPECT_NO_THROW(initialisationManager.TimedEventTrigger());
    }

    TEST_F(ClientInitialisationManagerTest, ItTimesOutConnectionsThatHaventInitialised)
    {
        SetTestNow(std::chrono::system_clock::now() - std::chrono::seconds(11));
        initialisationManager.AddConnection(integration1);
        initialisationManager.AddConnection(integration2);
        SetTestNow(std::chrono::system_clock::now());
        initialisationManager.TimedEventTrigger();
        EXPECT_EQ(0, initialisationManager.CountConnections());
    }

    TEST_F(ClientInitialisationManagerTest, ItDoesNothingIfNoConnectionMessagesToReceive)
    {
        initialisationManager.AddConnection(integration1);
        initialisationManager.AddConnection(integration2);
        ON_CALL(*this->mockConnection1, Receive)
            .WillByDefault(testing::Return(std::queue<std::string>{}));

        ON_CALL(*this->mockConnection2, Receive)
            .WillByDefault(testing::Return(std::queue<std::string>{}));

        initialisationManager.TimedEventTrigger();
        EXPECT_EQ(2, initialisationManager.CountConnections());
    }

    TEST_F(ClientInitialisationManagerTest, ItInitialisesAClientWithDetails)
    {
        initialisationManager.AddConnection(integration1);
        initialisationManager.AddConnection(integration2);

        nlohmann::json integrationMessage = {
            {"type", "initialise"},
            {"version", 1},
            {
                "data", nlohmann::json::object({
                    {"integration_name", "UKCPTEST"},
                    {"integration_version", "1.5"},
                    {
                        "event_subscriptions", nlohmann::json::array({
                            nlohmann::json::object({{"type", "foo"}, {"version", 1}}),
                            nlohmann::json::object({{"type", "bar"}, {"version", 2}})
                        })
                    }
                })
            }
        };

        std::queue<std::string> returnedMessages;
        returnedMessages.push(integrationMessage.dump());
        ON_CALL(*this->mockConnection1, Receive)
            .WillByDefault(testing::Return(returnedMessages));

        ON_CALL(*this->mockConnection2, Receive)
            .WillByDefault(testing::Return(std::queue<std::string>{}));

        initialisationManager.TimedEventTrigger();
        EXPECT_EQ(1, initialisationManager.CountConnections());
        EXPECT_EQ(1, clientManager->CountClients());
        auto client = *this->clientManager->cbegin();
        EXPECT_EQ("UKCPTEST/1.5", client->GetIntegrationString());
        EXPECT_EQ(1, client->Id());
    }

    TEST_F(ClientInitialisationManagerTest, ItInitialisesAClientWithItsConnection)
    {
        initialisationManager.AddConnection(integration1);
        initialisationManager.AddConnection(integration2);

        nlohmann::json integrationMessage = {
            {"type", "initialise"},
            {"version", 1},
            {
                "data", nlohmann::json::object({
                    {"integration_name", "UKCPTEST"},
                    {"integration_version", "1.5"},
                    {
                        "event_subscriptions", nlohmann::json::array({
                            nlohmann::json::object({{"type", "foo"}, {"version", 1}}),
                            nlohmann::json::object({{"type", "bar"}, {"version", 2}})
                        })
                    }
                })
            }
        };

        std::queue<std::string> returnedMessages;
        returnedMessages.push(integrationMessage.dump());
        ON_CALL(*this->mockConnection1, Receive)
            .WillByDefault(testing::Return(returnedMessages));

        ON_CALL(*this->mockConnection2, Receive)
            .WillByDefault(testing::Return(std::queue<std::string>{}));

        EXPECT_CALL(*this->mockConnection1, Active)
            .Times(1)
            .WillOnce(testing::Return(true));

        EXPECT_CALL(*this->mockConnection2, Active)
            .Times(0);

        initialisationManager.TimedEventTrigger();
        EXPECT_EQ(1, initialisationManager.CountConnections());
        EXPECT_EQ(1, clientManager->CountClients());
        auto client = *this->clientManager->cbegin();
        EXPECT_TRUE(client->Connection()->Active());
    }

    TEST_F(ClientInitialisationManagerTest, ItInitialisesAClientsInterestedEvents)
    {
        initialisationManager.AddConnection(integration1);
        initialisationManager.AddConnection(integration2);

        nlohmann::json integrationMessage = {
            {"type", "initialise"},
            {"version", 1},
            {
                "data", nlohmann::json::object({
                    {"integration_name", "UKCPTEST"},
                    {"integration_version", "1.5"},
                    {
                        "event_subscriptions", nlohmann::json::array({
                            nlohmann::json::object({{"type", "foo"}, {"version", 1}}),
                            nlohmann::json::object({{"type", "bar"}, {"version", 2}})
                        })
                    }
                })
            }
        };

        std::queue<std::string> returnedMessages;
        returnedMessages.push(integrationMessage.dump());
        ON_CALL(*this->mockConnection1, Receive)
            .WillByDefault(testing::Return(returnedMessages));

        ON_CALL(*this->mockConnection2, Receive)
            .WillByDefault(testing::Return(std::queue<std::string>{}));

        initialisationManager.TimedEventTrigger();
        EXPECT_EQ(1, initialisationManager.CountConnections());
        EXPECT_EQ(1, clientManager->CountClients());
        auto client = *this->clientManager->cbegin();
        EXPECT_EQ(2, client->InterestedMessages().size());
        EXPECT_TRUE(client->InterestedInMessage(UKControllerPlugin::Integration::MessageType{"foo", 1}));
        EXPECT_TRUE(client->InterestedInMessage(UKControllerPlugin::Integration::MessageType{"bar", 2}));
    }

    TEST_F(ClientInitialisationManagerTest, ClientsAreAssignedDifferentIds)
    {
        initialisationManager.AddConnection(integration1);
        initialisationManager.AddConnection(integration2);

        nlohmann::json integrationMessage = {
            {"type", "initialise"},
            {"version", 1},
            {
                "data", nlohmann::json::object({
                    {"integration_name", "UKCPTEST"},
                    {"integration_version", "1.5"},
                    {
                        "event_subscriptions", nlohmann::json::array({
                            nlohmann::json::object({{"type", "foo"}, {"version", 1}}),
                            nlohmann::json::object({{"type", "bar"}, {"version", 2}})
                        })
                    }
                })
            }
        };

        std::queue<std::string> returnedMessages;
        returnedMessages.push(integrationMessage.dump());
        ON_CALL(*this->mockConnection1, Receive)
            .WillByDefault(testing::Return(returnedMessages));

        ON_CALL(*this->mockConnection2, Receive)
            .WillByDefault(testing::Return(returnedMessages));

        initialisationManager.TimedEventTrigger();
        EXPECT_EQ(0, initialisationManager.CountConnections());
        EXPECT_EQ(2, clientManager->CountClients());
        auto client1 = *this->clientManager->cbegin();
        auto client2 = *++this->clientManager->cbegin();
        EXPECT_NE(client1->Id(), client2->Id());
    }

    TEST_F(ClientInitialisationManagerTest, ItInitialisesAClientWithAnInitialisedMessage)
    {
        initialisationManager.AddConnection(integration1);

        nlohmann::json integrationMessage = {
            {"type", "initialise"},
            {"version", 1},
            {
                "data", nlohmann::json::object({
                    {"integration_name", "UKCPTEST"},
                    {"integration_version", "1.5"},
                    {
                        "event_subscriptions", nlohmann::json::array({
                            nlohmann::json::object({{"type", "foo"}, {"version", 1}}),
                            nlohmann::json::object({{"type", "bar"}, {"version", 2}})
                        })
                    }
                })
            }
        };

        std::queue<std::string> returnedMessages;
        returnedMessages.push(integrationMessage.dump());
        ON_CALL(*this->mockConnection1, Receive)
            .WillByDefault(testing::Return(returnedMessages));

        EXPECT_CALL(
            *this->mockConnection1,
            Send(UKControllerPlugin::Integration::InitialisationSuccessMessage().ToJson().dump())
        )
            .Times(1);

        initialisationManager.TimedEventTrigger();
    }

    TEST_F(ClientInitialisationManagerTest, ItRejectsIniitialisationMessageWithNoType)
    {
        initialisationManager.AddConnection(integration1);

        nlohmann::json integrationMessage = {
            {"version", 1},
            {
                "data", nlohmann::json::object({
                    {"integration_name", "UKCPTEST"},
                    {"integration_version", "1.5"},
                    {
                        "event_subscriptions", nlohmann::json::array({
                            nlohmann::json::object({{"type", "foo"}, {"version", 1}}),
                            nlohmann::json::object({{"type", "bar"}, {"version", 2}})
                        })
                    }
                })
            }
        };

        std::queue<std::string> returnedMessages;
        returnedMessages.push(integrationMessage.dump());
        EXPECT_CALL(*this->mockConnection1, Receive)
            .Times(1)
            .WillOnce(testing::Return(returnedMessages));

        initialisationManager.TimedEventTrigger();
        EXPECT_EQ(0, this->clientManager->CountClients());
        EXPECT_EQ(1, this->initialisationManager.CountConnections());
    }

    TEST_F(ClientInitialisationManagerTest, ItRejectsIniitialisationMessageWithTypeNotString)
    {
        initialisationManager.AddConnection(integration1);

        nlohmann::json integrationMessage = {
            {"type", 1},
            {"version", 1},
            {
                "data", nlohmann::json::object({
                    {"integration_name", "UKCPTEST"},
                    {"integration_version", "1.5"},
                    {
                        "event_subscriptions", nlohmann::json::array({
                            nlohmann::json::object({{"type", "foo"}, {"version", 1}}),
                            nlohmann::json::object({{"type", "bar"}, {"version", 2}})
                        })
                    }
                })
            }
        };

        std::queue<std::string> returnedMessages;
        returnedMessages.push(integrationMessage.dump());
        EXPECT_CALL(*this->mockConnection1, Receive)
            .Times(1)
            .WillOnce(testing::Return(returnedMessages));

        initialisationManager.TimedEventTrigger();
        EXPECT_EQ(0, this->clientManager->CountClients());
        EXPECT_EQ(1, this->initialisationManager.CountConnections());
    }

    TEST_F(ClientInitialisationManagerTest, ItRejectsIniitialisationMessageWithNoVersion)
    {
        initialisationManager.AddConnection(integration1);

        nlohmann::json integrationMessage = {
            {"type", "initialise"},
            {
                "data", nlohmann::json::object({
                    {"integration_name", "UKCPTEST"},
                    {"integration_version", "1.5"},
                    {
                        "event_subscriptions", nlohmann::json::array({
                            nlohmann::json::object({{"type", "foo"}, {"version", 1}}),
                            nlohmann::json::object({{"type", "bar"}, {"version", 2}})
                        })
                    }
                })
            }
        };

        std::queue<std::string> returnedMessages;
        returnedMessages.push(integrationMessage.dump());
        EXPECT_CALL(*this->mockConnection1, Receive)
            .Times(1)
            .WillOnce(testing::Return(returnedMessages));

        initialisationManager.TimedEventTrigger();
        EXPECT_EQ(0, this->clientManager->CountClients());
        EXPECT_EQ(1, this->initialisationManager.CountConnections());
    }

    TEST_F(ClientInitialisationManagerTest, ItRejectsIniitialisationMessageWithVersionNotInteger)
    {
        initialisationManager.AddConnection(integration1);

        nlohmann::json integrationMessage = {
            {"type", "initialise"},
            {"version", "1"},
            {
                "data", nlohmann::json::object({
                    {"integration_name", "UKCPTEST"},
                    {"integration_version", "1.5"},
                    {
                        "event_subscriptions", nlohmann::json::array({
                            nlohmann::json::object({{"type", "foo"}, {"version", 1}}),
                            nlohmann::json::object({{"type", "bar"}, {"version", 2}})
                        })
                    }
                })
            }
        };

        std::queue<std::string> returnedMessages;
        returnedMessages.push(integrationMessage.dump());
        EXPECT_CALL(*this->mockConnection1, Receive)
            .Times(1)
            .WillOnce(testing::Return(returnedMessages));

        initialisationManager.TimedEventTrigger();
        EXPECT_EQ(0, this->clientManager->CountClients());
        EXPECT_EQ(1, this->initialisationManager.CountConnections());
    }

    TEST_F(ClientInitialisationManagerTest, ItRejectsIniitialisationMessageWithNoData)
    {
        initialisationManager.AddConnection(integration1);

        nlohmann::json integrationMessage = {
            {"type", "initialise"},
            {"version", 1},
        };

        std::queue<std::string> returnedMessages;
        returnedMessages.push(integrationMessage.dump());
        EXPECT_CALL(*this->mockConnection1, Receive)
            .Times(1)
            .WillOnce(testing::Return(returnedMessages));

        initialisationManager.TimedEventTrigger();
        EXPECT_EQ(0, this->clientManager->CountClients());
        EXPECT_EQ(1, this->initialisationManager.CountConnections());
    }

    TEST_F(ClientInitialisationManagerTest, ItRejectsIniitialisationMessageWithDataNotObject)
    {
        initialisationManager.AddConnection(integration1);

        nlohmann::json integrationMessage = {
            {"type", "initialise"},
            {"version", 1},
            {
                "data",
                nlohmann::json::array({"integration_name", "UKCPTEST"})
            }
        };

        std::queue<std::string> returnedMessages;
        returnedMessages.push(integrationMessage.dump());
        EXPECT_CALL(*this->mockConnection1, Receive)
            .Times(1)
            .WillOnce(testing::Return(returnedMessages));

        initialisationManager.TimedEventTrigger();
        EXPECT_EQ(0, this->clientManager->CountClients());
        EXPECT_EQ(1, this->initialisationManager.CountConnections());
    }

    TEST_F(ClientInitialisationManagerTest, ItRejectsIniitialisationMessageIntegrationNameMissing)
    {
        initialisationManager.AddConnection(integration1);

        nlohmann::json integrationMessage = {
            {"type", "initialise"},
            {"version", 1},
            {
                "data", nlohmann::json::object({
                    {"integration_version", "1.5"},
                    {
                        "event_subscriptions", nlohmann::json::array({
                            nlohmann::json::object({{"type", "foo"}, {"version", 1}}),
                            nlohmann::json::object({{"type", "bar"}, {"version", 2}})
                        })
                    }
                })
            }
        };

        std::queue<std::string> returnedMessages;
        returnedMessages.push(integrationMessage.dump());
        EXPECT_CALL(*this->mockConnection1, Receive)
            .Times(1)
            .WillOnce(testing::Return(returnedMessages));

        initialisationManager.TimedEventTrigger();
        EXPECT_EQ(0, this->clientManager->CountClients());
        EXPECT_EQ(1, this->initialisationManager.CountConnections());
    }

    TEST_F(ClientInitialisationManagerTest, ItRejectsIniitialisationMessageWithIntegrationNameNotString)
    {
        initialisationManager.AddConnection(integration1);

        nlohmann::json integrationMessage = {
            {"type", "initialise"},
            {"version", 1},
            {
                "data", nlohmann::json::object({
                    {"integration_name", 123},
                    {"integration_version", "1.5"},
                    {
                        "event_subscriptions", nlohmann::json::array({
                            nlohmann::json::object({{"type", "foo"}, {"version", 1}}),
                            nlohmann::json::object({{"type", "bar"}, {"version", 2}})
                        })
                    }
                })
            }
        };

        std::queue<std::string> returnedMessages;
        returnedMessages.push(integrationMessage.dump());
        EXPECT_CALL(*this->mockConnection1, Receive)
            .Times(1)
            .WillOnce(testing::Return(returnedMessages));

        initialisationManager.TimedEventTrigger();
        EXPECT_EQ(0, this->clientManager->CountClients());
        EXPECT_EQ(1, this->initialisationManager.CountConnections());
    }

    TEST_F(ClientInitialisationManagerTest, ItRejectsIniitialisationMessageWithIntegrationVersionMissing)
    {
        initialisationManager.AddConnection(integration1);

        nlohmann::json integrationMessage = {
            {"type", "initialise"},
            {"version", 1},
            {
                "data", nlohmann::json::object({
                    {"integration_name", "UKCPTEST"},
                    {
                        "event_subscriptions", nlohmann::json::array({
                            nlohmann::json::object({{"type", "foo"}, {"version", 1}}),
                            nlohmann::json::object({{"type", "bar"}, {"version", 2}})
                        })
                    }
                })
            }
        };

        std::queue<std::string> returnedMessages;
        returnedMessages.push(integrationMessage.dump());
        EXPECT_CALL(*this->mockConnection1, Receive)
            .Times(1)
            .WillOnce(testing::Return(returnedMessages));

        initialisationManager.TimedEventTrigger();
        EXPECT_EQ(0, this->clientManager->CountClients());
        EXPECT_EQ(1, this->initialisationManager.CountConnections());
    }

    TEST_F(ClientInitialisationManagerTest, ItRejectsIniitialisationMessageWithIntegrationVersionNotAString)
    {
        initialisationManager.AddConnection(integration1);

        nlohmann::json integrationMessage = {
            {"type", "initialise"},
            {"version", 1},
            {
                "data", nlohmann::json::object({
                    {"integration_name", "UKCPTEST"},
                    {"integration_version", 1},
                    {
                        "event_subscriptions", nlohmann::json::array({
                            nlohmann::json::object({{"type", "foo"}, {"version", 1}}),
                            nlohmann::json::object({{"type", "bar"}, {"version", 2}})
                        })
                    }
                })
            }
        };

        std::queue<std::string> returnedMessages;
        returnedMessages.push(integrationMessage.dump());
        EXPECT_CALL(*this->mockConnection1, Receive)
            .Times(1)
            .WillOnce(testing::Return(returnedMessages));

        initialisationManager.TimedEventTrigger();
        EXPECT_EQ(0, this->clientManager->CountClients());
        EXPECT_EQ(1, this->initialisationManager.CountConnections());
    }

    TEST_F(ClientInitialisationManagerTest, ItRejectsIniitialisationMessageWithEventSubscriptionsMissing)
    {
        initialisationManager.AddConnection(integration1);

        nlohmann::json integrationMessage = {
            {"type", "initialise"},
            {"version", 1},
            {
                "data", nlohmann::json::object({
                    {"integration_name", "UKCPTEST"},
                    {"integration_version", "1.5"},
                })
            }
        };

        std::queue<std::string> returnedMessages;
        returnedMessages.push(integrationMessage.dump());
        EXPECT_CALL(*this->mockConnection1, Receive)
            .Times(1)
            .WillOnce(testing::Return(returnedMessages));

        initialisationManager.TimedEventTrigger();
        EXPECT_EQ(0, this->clientManager->CountClients());
        EXPECT_EQ(1, this->initialisationManager.CountConnections());
    }

    TEST_F(ClientInitialisationManagerTest, ItRejectsIniitialisationMessageWithEventSubscriptionsNotArray)
    {
        initialisationManager.AddConnection(integration1);

        nlohmann::json integrationMessage = {
            {"type", "initialise"},
            {"version", 1},
            {
                "data", nlohmann::json::object({
                    {"integration_name", "UKCPTEST"},
                    {"integration_version", "1.5"},
                    {
                        "event_subscriptions", nlohmann::json::object({{"foo", "bar"}})
                    }
                })
            }
        };

        std::queue<std::string> returnedMessages;
        returnedMessages.push(integrationMessage.dump());
        EXPECT_CALL(*this->mockConnection1, Receive)
            .Times(1)
            .WillOnce(testing::Return(returnedMessages));

        initialisationManager.TimedEventTrigger();
        EXPECT_EQ(0, this->clientManager->CountClients());
        EXPECT_EQ(1, this->initialisationManager.CountConnections());
    }

    TEST_F(ClientInitialisationManagerTest, ItRejectsIniitialisationMessageWithTypeMissing)
    {
        initialisationManager.AddConnection(integration1);

        nlohmann::json integrationMessage = {
            {"type", "initialise"},
            {"version", 1},
            {
                "data", nlohmann::json::object({
                    {"integration_name", "UKCPTEST"},
                    {"integration_version", "1.5"},
                    {
                        "event_subscriptions", nlohmann::json::array({
                            nlohmann::json::object({{"version", 1}}),
                            nlohmann::json::object({{"type", "bar"}, {"version", 2}})
                        })
                    }
                })
            }
        };

        std::queue<std::string> returnedMessages;
        returnedMessages.push(integrationMessage.dump());
        EXPECT_CALL(*this->mockConnection1, Receive)
            .Times(1)
            .WillOnce(testing::Return(returnedMessages));

        initialisationManager.TimedEventTrigger();
        EXPECT_EQ(0, this->clientManager->CountClients());
        EXPECT_EQ(1, this->initialisationManager.CountConnections());
    }

    TEST_F(ClientInitialisationManagerTest, ItRejectsIniitialisationMessageWithEventTypeTypeNotAString)
    {
        initialisationManager.AddConnection(integration1);

        nlohmann::json integrationMessage = {
            {"type", "initialise"},
            {"version", 1},
            {
                "data", nlohmann::json::object({
                    {"integration_name", "UKCPTEST"},
                    {"integration_version", "1.5"},
                    {
                        "event_subscriptions", nlohmann::json::array({
                            nlohmann::json::object({{"type", 1}, {"version", 1}}),
                            nlohmann::json::object({{"type", "bar"}, {"version", 2}})
                        })
                    }
                })
            }
        };

        std::queue<std::string> returnedMessages;
        returnedMessages.push(integrationMessage.dump());
        EXPECT_CALL(*this->mockConnection1, Receive)
            .Times(1)
            .WillOnce(testing::Return(returnedMessages));

        initialisationManager.TimedEventTrigger();
        EXPECT_EQ(0, this->clientManager->CountClients());
        EXPECT_EQ(1, this->initialisationManager.CountConnections());
    }

    TEST_F(ClientInitialisationManagerTest, ItRejectsIniitialisationMessageWithNoEventVersion)
    {
        initialisationManager.AddConnection(integration1);

        nlohmann::json integrationMessage = {
            {"type", "initialise"},
            {"version", 1},
            {
                "data", nlohmann::json::object({
                    {"integration_name", "UKCPTEST"},
                    {"integration_version", "1.5"},
                    {
                        "event_subscriptions", nlohmann::json::array({
                            nlohmann::json::object({{"type", "foo"}, {"version", 1}}),
                            nlohmann::json::object({{"type", "bar"}})
                        })
                    }
                })
            }
        };

        std::queue<std::string> returnedMessages;
        returnedMessages.push(integrationMessage.dump());
        EXPECT_CALL(*this->mockConnection1, Receive)
            .Times(1)
            .WillOnce(testing::Return(returnedMessages));

        initialisationManager.TimedEventTrigger();
        EXPECT_EQ(0, this->clientManager->CountClients());
        EXPECT_EQ(1, this->initialisationManager.CountConnections());
    }

    TEST_F(ClientInitialisationManagerTest, ItRejectsIniitialisationMessageWithEventVersionNotAnInteger)
    {
        initialisationManager.AddConnection(integration1);

        nlohmann::json integrationMessage = {
            {"type", "initialise"},
            {"version", 1},
            {
                "data", nlohmann::json::object({
                    {"integration_name", "UKCPTEST"},
                    {"integration_version", "1.5"},
                    {
                        "event_subscriptions", nlohmann::json::array({
                            nlohmann::json::object({{"type", "foo"}, {"version", 1}}),
                            nlohmann::json::object({{"type", "bar"}, {"version", "123"}})
                        })
                    }
                })
            }
        };

        std::queue<std::string> returnedMessages;
        returnedMessages.push(integrationMessage.dump());
        EXPECT_CALL(*this->mockConnection1, Receive)
            .Times(1)
            .WillOnce(testing::Return(returnedMessages));

        initialisationManager.TimedEventTrigger();
        EXPECT_EQ(0, this->clientManager->CountClients());
        EXPECT_EQ(1, this->initialisationManager.CountConnections());
    }
} // namespace UKControllerPluginTest::Integration
