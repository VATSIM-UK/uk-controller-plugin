#include "controller/ControllerStatusEventHandlerCollection.h"
#include "departure/AircraftDepartedEvent.h"
#include "departure/DepartureMonitor.h"
#include "login/Login.h"
#include "mock/MockEuroScopeCFlightplanInterface.h"
#include "mock/MockEuroScopeCRadarTargetInterface.h"
#include "mock/MockEuroscopePluginLoopbackInterface.h"
#include "test/EventBusTestCase.h"

namespace UKControllerPluginTest::Departure {

    class DepartureMonitorTest : public UKControllerPluginUtilsTest::EventBusTestCase
    {
        public:
        DepartureMonitorTest() : login(mockPlugin, controllerStatuses), monitor(login, mockPlugin)
        {
            mockFlightplan = std::make_shared<testing::NiceMock<Euroscope::MockEuroScopeCFlightPlanInterface>>();
            mockRadarTarget = std::make_shared<testing::NiceMock<Euroscope::MockEuroScopeCRadarTargetInterface>>();

            ON_CALL(*mockFlightplan, GetCallsign).WillByDefault(testing::Return("BAW123"));
            ON_CALL(*mockFlightplan, GetOrigin).WillByDefault(testing::Return("EGKK"));

            mockPlugin.AddAllFlightplansItem({mockFlightplan, mockRadarTarget});
            login.SetLoginTime(std::chrono::system_clock::now() - std::chrono::seconds(11));
        }

        std::shared_ptr<testing::NiceMock<Euroscope::MockEuroScopeCFlightPlanInterface>> mockFlightplan;
        std::shared_ptr<testing::NiceMock<Euroscope::MockEuroScopeCRadarTargetInterface>> mockRadarTarget;
        UKControllerPlugin::Controller::ControllerStatusEventHandlerCollection controllerStatuses;
        testing::NiceMock<Euroscope::MockEuroscopePluginLoopbackInterface> mockPlugin;
        UKControllerPlugin::Controller::Login login;
        UKControllerPlugin::Departure::DepartureMonitor monitor;
    };

    TEST_F(DepartureMonitorTest, ItSendsDepartedEvent)
    {
        ON_CALL(*mockFlightplan, GetDistanceFromOrigin).WillByDefault(testing::Return(2.5));

        ON_CALL(*mockRadarTarget, GetGroundSpeed).WillByDefault(testing::Return(125));

        ON_CALL(*mockRadarTarget, GetFlightLevel).WillByDefault(testing::Return(2500));

        monitor.TimedEventTrigger();

        AssertSingleEventDispatched();
        AssertFirstEventDispatched<UKControllerPlugin::Departure::AircraftDepartedEvent>([](const auto& event) {
            EXPECT_EQ("BAW123", event.callsign);
            EXPECT_EQ("EGKK", event.airfield);
        });
    }

    TEST_F(DepartureMonitorTest, ItDoesntSendDuplicateDepartedEvents)
    {
        ON_CALL(*mockFlightplan, GetDistanceFromOrigin).WillByDefault(testing::Return(2.5));

        ON_CALL(*mockRadarTarget, GetGroundSpeed).WillByDefault(testing::Return(125));

        ON_CALL(*mockRadarTarget, GetFlightLevel).WillByDefault(testing::Return(2500));

        monitor.TimedEventTrigger();

        AssertSingleEventDispatched();
        AssertFirstEventDispatched<UKControllerPlugin::Departure::AircraftDepartedEvent>([](const auto& event) {
            EXPECT_EQ("BAW123", event.callsign);
            EXPECT_EQ("EGKK", event.airfield);
        });

        monitor.TimedEventTrigger();
        AssertSingleEventDispatched();
    }

    TEST_F(DepartureMonitorTest, ItAllowsRedepartureIfAircraftGoesOffline)
    {
        ON_CALL(*mockFlightplan, GetDistanceFromOrigin).WillByDefault(testing::Return(2.5));

        ON_CALL(*mockRadarTarget, GetGroundSpeed).WillByDefault(testing::Return(125));

        ON_CALL(*mockRadarTarget, GetFlightLevel).WillByDefault(testing::Return(2500));

        monitor.TimedEventTrigger();
        monitor.FlightPlanDisconnectEvent(*mockFlightplan);
        monitor.TimedEventTrigger();

        AssertEventDispatchCount(2);
        AssertEventDispatched<UKControllerPlugin::Departure::AircraftDepartedEvent>(1, [](const auto& event) {
            EXPECT_EQ("BAW123", event.callsign);
            EXPECT_EQ("EGKK", event.airfield);
        });
    }

    TEST_F(DepartureMonitorTest, ItAllowsRedepartureIfAircraftChangesOriginAirport)
    {
        ON_CALL(*mockFlightplan, GetDistanceFromOrigin).WillByDefault(testing::Return(2.5));

        ON_CALL(*mockRadarTarget, GetGroundSpeed).WillByDefault(testing::Return(125));

        ON_CALL(*mockRadarTarget, GetFlightLevel).WillByDefault(testing::Return(2500));

        monitor.TimedEventTrigger();

        ON_CALL(*mockFlightplan, GetOrigin).WillByDefault(testing::Return("EGLL"));

        monitor.FlightPlanEvent(*mockFlightplan, *mockRadarTarget);
        monitor.TimedEventTrigger();

        AssertEventDispatchCount(2);
        AssertEventDispatched<UKControllerPlugin::Departure::AircraftDepartedEvent>(1, [](const auto& event) {
            EXPECT_EQ("BAW123", event.callsign);
            EXPECT_EQ("EGLL", event.airfield);
        });
    }

    TEST_F(DepartureMonitorTest, ItDoesntAllowRedepartureIfAircraftDoesntChangeOriginAirport)
    {
        ON_CALL(*mockFlightplan, GetDistanceFromOrigin).WillByDefault(testing::Return(2.5));

        ON_CALL(*mockRadarTarget, GetGroundSpeed).WillByDefault(testing::Return(125));

        ON_CALL(*mockRadarTarget, GetFlightLevel).WillByDefault(testing::Return(2500));

        monitor.TimedEventTrigger();
        monitor.FlightPlanEvent(*mockFlightplan, *mockRadarTarget);
        monitor.TimedEventTrigger();

        AssertSingleEventDispatched();
    }

    TEST_F(DepartureMonitorTest, ItDoesntSendDepartedEventNoOrigin)
    {
        ON_CALL(*mockFlightplan, GetOrigin).WillByDefault(testing::Return(""));

        ON_CALL(*mockFlightplan, GetDistanceFromOrigin).WillByDefault(testing::Return(2.5));

        ON_CALL(*mockRadarTarget, GetGroundSpeed).WillByDefault(testing::Return(125));

        ON_CALL(*mockRadarTarget, GetFlightLevel).WillByDefault(testing::Return(2500));

        monitor.TimedEventTrigger();

        AssertNoEventsDispatched();
    }

    TEST_F(DepartureMonitorTest, ItDoesntSendDepartedEventTooFarFromOrigin)
    {
        ON_CALL(*mockFlightplan, GetDistanceFromOrigin).WillByDefault(testing::Return(5.1));

        ON_CALL(*mockRadarTarget, GetGroundSpeed).WillByDefault(testing::Return(125));

        ON_CALL(*mockRadarTarget, GetFlightLevel).WillByDefault(testing::Return(2500));

        monitor.TimedEventTrigger();

        AssertNoEventsDispatched();
    }

    TEST_F(DepartureMonitorTest, ItDoesntSendDepartedEventTooHigh)
    {
        ON_CALL(*mockFlightplan, GetDistanceFromOrigin).WillByDefault(testing::Return(2.5));

        ON_CALL(*mockRadarTarget, GetGroundSpeed).WillByDefault(testing::Return(125));

        ON_CALL(*mockRadarTarget, GetFlightLevel).WillByDefault(testing::Return(5100));

        monitor.TimedEventTrigger();

        AssertNoEventsDispatched();
    }

    TEST_F(DepartureMonitorTest, ItDoesntSendDepartedEventTooLow)
    {
        ON_CALL(*mockFlightplan, GetDistanceFromOrigin).WillByDefault(testing::Return(2.5));

        ON_CALL(*mockRadarTarget, GetGroundSpeed).WillByDefault(testing::Return(125));

        ON_CALL(*mockRadarTarget, GetFlightLevel).WillByDefault(testing::Return(1400));

        monitor.TimedEventTrigger();

        AssertNoEventsDispatched();
    }

    TEST_F(DepartureMonitorTest, ItDoesntSendDepartedEventTooSlow)
    {
        ON_CALL(*mockFlightplan, GetDistanceFromOrigin).WillByDefault(testing::Return(2.5));

        ON_CALL(*mockRadarTarget, GetGroundSpeed).WillByDefault(testing::Return(69));

        ON_CALL(*mockRadarTarget, GetFlightLevel).WillByDefault(testing::Return(2500));

        monitor.TimedEventTrigger();

        AssertNoEventsDispatched();
    }

    TEST_F(DepartureMonitorTest, ItDoesntSendDepartedEventOutOfRange)
    {
        ON_CALL(*mockFlightplan, GetDistanceFromOrigin).WillByDefault(testing::Return(0));

        ON_CALL(*mockRadarTarget, GetGroundSpeed).WillByDefault(testing::Return(125));

        ON_CALL(*mockRadarTarget, GetFlightLevel).WillByDefault(testing::Return(0));

        monitor.TimedEventTrigger();

        AssertNoEventsDispatched();
    }

    TEST_F(DepartureMonitorTest, ItDoesntSendDepartedEventOnlyJustLoggedOn)
    {
        login.SetLoginTime(std::chrono::system_clock::now() - std::chrono::seconds(5));

        ON_CALL(*mockFlightplan, GetDistanceFromOrigin).WillByDefault(testing::Return(1));

        ON_CALL(*mockRadarTarget, GetGroundSpeed).WillByDefault(testing::Return(125));

        ON_CALL(*mockRadarTarget, GetFlightLevel).WillByDefault(testing::Return(3000));

        monitor.TimedEventTrigger();

        AssertNoEventsDispatched();
    }
} // namespace UKControllerPluginTest::Departure
