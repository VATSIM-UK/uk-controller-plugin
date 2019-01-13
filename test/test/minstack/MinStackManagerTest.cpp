#include "pch/pch.h"
#include "minstack/MinStackManager.h"
#include "mock/MockTerminalControlArea.h"

using UKControllerPluginTest::MinStack::MockTerminalControlArea;
using UKControllerPlugin::MinStack::TerminalControlArea;
using UKControllerPlugin::MinStack::MinStackManager;
using ::testing::StrictMock;
using ::testing::Return;

namespace UKControllerPluginTest {
    namespace MinStack {

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
