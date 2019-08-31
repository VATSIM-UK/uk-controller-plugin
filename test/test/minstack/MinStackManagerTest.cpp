#include "pch/pch.h"
#include "minstack/MinStackManager.h"
#include "mock/MockTerminalControlArea.h"
#include "minstack/MinStackLevel.h"
#include "websocket/WebsocketSubscription.h"
#include "websocket/WebsocketMessage.h"

using UKControllerPluginTest::MinStack::MockTerminalControlArea;
using UKControllerPlugin::MinStack::TerminalControlArea;
using UKControllerPlugin::MinStack::MinStackManager;
using UKControllerPlugin::MinStack::MinStackLevel;
using UKControllerPlugin::Websocket::WebsocketSubscription;
using UKControllerPlugin::Websocket::WebsocketMessage;
using ::testing::StrictMock;
using ::testing::Return;
using ::testing::Test;

namespace UKControllerPluginTest {
    namespace MinStack {

        class MinStackManagerTest : public Test
        {
            public: 
                MinStackManager msl;
        };

        TEST_F(MinStackManagerTest, ItGeneratesTmaKeys)
        {
            EXPECT_EQ("tma.LTMA", this->msl.GetMslKeyTma("LTMA"));
        }

        TEST_F(MinStackManagerTest, ItGeneratesAirfieldKeys)
        {
            EXPECT_EQ("airfield.EGGD", this->msl.GetMslKeyAirfield("EGGD"));
        }

        TEST_F(MinStackManagerTest, ItCanStoreUserAcknowledgementsOfMinStackLevels)
        {
            this->msl.AddMsl(msl.GetMslKeyTma("LTMA"), "tma", "LTMA", 8000);
            EXPECT_FALSE(this->msl.GetMinStackLevel(msl.GetMslKeyTma("LTMA")).IsAcknowledged());
            this->msl.AcknowledgeMsl(msl.GetMslKeyTma("LTMA"));
            EXPECT_TRUE(this->msl.GetMinStackLevel(msl.GetMslKeyTma("LTMA")).IsAcknowledged());
        }

        TEST_F(MinStackManagerTest, ItReturnsAllThePossibleKeysForMinStacks)
        {
            this->msl.AddMsl(this->msl.GetMslKeyTma("LTMA"), "tma", "LTMA", 8000);
            this->msl.AddMsl(this->msl.GetMslKeyTma("STMA"), "tma", "STMA", 8000);


            std::set<std::string> expectedKeys = { this->msl.GetMslKeyTma("LTMA"), this->msl.GetMslKeyTma("STMA") };
            EXPECT_EQ(expectedKeys, this->msl.GetAllMslKeys());
        }

        TEST_F(MinStackManagerTest, ItReturnsInvalidMslIfKeyNotFound)
        {
            EXPECT_EQ(this->msl.invalidMsl, this->msl.GetMinStackLevel("nope"));
        }

        TEST_F(MinStackManagerTest, ItReturnsAMsl)
        {
            this->msl.AddMsl(this->msl.GetMslKeyTma("LTMA"), "tma", "LTMA", 8000);
            MinStackLevel minStack = this->msl.GetMinStackLevel(this->msl.GetMslKeyTma("LTMA"));

            EXPECT_EQ(8000, minStack.msl);
            EXPECT_EQ("tma", minStack.type);
            EXPECT_EQ("LTMA", minStack.name);
            EXPECT_FALSE(minStack.IsAcknowledged());
        }

        TEST_F(MinStackManagerTest, ItHasWebsocketSubscriptions)
        {
            std::set<WebsocketSubscription> expected = {
                {
                    WebsocketSubscription::SUB_TYPE_CHANNEL,
                    "private-minstack-updates"
                }
            };
            EXPECT_EQ(expected, this->msl.GetSubscriptions());
        }

        TEST_F(MinStackManagerTest, ItAllowsManualMinStackUpdates)
        {
            this->msl.AddMsl(this->msl.GetMslKeyTma("LTMA"), "tma", "LTMA", 8000);
            this->msl.SetMinStackLevel(this->msl.GetMslKeyTma("LTMA"), 9000);

            EXPECT_EQ(9000, this->msl.GetMinStackLevel(this->msl.GetMslKeyTma("LTMA")).msl);
        }

        TEST_F(MinStackManagerTest, ItUpdatesMinStackLevelsFromJson)
        {
            nlohmann::json mslData;
            mslData["airfield"] = {
                {"EGBB", 7000}
            };
            mslData["tma"] = {
                {"MTMA", 6000}
            };

            WebsocketMessage message{
                "App\\Events\\MinStacksUpdatedEvent",
                "private-minstack-updates",
                mslData,
                false
            };

            this->msl.ProcessWebsocketMessage(message);
            EXPECT_EQ(7000, this->msl.GetMinStackLevel(this->msl.GetMslKeyAirfield("EGBB")).msl);
            EXPECT_EQ(6000, this->msl.GetMinStackLevel(this->msl.GetMslKeyTma("MTMA")).msl);
        }

        TEST_F(MinStackManagerTest, ItDoesntDoManualMinStackUpdatesIfItDoesntExist)
        {
            EXPECT_NO_THROW(this->msl.SetMinStackLevel("nope", 8000));
        }

        TEST(MinStack, HasTerminalControlAreaReturnsTrueIfTerminalControlAreaKnown)
        {
            std::shared_ptr<StrictMock<MockTerminalControlArea>> ltmaMock(
                new StrictMock<MockTerminalControlArea>(L"LTMA", "LTMA", 6000, "EGLL", true)
            );

            EXPECT_CALL(*ltmaMock, GetCharName())
                .Times(2)
                .WillRepeatedly(Return(const_cast<char *>("LTMA")));

            MinStackManager msl;
            msl.AddTerminalControlArea(ltmaMock);

            EXPECT_TRUE(msl.HasTerminalControlArea("LTMA"));
        }

        TEST(MinStack, HasTerminalControlAreaReturnsFalseIfTerminalControlAreaNotKnown)
        {
            std::shared_ptr<StrictMock<MockTerminalControlArea>> ltmaMock(
                new StrictMock<MockTerminalControlArea>(L"LTMA", "LTMA", 6000, "EGLL", true)
            );

            EXPECT_CALL(*ltmaMock, GetCharName())
                .Times(2)
                .WillRepeatedly(Return(const_cast<char *>("LTMA")));

            MinStackManager msl;
            msl.AddTerminalControlArea(ltmaMock);
            EXPECT_FALSE(msl.HasTerminalControlArea("OMDB"));
        }

        TEST(MinStack, IsConcernedAirfieldReturnsTrueIfAirfieldConcerned)
        {
            std::shared_ptr<StrictMock<MockTerminalControlArea>> ltmaMock(
                new StrictMock<MockTerminalControlArea>(L"LTMA", "LTMA", 6000, "EGLL", true)
            );

            EXPECT_CALL(*ltmaMock, GetCharName())
                .Times(2)
                .WillRepeatedly(Return(const_cast<char *>("LTMA")));

            MinStackManager msl;
            msl.AddTerminalControlArea(ltmaMock);
            EXPECT_TRUE(msl.IsConcernedAirfield("EGLL"));
        }

        TEST(MinStack, IsConcernedAirfieldReturnsFalseIfAirfieldNotConcerned)
        {
            std::shared_ptr<StrictMock<MockTerminalControlArea>> ltmaMock(
                new StrictMock<MockTerminalControlArea>(L"LTMA", "LTMA", 6000, "EGLL", true)
            );

            EXPECT_CALL(*ltmaMock, GetCharName())
                .Times(2)
                .WillRepeatedly(Return(const_cast<char *>("LTMA")));

            MinStackManager msl;
            msl.AddTerminalControlArea(ltmaMock);
            EXPECT_FALSE(msl.IsConcernedAirfield("ESSEX"));
        }

        TEST(MinStack, GetAllTmasReturnsCorrectData)
        {
            // Createsome mock TMAs
            std::shared_ptr<StrictMock<MockTerminalControlArea>> ltmaMock(
                new StrictMock<MockTerminalControlArea>(L"LTMA", "LTMA", 6000, "EGLL", true)
            );
            std::shared_ptr<StrictMock<MockTerminalControlArea>> mtmaMock(
                new StrictMock<MockTerminalControlArea>(L"MTMA", "MTMA", 5000, "EGCC", true)
            );
            std::shared_ptr<StrictMock<MockTerminalControlArea>> stmaMock(
                new StrictMock<MockTerminalControlArea>(L"STMA", "STMA", 6000, "EGPF", false)
            );

            EXPECT_CALL(*ltmaMock, GetCharName())
                .Times(2)
                .WillRepeatedly(Return(const_cast<char *>("LTMA")));

            EXPECT_CALL(*mtmaMock, GetCharName())
                .Times(2)
                .WillRepeatedly(Return(const_cast<char *>("MTMA")));

            EXPECT_CALL(*stmaMock, GetCharName())
                .Times(2)
                .WillRepeatedly(Return(const_cast<char *>("STMA")));

            MinStackManager msl;
            msl.AddTerminalControlArea(ltmaMock);
            msl.AddTerminalControlArea(mtmaMock);
            msl.AddTerminalControlArea(stmaMock);
            std::vector<std::shared_ptr<const TerminalControlArea>> data = msl.GetAllTmas();

            EXPECT_EQ(data[0], ltmaMock);
            EXPECT_EQ(data[1], mtmaMock);
            EXPECT_EQ(data[2], stmaMock);
        }

        TEST(MinStack, MinStackClickedDoesNothingIfTmaNotKnown)
        {
            // Create a mock of the LTMA
            std::shared_ptr<StrictMock<MockTerminalControlArea>> ltmaMock(
                new StrictMock<MockTerminalControlArea>(L"LTMA", "LTMA", 6000, "EGLL", true)
            );

            EXPECT_CALL(*ltmaMock, AcknowledgeMinStack())
                .Times(0);

            EXPECT_CALL(*ltmaMock, GetCharName())
                .Times(2)
                .WillRepeatedly(Return(const_cast<char *>("LTMA")));


            // Run the test
            MinStackManager msl;
            msl.AddTerminalControlArea(ltmaMock);
            msl.MinStackClicked("STMA");
        }

        TEST(MinStack, MinStackClickedAcknowledgesTheMinStackOfTheGivenTma)
        {
            // Create a mock of the LTMA
            std::shared_ptr<StrictMock<MockTerminalControlArea>> ltmaMock(
                new StrictMock<MockTerminalControlArea>(L"LTMA", "LTMA", 6000, "EGLL", true)
            );

            EXPECT_CALL(*ltmaMock, AcknowledgeMinStack())
                .Times(1);

            EXPECT_CALL(*ltmaMock, GetCharName())
                .Times(2)
                .WillRepeatedly(Return(const_cast<char *>("LTMA")));

            // Create a mock of the MTMA
            std::shared_ptr<StrictMock<MockTerminalControlArea>> mtmaMock(
                new StrictMock<MockTerminalControlArea>(L"MTMA", "MTMA", 5000, "EGCC", true)
            );

            EXPECT_CALL(*mtmaMock, GetCharName())
                .Times(2)
                .WillRepeatedly(Return(const_cast<char *>("MTMA")));

            // Run the test
            MinStackManager msl;
            msl.AddTerminalControlArea(ltmaMock);
            msl.AddTerminalControlArea(mtmaMock);
            msl.MinStackClicked("LTMA");
        }

        TEST(MinStack, NewMetarGetTerminalControlAreaForAirfieldDoesNothingIfTmaNotKnown)
        {
            // Create a mock of the LTMA
            std::shared_ptr<StrictMock<MockTerminalControlArea>> ltmaMock(
                new StrictMock<MockTerminalControlArea>(L"LTMA", "LTMA", 6000, "EGLL", true)
            );

            EXPECT_CALL(*ltmaMock, GetCharName())
                .Times(2)
                .WillRepeatedly(Return(const_cast<char *>("LTMA")));

            // Run the test
            MinStackManager msl;
            msl.AddTerminalControlArea(ltmaMock);
            msl.NewMetar("EGFF", "EGFF 122250Z AUTO 25003KT 9999 NCD 16/12 Q1022");
        }

        TEST(MinStack, NewMetarProcessMetarDoesNothingIfMetarInvalid)
        {
            // Create a mock of the LTMA
            std::shared_ptr<StrictMock<MockTerminalControlArea>> ltmaMock(
                new StrictMock<MockTerminalControlArea>(L"LTMA", "LTMA", 6000, "EGLL", true)
            );

            EXPECT_CALL(*ltmaMock, GetCharName())
                .Times(2)
                .WillRepeatedly(Return(const_cast<char *>("LTMA")));

            // Run the test
            MinStackManager msl;
            msl.AddTerminalControlArea(ltmaMock);
            msl.NewMetar("EGFF", "EGFF 122250Z AUTO 25003KT 9999 NCD 16/12");
        }

        TEST(MinStack, NewMetarProcessMetarDoesNothingIfMetarEmptyString)
        {
            // Create a mock of the LTMA
            std::shared_ptr<StrictMock<MockTerminalControlArea>> ltmaMock(
                new StrictMock<MockTerminalControlArea>(L"LTMA", "LTMA", 6000, "EGLL", true)
            );

            EXPECT_CALL(*ltmaMock, GetCharName())
                .Times(2)
                .WillRepeatedly(Return(const_cast<char *>("LTMA")));

            // Run the test
            MinStackManager msl;
            msl.AddTerminalControlArea(ltmaMock);
            msl.NewMetar("EGLL", "");
        }

        TEST(MinStack, NewMetarProcessMetarCallsSetMinStackIfMetarValid)
        {
            // Create a mock of the LTMA
            std::shared_ptr<StrictMock<MockTerminalControlArea>> ltmaMock(
                new StrictMock<MockTerminalControlArea>(L"LTMA", "LTMA", 6000, "EGLL", true)
            );

            EXPECT_CALL(*ltmaMock, SetMinStack(1013))
                .Times(1);

            EXPECT_CALL(*ltmaMock, GetCharName())
                .Times(2)
                .WillRepeatedly(Return(const_cast<char *>("LTMA")));

            // Run the test
            MinStackManager msl;
            msl.AddTerminalControlArea(ltmaMock);
            msl.NewMetar("EGLL", "EGLL 122250Z AUTO 25003KT 9999 NCD 16/12 Q1013");
        }
    }  // namespace MinStack
}  // namespace UKControllerPluginTest
