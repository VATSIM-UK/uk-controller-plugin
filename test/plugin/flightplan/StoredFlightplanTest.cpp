#include "flightplan/StoredFlightplan.h"
#include "mock/MockEuroScopeCFlightplanInterface.h"
#include "helper/HelperFunctions.h"

using ::testing::NiceMock;
using ::testing::Return;
using UKControllerPlugin::HelperFunctions;
using UKControllerPlugin::Flightplan::StoredFlightplan;
using UKControllerPluginTest::Euroscope::MockEuroScopeCFlightPlanInterface;

namespace UKControllerPluginTest::Flightplan {

    TEST(StoredFlightplan, CanConstructFromEuroscopePlanWithSquawk)
    {
        NiceMock<MockEuroScopeCFlightPlanInterface> mockEuroscope;

        ON_CALL(mockEuroscope, GetCallsign()).WillByDefault(Return("BAW123"));

        ON_CALL(mockEuroscope, GetOrigin()).WillByDefault(Return("EGKK"));

        ON_CALL(mockEuroscope, GetDestination()).WillByDefault(Return("EDDF"));

        ON_CALL(mockEuroscope, HasAssignedSquawk()).WillByDefault(Return(true));

        ON_CALL(mockEuroscope, GetAssignedSquawk()).WillByDefault(Return("2415"));

        ON_CALL(mockEuroscope, GetExpectedDepartureTime()).WillByDefault(Return("2301"));

        StoredFlightplan plan(mockEuroscope);
        EXPECT_TRUE(plan.GetCallsign() == "BAW123");
        EXPECT_TRUE(plan.GetOrigin() == "EGKK");
        EXPECT_TRUE(plan.GetDestination() == "EDDF");
        EXPECT_TRUE(plan.GetPreviouslyAssignedSquawk() == "2415");
        EXPECT_TRUE(plan.GetTimeout() == 0);
        EXPECT_EQ(
            HelperFunctions::GetTimeFromNumberString("2301") - std::chrono::minutes(15),
            plan.GetExpectedOffBlockTime());
    }

    TEST(StoredFlightplan, CanConstructFromEuroscopePlanWithoutSquawk)
    {
        NiceMock<MockEuroScopeCFlightPlanInterface> mockEuroscope;

        ON_CALL(mockEuroscope, GetCallsign()).WillByDefault(Return("BAW123"));

        ON_CALL(mockEuroscope, GetOrigin()).WillByDefault(Return("EGKK"));

        ON_CALL(mockEuroscope, GetDestination()).WillByDefault(Return("EDDF"));

        ON_CALL(mockEuroscope, HasAssignedSquawk()).WillByDefault(Return(false));

        StoredFlightplan plan(mockEuroscope);
        EXPECT_TRUE(plan.GetCallsign() == "BAW123");
        EXPECT_TRUE(plan.GetOrigin() == "EGKK");
        EXPECT_TRUE(plan.GetDestination() == "EDDF");
        EXPECT_FALSE(plan.HasPreviouslyAssignedSquawk());
        EXPECT_TRUE(plan.GetTimeout() == 0);
    }

    TEST(StoredFlightplan, CanConstructFromEuroscopePlanWithoutExpectedDepartureTime)
    {
        NiceMock<MockEuroScopeCFlightPlanInterface> mockEuroscope;

        ON_CALL(mockEuroscope, GetCallsign()).WillByDefault(Return("BAW123"));

        ON_CALL(mockEuroscope, GetOrigin()).WillByDefault(Return("EGKK"));

        ON_CALL(mockEuroscope, GetDestination()).WillByDefault(Return("EDDF"));

        ON_CALL(mockEuroscope, HasAssignedSquawk()).WillByDefault(Return(false));

        StoredFlightplan plan(mockEuroscope);
        EXPECT_TRUE(plan.GetCallsign() == "BAW123");
        EXPECT_TRUE(plan.GetOrigin() == "EGKK");
        EXPECT_TRUE(plan.GetDestination() == "EDDF");
        EXPECT_FALSE(plan.HasPreviouslyAssignedSquawk());
        EXPECT_TRUE(plan.GetTimeout() == 0);
        EXPECT_EQ((std::chrono::system_clock::time_point::max)(), plan.GetExpectedOffBlockTime());
    }

    TEST(StoredFlightplan, SetGetCallsign)
    {
        StoredFlightplan plan("BAW123", "EGKK", "EDDM");
        EXPECT_TRUE(plan.GetCallsign() == "BAW123");
        plan.SetCallsign("EZY456");
        EXPECT_TRUE(plan.GetCallsign() == "EZY456");
    }

    TEST(StoredFlightplan, SetGetDestination)
    {
        StoredFlightplan plan("BAW123", "EGKK", "EDDM");
        EXPECT_TRUE(plan.GetDestination() == "EDDM");
        plan.SetDestination("EDDF");
        EXPECT_TRUE(plan.GetDestination() == "EDDF");
    }

    TEST(StoredFlightplan, SetGetOrigin)
    {
        StoredFlightplan plan("BAW123", "EGKK", "EDDM");
        EXPECT_TRUE(plan.GetOrigin() == "EGKK");
        plan.SetOrigin("EGLL");
        EXPECT_TRUE(plan.GetOrigin() == "EGLL");
    }

    TEST(StoredFlightplan, TimeoutDefaultsToDefault)
    {
        StoredFlightplan plan("BAW123", "EGKK", "EDDM");
        EXPECT_EQ(0, plan.GetTimeout());
    }

    TEST(StoredFlightplan, SetGetTimeout)
    {
        StoredFlightplan plan("BAW123", "EGKK", "EDDM");
        time_t timeout;
        time(&timeout);

        plan.SetTimeout(10);
        EXPECT_TRUE(plan.GetTimeout() >= timeout);
        EXPECT_TRUE(plan.GetTimeout() < timeout + 15);
    }

    TEST(StoredFlightplan, ResetTimeoutResetsToDefault)
    {
        StoredFlightplan plan("BAW123", "EGKK", "EDDM");
        plan.SetTimeout(10);
        plan.ResetTimeout();
        EXPECT_EQ(0, plan.GetTimeout());
    }

    TEST(StoredFlightplan, HasTimedOutReturnsFalseIfDefault)
    {
        StoredFlightplan plan("BAW123", "EGKK", "EDDM");
        EXPECT_FALSE(plan.HasTimedOut());
    }

    TEST(StoredFlightplan, ResetTimeoutReturnsTrueIfTimedOut)
    {
        StoredFlightplan plan("BAW123", "EGKK", "EDDM");

        plan.SetTimeout(-5);
        EXPECT_TRUE(plan.HasTimedOut());
    }

    TEST(StoredFlightplan, ResetTimeoutReturnsTrueIfNotTimedOut)
    {
        StoredFlightplan plan("BAW123", "EGKK", "EDDM");
        plan.SetTimeout(600);
        EXPECT_FALSE(plan.HasTimedOut());
    }

    TEST(StoredFlightplan, TestItSetsADefaultOffBlockTime)
    {
        StoredFlightplan plan("BAW123", "EGKK", "EDDM");
        EXPECT_TRUE((std::chrono::system_clock::time_point::max)() == plan.GetActualOffBlockTime());
    }

    TEST(StoredFlightplan, TestSetActualOffBlockTimeSetsTime)
    {
        StoredFlightplan plan("BAW123", "EGKK", "EDDM");
        std::chrono::system_clock::time_point time = std::chrono::system_clock::now();
        plan.SetActualOffBlockTime(time);
        EXPECT_TRUE(time == plan.GetActualOffBlockTime());
    }

    TEST(StoredFlightplan, TestSetGetPreviouslyAssignedSquawk)
    {
        StoredFlightplan plan("BAW123", "EGKK", "EDDM");
        EXPECT_TRUE(plan.GetPreviouslyAssignedSquawk() == "2200");
        plan.SetPreviouslyAssignedSquawk("1234");
        EXPECT_TRUE(plan.GetPreviouslyAssignedSquawk() == "1234");
    }

    TEST(StoredFlightplan, HasPreviouslyAssignedSquawkReturnsFalseIfNotSet)
    {
        StoredFlightplan plan("BAW123", "EGKK", "EDDM");
        EXPECT_FALSE(plan.HasPreviouslyAssignedSquawk());
    }

    TEST(StoredFlightplan, HasPreviouslyAssignedSquawkReturnsTrueIfSet)
    {
        StoredFlightplan plan("BAW123", "EGKK", "EDDM");
        plan.SetPreviouslyAssignedSquawk("1234");
        EXPECT_TRUE(plan.HasPreviouslyAssignedSquawk());
    }

    TEST(StoredFlightplan, EqualityOperatorFailsOnDifferentCallsign)
    {
        StoredFlightplan plan1("BAW123", "EGKK", "EDDM");
        StoredFlightplan plan2("BAW456", "EGKK", "EDDM");
        EXPECT_FALSE(plan1 == plan2);
    }

    TEST(StoredFlightplan, EqualityOperatorFailsOnDifferentOrigin)
    {
        StoredFlightplan plan1("BAW123", "EGKK", "EDDM");
        StoredFlightplan plan2("BAW123", "EGLL", "EDDM");
        EXPECT_FALSE(plan1 == plan2);
    }

    TEST(StoredFlightplan, EqualityOperatorFailsOnDifferentDestination)
    {
        StoredFlightplan plan1("BAW123", "EGKK", "EDDF");
        StoredFlightplan plan2("BAW123", "EGKK", "EDDS");
        EXPECT_FALSE(plan1 == plan2);
    }

    TEST(StoredFlightplan, EqualityOperatorPassesOnSame)
    {
        StoredFlightplan plan1("BAW123", "EGKK", "EDDF");
        StoredFlightplan plan2("BAW123", "EGKK", "EDDF");
        EXPECT_TRUE(plan1 == plan2);
    }

    TEST(StoredFlightplan, InequalityOperatorReturnsTrueIfCallsignDifferent)
    {
        StoredFlightplan plan1("BAW123", "EGKK", "EDDF");
        StoredFlightplan plan2("BAW456", "EGKK", "EDDF");
        EXPECT_TRUE(plan1 != plan2);
    }

    TEST(StoredFlightplan, InequalityOperatorReturnsTrueIfOriginDifferent)
    {
        StoredFlightplan plan1("BAW123", "EGKK", "EDDF");
        StoredFlightplan plan2("BAW123", "EGLL", "EDDF");
        EXPECT_TRUE(plan1 != plan2);
    }

    TEST(StoredFlightplan, InequalityOperatorReturnsTrueIfDestinationDifferent)
    {
        StoredFlightplan plan1("BAW123", "EGKK", "EDDF");
        StoredFlightplan plan2("BAW123", "EGKK", "EDDM");
        EXPECT_TRUE(plan1 != plan2);
    }

    TEST(StoredFlightplan, EqualityOperatorReturnsFalseIfSame)
    {
        StoredFlightplan plan1("BAW123", "EGKK", "EDDF");
        StoredFlightplan plan2("BAW123", "EGKK", "EDDF");
        EXPECT_FALSE(plan1 != plan2);
    }

    TEST(StoredFlightplan, EuroscopeEqualityOperatorReturnsFalseIfCallsignDifferent)
    {
        NiceMock<MockEuroScopeCFlightPlanInterface> mockEuroscope;

        ON_CALL(mockEuroscope, GetCallsign()).WillByDefault(Return("BAW123"));

        ON_CALL(mockEuroscope, GetOrigin()).WillByDefault(Return("EGKK"));

        ON_CALL(mockEuroscope, GetDestination()).WillByDefault(Return("EDDF"));

        StoredFlightplan plan("BAW124", "EGKK", "EDDF");
        EXPECT_FALSE(plan == mockEuroscope);
    }

    TEST(StoredFlightplan, EuroscopeEqualityOperatorFalseIfOriginDifferent)
    {
        NiceMock<MockEuroScopeCFlightPlanInterface> mockEuroscope;

        ON_CALL(mockEuroscope, GetCallsign()).WillByDefault(Return("BAW123"));

        ON_CALL(mockEuroscope, GetOrigin()).WillByDefault(Return("EGKK"));

        ON_CALL(mockEuroscope, GetDestination()).WillByDefault(Return("EDDF"));

        StoredFlightplan plan("BAW123", "EGLL", "EDDF");
        EXPECT_FALSE(plan == mockEuroscope);
    }

    TEST(StoredFlightplan, EuroscopeEqualityOperatorReturnsFalseIfDestinationDifferent)
    {
        NiceMock<MockEuroScopeCFlightPlanInterface> mockEuroscope;

        ON_CALL(mockEuroscope, GetCallsign()).WillByDefault(Return("BAW123"));

        ON_CALL(mockEuroscope, GetOrigin()).WillByDefault(Return("EGKK"));

        ON_CALL(mockEuroscope, GetDestination()).WillByDefault(Return("EDDF"));

        StoredFlightplan plan("BAW123", "EGKK", "EDDM");
        EXPECT_FALSE(plan == mockEuroscope);
    }

    TEST(StoredFlightplan, EuroscopeAssignmentOperatorSetsProperties)
    {
        NiceMock<MockEuroScopeCFlightPlanInterface> mockEuroscope;

        ON_CALL(mockEuroscope, GetCallsign()).WillByDefault(Return("BAW456"));

        ON_CALL(mockEuroscope, GetOrigin()).WillByDefault(Return("EGLL"));

        ON_CALL(mockEuroscope, GetDestination()).WillByDefault(Return("EDDM"));

        StoredFlightplan plan("BAW123", "EGKK", "EDDF");
        plan = mockEuroscope;

        EXPECT_TRUE(plan.GetCallsign() == "BAW456");
        EXPECT_TRUE(plan.GetOrigin() == "EGLL");
        EXPECT_TRUE(plan.GetDestination() == "EDDM");
    }

    TEST(StoredFlightplan, AssignmentOperatorSetsProperties)
    {
        StoredFlightplan plan1("BAW123", "EGKK", "EDDF");
        StoredFlightplan plan2("BAW456", "EGLL", "EDDM");
        plan2.SetTimeout(0);
        plan2.SetPreviouslyAssignedSquawk("1234");

        plan1 = plan2;

        EXPECT_TRUE(plan1.GetCallsign() == "BAW456");
        EXPECT_TRUE(plan1.GetOrigin() == "EGLL");
        EXPECT_TRUE(plan1.GetDestination() == "EDDM");
        EXPECT_LT(time(0) - plan1.GetTimeout(), 3);
        EXPECT_TRUE("1234" == plan1.GetPreviouslyAssignedSquawk());
    }

    TEST(StoredFlightplan, GetSetEstimatedDepartureTimeSetsTime)
    {
        StoredFlightplan plan("BAW123", "EGKK", "EDDF");
        std::chrono::system_clock::time_point time = std::chrono::system_clock::now();

        plan.SetEstimatedDepartureTime(time);
        EXPECT_EQ(time, plan.GetEstimatedDepartureTime());
    }
} // namespace UKControllerPluginTest::Flightplan
