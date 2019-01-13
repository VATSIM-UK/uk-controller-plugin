#include "pch/pch.h"
#include "minstack/TerminalControlArea.h"

using UKControllerPlugin::MinStack::TerminalControlArea;

namespace UKControllerPluginTest {
    namespace MinStack {
        TEST(TerminalControlArea, AcknowledgeMinStackSetsUserAcknowledgedTrue)
        {
            TerminalControlArea tma(L"LTMA", "LTMA", 6000, "EGLL", true);
            tma.AcknowledgeMinStack();
            EXPECT_TRUE(tma.MinStackAcknowledged());
        }

        TEST(TerminalControlArea, ConstructorInitialisesCurrentMinStackIntToZero)
        {
            TerminalControlArea tma(L"LTMA", "LTMA", 0, "EGLL", true);
            EXPECT_EQ(0, tma.GetCurrentMinStackInt());
        }

        TEST(TerminalControlArea, ConstructorInitialisesCurrentMinStackToDash)
        {
            TerminalControlArea tma(L"LTMA", "LTMA", 0, "EGLL", true);
            EXPECT_EQ(0, lstrcmp(L"-", tma.GetCurrentMinStackDisplay()));
        }

        TEST(TerminalControlArea, ConstructorInitialisesUserAcknowledgedToFalse)
        {
            TerminalControlArea tma(L"LTMA", "LTMA", 0, "EGLL", true);
            EXPECT_FALSE(tma.MinStackAcknowledged());
        }


        TEST(TerminalControlArea, ConstructorInitialisesCalculationAirfieldToConstructorArgs)
        {
            TerminalControlArea tma(L"LTMA", "LTMA", 0, "EGLL", true);
            EXPECT_EQ(0, tma.GetCalculationAirfield().compare("EGLL"));
        }

        TEST(TerminalControlArea, SetMinStackSetsCorrectIntValueHighPressure)
        {
            TerminalControlArea tma(L"LTMA", "LTMA", 6000, "EGLL", true);
            tma.SetMinStack(1029);
            EXPECT_EQ(70, tma.GetCurrentMinStackInt());
        }

        TEST(TerminalControlArea, SetMinStackSetsCorrectIntValueStandardAndStandardIsHigh)
        {
            TerminalControlArea tma(L"LTMA", "LTMA", 6000, "EGLL", true);
            tma.SetMinStack(1013);
            EXPECT_EQ(70, tma.GetCurrentMinStackInt());
        }

        TEST(TerminalControlArea, SetMinStackSetsCorrectIntValueStandardAndStandardIsLow)
        {
            TerminalControlArea tma(L"LTMA", "LTMA", 6000, "EGLL", false);
            tma.SetMinStack(1013);
            EXPECT_EQ(80, tma.GetCurrentMinStackInt());
        }


        TEST(TerminalControlArea, SetMinStackSetsCorrectIntValueLowPressureUpperBoundary)
        {
            TerminalControlArea tma(L"LTMA", "LTMA", 6000, "EGLL", true);
            tma.SetMinStack(1012);
            EXPECT_EQ(80, tma.GetCurrentMinStackInt());
        }

        TEST(TerminalControlArea, SetMinStackSetsCorrectIntValueLowPressure)
        {
            TerminalControlArea tma(L"LTMA", "LTMA", 6000, "EGLL", true);
            tma.SetMinStack(1000);
            EXPECT_EQ(80, tma.GetCurrentMinStackInt());
        }

        TEST(TerminalControlArea, SetMinStackSetsCorrectIntValueLowPressureLowerBoundary)
        {
            TerminalControlArea tma(L"LTMA", "LTMA", 6000, "EGLL", true);
            tma.SetMinStack(978);
            EXPECT_EQ(80, tma.GetCurrentMinStackInt());
        }

        TEST(TerminalControlArea, SetMinStackSetsCorrectIntValueDoubleLowPressureUpperBoundary)
        {
            TerminalControlArea tma(L"LTMA", "LTMA", 6000, "EGLL", true);
            tma.SetMinStack(977);
            EXPECT_EQ(90, tma.GetCurrentMinStackInt());
        }

        TEST(TerminalControlArea, SetMinStackSetsCorrectIntValueDoubleLowPressure)
        {
            TerminalControlArea tma(L"LTMA", "LTMA", 6000, "EGLL", true);
            tma.SetMinStack(950);
            EXPECT_EQ(90, tma.GetCurrentMinStackInt());
        }

        TEST(TerminalControlArea, SetMinStackSetsCorrectDisplayValueHighPressure)
        {
            TerminalControlArea tma(L"LTMA", "LTMA", 6000, "EGLL", true);
            tma.SetMinStack(1029);
            EXPECT_EQ(0, lstrcmp(L"70", tma.GetCurrentMinStackDisplay()));
        }

        TEST(TerminalControlArea, SetMinStackSetsCorrectDisplayValueStandardPressureAndStandardIsHigh)
        {
            TerminalControlArea tma(L"LTMA", "LTMA", 6000, "EGLL", true);
            tma.SetMinStack(1013);
            EXPECT_EQ(0, lstrcmp(L"70", tma.GetCurrentMinStackDisplay()));
        }

        TEST(TerminalControlArea, SetMinStackSetsCorrectDisplayValueStandardPressureAndStandardIsLow)
        {
            TerminalControlArea tma(L"STMA", "STMA", 6000, "EGLL", false);
            tma.SetMinStack(1013);
            EXPECT_EQ(0, lstrcmp(L"80", tma.GetCurrentMinStackDisplay()));
        }

        TEST(TerminalControlArea, SetMinStackSetsCorrectDisplayValueLowPressureUpperBoundary)
        {
            TerminalControlArea tma(L"LTMA", "LTMA", 6000, "EGLL", true);
            tma.SetMinStack(1012);
            EXPECT_EQ(0, lstrcmp(L"80", tma.GetCurrentMinStackDisplay()));
        }

        TEST(TerminalControlArea, SetMinStackSetsCorrectDisplayValueLowPressure)
        {
            TerminalControlArea tma(L"LTMA", "LTMA", 6000, "EGLL", true);
            tma.SetMinStack(1000);
            EXPECT_EQ(0, lstrcmp(L"80", tma.GetCurrentMinStackDisplay()));
        }

        TEST(TerminalControlArea, SetMinStackSetsCorrectDisplayValueLowPressureLowerBoundary)
        {
            TerminalControlArea tma(L"LTMA", "LTMA", 6000, "EGLL", true);
            tma.SetMinStack(978);
            EXPECT_EQ(0, lstrcmp(L"80", tma.GetCurrentMinStackDisplay()));
        }

        TEST(TerminalControlArea, SetMinStackSetsCorrectDisplayValueDoubleLowPressureUpperBoundary)
        {
            TerminalControlArea tma(L"LTMA", "LTMA", 6000, "EGLL", true);
            tma.SetMinStack(977);
            EXPECT_EQ(0, lstrcmp(L"90", tma.GetCurrentMinStackDisplay()));
        }

        TEST(TerminalControlArea, SetMinStackSetsCorrectDisplayValueDoubleLowPressure)
        {
            TerminalControlArea tma(L"LTMA", "LTMA", 6000, "EGLL", true);
            tma.SetMinStack(950);
            EXPECT_EQ(0, lstrcmp(L"90", tma.GetCurrentMinStackDisplay()));
        }


        TEST(TerminalControlArea, SetMinStackSetsUserAcknowledgedToFalseOnFirstTime)
        {
            TerminalControlArea tma(L"LTMA", "LTMA", 6000, "EGLL", true);
            tma.SetMinStack(1013);
            EXPECT_FALSE(tma.MinStackAcknowledged());
        }

        TEST(TerminalControlArea, SetMinStackSetsUserAcknowledgedToFalseOnNewMSL)
        {
            TerminalControlArea tma(L"LTMA", "LTMA", 6000, "EGLL", true);
            tma.SetMinStack(1013);
            tma.AcknowledgeMinStack();
            tma.SetMinStack(1012);
            EXPECT_FALSE(tma.MinStackAcknowledged());
        }

        TEST(TerminalControlArea, SetMinStackDoesNotSetUserAcknowledgedFalseIfNoChange)
        {
            TerminalControlArea tma(L"LTMA", "LTMA", 6000, "EGLL", true);
            tma.SetMinStack(1013);
            tma.AcknowledgeMinStack();
            tma.SetMinStack(1014);
            EXPECT_TRUE(tma.MinStackAcknowledged());
        }

        TEST(TerminalControlArea, GetNameReturnsCorrectValue)
        {
            TerminalControlArea tma(L"LTMA", "LTMA", 0, "EGLL", true);
            EXPECT_EQ(0, lstrcmp(L"LTMA", tma.GetName()));
        }

        TEST(TerminalControlArea, GetCharNameReturnsCorrectValue)
        {
            TerminalControlArea tma(L"LTMA", "LTMA", 1234, "EGLL", true);
            EXPECT_EQ(0, strcmp("LTMA", tma.GetCharName()));
        }

        TEST(TerminalControlArea, GetTransitionReturnsCorrectValue)
        {
            TerminalControlArea tma(L"LTMA", "LTMA", 1234, "EGLL", true);
            EXPECT_EQ(1234, tma.GetTransition());
        }
    }  // namespace MinStack
}  // namespace UKControllerPluginTest
