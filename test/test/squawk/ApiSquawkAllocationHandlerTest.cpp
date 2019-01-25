#pragma once
#include "pch/pch.h"
#include "squawk/ApiSquawkAllocation.h"
#include "squawk/ApiSquawkAllocationHandler.h"
#include "mock/MockEuroscopePluginLoopbackInterface.h"
#include "mock/MockEuroScopeCFlightplanInterface.h"

using UKControllerPlugin::Squawk::ApiSquawkAllocation;
using UKControllerPlugin::Squawk::ApiSquawkAllocationHandler;
using UKControllerPluginTest::Euroscope::MockEuroScopeCFlightPlanInterface;
using UKControllerPluginTest::Euroscope::MockEuroscopePluginLoopbackInterface;
using ::testing::Test;
using ::testing::NiceMock;
using ::testing::Return;
using ::testing::Throw;

namespace UKControllerPluginTest {
    namespace Squawk {

        class ApiSquawkAllocationHandlerTest : public Test
        {
            public:
                ApiSquawkAllocationHandlerTest()
                    : handler(mockPlugin)
                {

                }

                void SetUp(void)
                {
                    mockFlightplan1.reset(new NiceMock<MockEuroScopeCFlightPlanInterface>);
                    mockFlightplan2.reset(new NiceMock<MockEuroScopeCFlightPlanInterface>);

                    ON_CALL(*this->mockFlightplan1, GetCallsign())
                        .WillByDefault(Return("BAW123"));

                    ON_CALL(*this->mockFlightplan2, GetCallsign())
                        .WillByDefault(Return("EZY12AX"));

                    ON_CALL(this->mockPlugin, GetFlightplanForCallsign("BAW123"))
                        .WillByDefault(Return(mockFlightplan1));

                    ON_CALL(this->mockPlugin, GetFlightplanForCallsign("EZY12AX"))
                        .WillByDefault(Return(mockFlightplan2));
                }

                NiceMock<MockEuroscopePluginLoopbackInterface> mockPlugin;
                std::shared_ptr<NiceMock<MockEuroScopeCFlightPlanInterface>> mockFlightplan1;
                std::shared_ptr<NiceMock<MockEuroScopeCFlightPlanInterface>> mockFlightplan2;
                ApiSquawkAllocationHandler handler;
        };

        TEST_F(ApiSquawkAllocationHandlerTest, ItStartsEmpty)
        {
            EXPECT_EQ(0, this->handler.Count());
        }

        TEST_F(ApiSquawkAllocationHandlerTest, ItAddsAnEvent)
        {
            ApiSquawkAllocation event{ "BAW123", "0123" };
            this->handler.AddAllocationEvent(event);
            EXPECT_EQ(1, this->handler.Count());
        }

        TEST_F(ApiSquawkAllocationHandlerTest, ItDoesntAddDuplicateEvents)
        {
            ApiSquawkAllocation event{ "BAW123", "0123" };
            this->handler.AddAllocationEvent(event);
            this->handler.AddAllocationEvent(event);
            EXPECT_EQ(1, this->handler.Count());
        }

        TEST_F(ApiSquawkAllocationHandlerTest, TimedEventAssignsAllSquawks)
        {
            ApiSquawkAllocation event1{ "BAW123", "0123" };
            ApiSquawkAllocation event2{ "EZY12AX", "5623" };
            this->handler.AddAllocationEvent(event1);
            this->handler.AddAllocationEvent(event2);
            EXPECT_CALL(*this->mockFlightplan1, SetSquawk("0123"))
                .Times(1);

            EXPECT_CALL(*this->mockFlightplan2, SetSquawk("5623"))
                .Times(1);

            this->handler.TimedEventTrigger();
        }

        TEST_F(ApiSquawkAllocationHandlerTest, TimedEventTriggerRemovesTheEvents)
        {
            ApiSquawkAllocation event1{ "BAW123", "0123" };
            ApiSquawkAllocation event2{ "EZY12AX", "5623" };
            this->handler.AddAllocationEvent(event1);
            this->handler.AddAllocationEvent(event2);
            this->handler.TimedEventTrigger();
            EXPECT_EQ(0, this->handler.Count());
        }

        TEST_F(ApiSquawkAllocationHandlerTest, TimedEventHandlesNoEventsToProcess)
        {
            EXPECT_NO_THROW(this->handler.TimedEventTrigger());
        }

        TEST_F(ApiSquawkAllocationHandlerTest, TimedEventHandlerGracefullyHandlesMissingFlightplans)
        {
            ApiSquawkAllocation event1{ "XXXXX", "0123" };
            ApiSquawkAllocation event2{ "BAW123", "0123" };
            this->handler.AddAllocationEvent(event1);
            this->handler.AddAllocationEvent(event2);

            ON_CALL(this->mockPlugin, GetFlightplanForCallsign("XXXXX"))
                .WillByDefault(Throw(std::invalid_argument("Test")));

            EXPECT_CALL(*this->mockFlightplan1, SetSquawk("0123"))
                .Times(1);

            EXPECT_NO_THROW(this->handler.TimedEventTrigger());
        }
    }  // namespace Squawk
}  // namespace UKControllerPluginTest
