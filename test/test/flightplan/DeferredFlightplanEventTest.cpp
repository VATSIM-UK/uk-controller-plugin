#pragma once
#include "pch/pch.h"
#include "flightplan/DeferredFlightplanEvent.h"
#include "mock/MockFlightPlanEventHandlerInterface.h"
#include "mock/MockEuroscopePluginLoopbackInterface.h"
#include "mock/MockEuroScopeCFlightplanInterface.h"
#include "mock/MockEuroScopeCRadarTargetInterface.h"

using UKControllerPlugin::Flightplan::DeferredFlightPlanEvent;
using UKControllerPluginTest::Euroscope::MockEuroscopePluginLoopbackInterface;
using UKControllerPluginTest::Flightplan::MockFlightPlanEventHandlerInterface;
using UKControllerPluginTest::Euroscope::MockEuroScopeCFlightPlanInterface;
using UKControllerPluginTest::Euroscope::MockEuroScopeCRadarTargetInterface;

using ::testing::Test;
using ::testing::NiceMock;
using ::testing::Return;
using ::testing::Throw;
using ::testing::Ref;

namespace UKControllerPluginTest {
    namespace Flightplan {

        class DeferredFlightplanEventTest : public Test
        {
            public:

                DeferredFlightplanEventTest()
                    : event(mockHandler, mockPlugin, callsign)
                {
                    this->mockFlightplan = std::make_shared<NiceMock<MockEuroScopeCFlightPlanInterface>>();
                    this->mockRadarTarget = std::make_shared<NiceMock<MockEuroScopeCRadarTargetInterface>>();
                }

                const std::string callsign = "BAW123";
                NiceMock<MockEuroscopePluginLoopbackInterface> mockPlugin;
                NiceMock<MockFlightPlanEventHandlerInterface> mockHandler;
                std::shared_ptr<NiceMock<MockEuroScopeCFlightPlanInterface>> mockFlightplan;
                std::shared_ptr<NiceMock<MockEuroScopeCRadarTargetInterface>> mockRadarTarget;
                DeferredFlightPlanEvent event;
        };

        TEST_F(DeferredFlightplanEventTest, ItCallsTheHandlerWhenRun)
        {
            ON_CALL(this->mockPlugin, GetFlightplanForCallsign(this->callsign))
                .WillByDefault(Return(this->mockFlightplan));

            ON_CALL(this->mockPlugin, GetRadarTargetForCallsign(this->callsign))
                .WillByDefault(Return(this->mockRadarTarget));

            EXPECT_CALL(this->mockHandler, FlightPlanEvent(Ref(*this->mockFlightplan), Ref(*this->mockRadarTarget)))
                .Times(1);

            this->event.Run();
        }

        TEST_F(DeferredFlightplanEventTest, ItHandlesMissingFlightplansGracefully)
        {
            ON_CALL(this->mockPlugin, GetFlightplanForCallsign(this->callsign))
                .WillByDefault(Return(nullptr));

            ON_CALL(this->mockPlugin, GetRadarTargetForCallsign(this->callsign))
                .WillByDefault(Return(this->mockRadarTarget));

            EXPECT_NO_THROW(this->event.Run());
        }

        TEST_F(DeferredFlightplanEventTest, ItHandlesMissingRadarTargetsGracefully)
        {
            ON_CALL(this->mockPlugin, GetFlightplanForCallsign(this->callsign))
                .WillByDefault(Return(this->mockFlightplan));

            ON_CALL(this->mockPlugin, GetRadarTargetForCallsign(this->callsign))
                .WillByDefault(Return(nullptr));

            EXPECT_NO_THROW(this->event.Run());
        }

    }  // namespace Flightplan
}  // namespace UKControllerPluginTest
