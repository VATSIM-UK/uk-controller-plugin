#include "flightplan/StoredFlightplan.h"
#include "flightplan/StoredFlightplanCollection.h"
#include "flightplan/StoredFlightplanEventHandler.h"

using ::testing::NiceMock;
using ::testing::Return;
using ::testing::Test;
using UKControllerPlugin::Flightplan::StoredFlightplan;
using UKControllerPlugin::Flightplan::StoredFlightplanCollection;
using UKControllerPlugin::Flightplan::StoredFlightplanEventHandler;
using UKControllerPluginTest::Euroscope::MockEuroScopeCFlightPlanInterface;
using UKControllerPluginTest::Euroscope::MockEuroScopeCRadarTargetInterface;

namespace UKControllerPluginTest {
    namespace Flightplan {

        class StoredFlightplanEventHandlerTest : public Test
        {
            public:
            StoredFlightplanCollection collection;
        };

        TEST_F(StoredFlightplanEventHandlerTest, FlightPlanEventAddsFlightplanIfNotRegistered)
        {
            NiceMock<MockEuroScopeCFlightPlanInterface> flightplanMock;

            ON_CALL(flightplanMock, GetCallsign()).WillByDefault(Return("BAW123"));

            ON_CALL(flightplanMock, GetOrigin()).WillByDefault(Return("EGKK"));

            ON_CALL(flightplanMock, GetOrigin()).WillByDefault(Return("EGCC"));

            NiceMock<MockEuroScopeCRadarTargetInterface> radarTargetMock;

            StoredFlightplanEventHandler handler(collection);
            handler.FlightPlanEvent(flightplanMock, radarTargetMock);

            EXPECT_TRUE(collection.HasFlightplanForCallsign("BAW123"));
        }

        TEST_F(StoredFlightplanEventHandlerTest, FlightPlanEventDoesNotChangeFlightplanIfNoChange)
        {
            NiceMock<MockEuroScopeCFlightPlanInterface> flightplanMock;

            ON_CALL(flightplanMock, GetCallsign()).WillByDefault(Return("BAW123"));

            ON_CALL(flightplanMock, GetOrigin()).WillByDefault(Return("EGKK"));

            ON_CALL(flightplanMock, GetOrigin()).WillByDefault(Return("EGCC"));

            NiceMock<MockEuroScopeCRadarTargetInterface> radarTargetMock;

            StoredFlightplanEventHandler handler(collection);
            handler.FlightPlanEvent(flightplanMock, radarTargetMock);
            StoredFlightplan flightplanFirst = collection.GetFlightplanForCallsign("BAW123");
            handler.FlightPlanEvent(flightplanMock, radarTargetMock);

            EXPECT_TRUE(flightplanFirst == collection.GetFlightplanForCallsign("BAW123"));
        }

        TEST_F(StoredFlightplanEventHandlerTest, FlightPlanEventChangesFlightplanIfDifferent)
        {
            NiceMock<MockEuroScopeCFlightPlanInterface> flightplanMock1;

            ON_CALL(flightplanMock1, GetCallsign()).WillByDefault(Return("BAW123"));

            ON_CALL(flightplanMock1, GetOrigin()).WillByDefault(Return("EGKK"));

            ON_CALL(flightplanMock1, GetDestination()).WillByDefault(Return("EGCC"));

            NiceMock<MockEuroScopeCFlightPlanInterface> flightplanMock2;

            ON_CALL(flightplanMock2, GetCallsign()).WillByDefault(Return("BAW123"));

            ON_CALL(flightplanMock2, GetOrigin()).WillByDefault(Return("EGKK"));

            ON_CALL(flightplanMock2, GetDestination()).WillByDefault(Return("EGGP"));

            NiceMock<MockEuroScopeCRadarTargetInterface> radarTargetMock;

            StoredFlightplanEventHandler handler(collection);
            handler.FlightPlanEvent(flightplanMock1, radarTargetMock);
            StoredFlightplan flightplanFirst = collection.GetFlightplanForCallsign("BAW123");
            handler.FlightPlanEvent(flightplanMock2, radarTargetMock);

            EXPECT_FALSE(flightplanFirst == collection.GetFlightplanForCallsign("BAW123"));
        }

        TEST_F(StoredFlightplanEventHandlerTest, FlightPlanDisconnectEventSucceedsIfNotFound)
        {
            StoredFlightplanEventHandler handler(collection);
            NiceMock<MockEuroScopeCFlightPlanInterface> flightplanMock1;

            EXPECT_CALL(flightplanMock1, GetCallsign()).Times(1).WillOnce(Return("BAW123"));

            handler.FlightPlanDisconnectEvent(flightplanMock1);
        }

        TEST_F(StoredFlightplanEventHandlerTest, FlightPlanDisconnectEventSetsTimeout)
        {
            NiceMock<MockEuroScopeCFlightPlanInterface> flightplanMock;

            ON_CALL(flightplanMock, GetCallsign()).WillByDefault(Return("BAW123"));

            ON_CALL(flightplanMock, GetOrigin()).WillByDefault(Return("EGKK"));

            ON_CALL(flightplanMock, GetOrigin()).WillByDefault(Return("EGCC"));

            NiceMock<MockEuroScopeCRadarTargetInterface> radarTargetMock;

            StoredFlightplanEventHandler handler(collection);
            handler.FlightPlanEvent(flightplanMock, radarTargetMock);
            handler.FlightPlanDisconnectEvent(flightplanMock);

            UKControllerPlugin::Flightplan::StoredFlightplan plan = collection.GetFlightplanForCallsign("BAW123");
            EXPECT_TRUE(plan.GetTimeout() != 0);
        }

        TEST_F(StoredFlightplanEventHandlerTest, FlightPlanEventResetsTimeout)
        {
            NiceMock<MockEuroScopeCFlightPlanInterface> flightplanMock;

            ON_CALL(flightplanMock, GetCallsign()).WillByDefault(Return("BAW123"));

            ON_CALL(flightplanMock, GetOrigin()).WillByDefault(Return("EGKK"));

            ON_CALL(flightplanMock, GetOrigin()).WillByDefault(Return("EGCC"));

            NiceMock<MockEuroScopeCRadarTargetInterface> radarTargetMock;

            StoredFlightplanEventHandler handler(collection);
            handler.FlightPlanEvent(flightplanMock, radarTargetMock);
            handler.FlightPlanDisconnectEvent(flightplanMock);
            handler.FlightPlanEvent(flightplanMock, radarTargetMock);

            UKControllerPlugin::Flightplan::StoredFlightplan plan = collection.GetFlightplanForCallsign("BAW123");
            EXPECT_TRUE(plan.GetTimeout() == 0);
        }

        TEST_F(StoredFlightplanEventHandlerTest, TimedEventTriggerRemovesTimedOutFlightplans)
        {
            StoredFlightplanEventHandler handler(collection);
            StoredFlightplan planOk("BAW456", "EGKK", "EGLL");
            StoredFlightplan planTimedOut("BAW123", "EGLL", "EGKK");
            planTimedOut.SetTimeout(-1);
            collection.UpdatePlan(planOk);
            collection.UpdatePlan(planTimedOut);

            handler.TimedEventTrigger();
            EXPECT_TRUE(collection.HasFlightplanForCallsign("BAW456"));
            EXPECT_FALSE(collection.HasFlightplanForCallsign("BAW123"));
        }
    } // namespace Flightplan
} // namespace UKControllerPluginTest
