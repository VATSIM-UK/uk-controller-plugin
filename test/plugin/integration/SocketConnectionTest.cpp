#include "pch/pch.h"
#include "integration/SocketConnection.h"
#include "mock/MockSocket.h"

using testing::Test;
using UKControllerPlugin::Integration::SocketConnection;

namespace UKControllerPluginTest::Integration {

    class SocketConnectionTest : public Test
    {
        public:
            SocketConnectionTest()
                : mockSocket(new testing::NiceMock<MockSocket>),
                  connection(mockSocket)
            { }

            std::shared_ptr<testing::NiceMock<MockSocket>> mockSocket;
            SocketConnection connection;
    };

    TEST_F(SocketConnectionTest, ItReturnsActiveIfTheSocketIsActive)
    {
        ON_CALL(*this->mockSocket, Active)
            .WillByDefault(testing::Return(true));

        EXPECT_TRUE(this->connection.Active());
    }

    TEST_F(SocketConnectionTest, ItReturnsInactiveIfTheSocketIsInactive)
    {
        ON_CALL(*this->mockSocket, Active)
            .WillByDefault(testing::Return(false));

        EXPECT_FALSE(this->connection.Active());
    }

    TEST_F(SocketConnectionTest, ItSendsAMessageFollowedByDelimiter)
    {
        std::string expectedMessage = "testmessage";
        expectedMessage.append({'\x1F'});

        EXPECT_CALL(*this->mockSocket, InsertStringOverride(expectedMessage))
            .Times(1);

        this->connection.Send("testmessage");
    }

    TEST_F(SocketConnectionTest, ItReceivesNoMessageFromTheSocket)
    {
        EXPECT_CALL(*this->mockSocket, ExtractStreamOverride())
            .Times(1)
            .WillOnce(testing::Return(""));


        EXPECT_EQ(0, this->connection.Receive().size());
    }

    TEST_F(SocketConnectionTest, ItReceivesASingleMessageOffTheSocket)
    {
        std::string messageString = "testmessage1";
        messageString.append({'\x1F'});

        EXPECT_CALL(*this->mockSocket, ExtractStreamOverride())
            .Times(1)
            .WillOnce(testing::Return(messageString));

        std::queue<std::string> expected;
        expected.push("testmessage1");

        EXPECT_EQ(expected, this->connection.Receive());
    }

    TEST_F(SocketConnectionTest, ItReceivesASingleMessageAfterNoDataOffTheSocket)
    {
        std::string messageString = "testmessage1";
        messageString.append({'\x1F'});

        EXPECT_CALL(*this->mockSocket, ExtractStreamOverride())
            .Times(3)
            .WillOnce(testing::Return(""))
            .WillOnce(testing::Return(""))
            .WillOnce(testing::Return(messageString));

        std::queue<std::string> expected;
        expected.push("testmessage1");

        EXPECT_EQ(0, this->connection.Receive().size());
        EXPECT_EQ(0, this->connection.Receive().size());
        EXPECT_EQ(expected, this->connection.Receive());
    }

    TEST_F(SocketConnectionTest, ItReceivesMulitpleMessagesAtOnceOffTheSocket)
    {
        std::string messageString = "testmessage1";
        messageString.append({'\x1F'});
        messageString.append("testmessage2");
        messageString.append({'\x1F'});

        EXPECT_CALL(*this->mockSocket, ExtractStreamOverride())
            .Times(1)
            .WillOnce(testing::Return(messageString));

        std::queue<std::string> expected;
        expected.push("testmessage1");
        expected.push("testmessage2");

        EXPECT_EQ(expected, this->connection.Receive());
    }

    TEST_F(SocketConnectionTest, ItDiscardsReceivedMessagesOnceTaken)
    {
        std::string messageString = "testmessage1";
        messageString.append({'\x1F'});

        EXPECT_CALL(*this->mockSocket, ExtractStreamOverride())
            .Times(2)
            .WillOnce(testing::Return(messageString))
            .WillOnce(testing::Return(""));

        std::queue<std::string> expected1;
        expected1.push("testmessage1");

        EXPECT_EQ(expected1, this->connection.Receive());
        EXPECT_EQ(0, this->connection.Receive().size());
    }

    TEST_F(SocketConnectionTest, ItHandlesPartialMessagesFromTheSocket)
    {
        std::string messageStringPart1 = "testmess";
        std::string messageStringPart2 = "ag";
        std::string messageStringPart3 = "e";
        messageStringPart3.append({'\x1F'});

        EXPECT_CALL(*this->mockSocket, ExtractStreamOverride)
            .Times(4)
            .WillOnce(testing::Return(messageStringPart1))
            .WillOnce(testing::Return(""))
            .WillOnce(testing::Return(messageStringPart2))
            .WillOnce(testing::Return(messageStringPart3));

        std::queue<std::string> eventualExpected;
        eventualExpected.push("testmessage");

        EXPECT_EQ(0, this->connection.Receive().size());
        EXPECT_EQ(0, this->connection.Receive().size());
        EXPECT_EQ(0, this->connection.Receive().size());
        EXPECT_EQ(eventualExpected, this->connection.Receive());
    }

    TEST_F(SocketConnectionTest, ItHandlesMultiplePartialMessagesFromTheSocket)
    {
        std::string messageStringPart1 = "testmess";
        std::string messageStringPart2 = "ag";
        std::string messageStringPart3 = "e";
        messageStringPart3.append({'\x1F'});
        messageStringPart3.append("test");
        std::string messageStringPart4 = "message2";
        messageStringPart4.append({'\x1F'});

        EXPECT_CALL(*this->mockSocket, ExtractStreamOverride)
            .Times(6)
            .WillOnce(testing::Return(messageStringPart1))
            .WillOnce(testing::Return(""))
            .WillOnce(testing::Return(messageStringPart2))
            .WillOnce(testing::Return(messageStringPart3))
            .WillOnce(testing::Return(""))
            .WillOnce(testing::Return(messageStringPart4));

        std::queue<std::string> firstExpected;
        firstExpected.push("testmessage");

        std::queue<std::string> secondExpected;
        secondExpected.push("testmessage2");

        EXPECT_EQ(0, this->connection.Receive().size());
        EXPECT_EQ(0, this->connection.Receive().size());
        EXPECT_EQ(0, this->connection.Receive().size());
        EXPECT_EQ(firstExpected, this->connection.Receive());
        EXPECT_EQ(0, this->connection.Receive().size());
        EXPECT_EQ(secondExpected, this->connection.Receive());
    }
} // namespace UKControllerPluginTest::Integration
