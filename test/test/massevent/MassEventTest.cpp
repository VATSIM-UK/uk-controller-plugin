#include "pch/pch.h"
#include "massevent/MassEvent.h"
#include "mock/MockEuroscopePluginLoopbackInterface.h"
#include "mock/MockEuroScopeCFlightplanInterface.h"
#include "mock/MockEuroScopeCRadarTargetInterface.h"
#include "mock/MockInitialAltitudeEventHandler.h"
#include "flightplan/StoredFlightplanCollection.h"
#include "flightplan/StoredFlightplan.h"
#include "mock/MockSquawkEventHandler.h"

using UKControllerPlugin::EventHandler::MassEvent;
using UKControllerPluginTest::Euroscope::MockEuroscopePluginLoopbackInterface;
using UKControllerPluginTest::Euroscope::MockEuroScopeCFlightPlanInterface;
using UKControllerPluginTest::Euroscope::MockEuroScopeCRadarTargetInterface;
using UKControllerPluginTest::EventHandler::MockInitialAltitudeEventHandler;
using UKControllerPlugin::Flightplan::StoredFlightplan;
using UKControllerPlugin::Flightplan::StoredFlightplanCollection;
using UKControllerPlugin::InitialAltitude::InitialAltitudeEventHandler;
using UKControllerPlugin::InitialAltitude::InitialAltitudeGenerator;
using UKControllerPluginTest::Squawk::MockSquawkEventHandler;
using ::testing::NiceMock;
using ::testing::Return;
using ::testing::StrictMock;
using ::testing::Ref;

namespace UKControllerPluginTest {
    namespace EventHandler {

        TEST(MassEvent, SetAllInitialAltitudesSetsAll)
        {
            std::shared_ptr<MockEuroScopeCFlightPlanInterface> mockFlightplan1(
                new NiceMock<MockEuroScopeCFlightPlanInterface>
            );
            std::shared_ptr<MockEuroScopeCRadarTargetInterface> mockRadarTarget1(
                new NiceMock<MockEuroScopeCRadarTargetInterface>
            );
            std::shared_ptr<MockEuroScopeCFlightPlanInterface> mockFlightplan2(
                new NiceMock<MockEuroScopeCFlightPlanInterface>
            );
            std::shared_ptr<MockEuroScopeCRadarTargetInterface> mockRadarTarget2(
                new NiceMock<MockEuroScopeCRadarTargetInterface>
            );

            StrictMock<MockEuroscopePluginLoopbackInterface> mockEuroscopePlugin;

            EXPECT_CALL(mockEuroscopePlugin, GetFlightplanForCallsign("BAW123"))
                .Times(1)
                .WillOnce(Return(mockFlightplan1));

            EXPECT_CALL(mockEuroscopePlugin, GetRadarTargetForCallsign("BAW123"))
                .Times(1)
                .WillOnce(Return(mockRadarTarget1));

            EXPECT_CALL(mockEuroscopePlugin, GetFlightplanForCallsign("EZY456"))
                .Times(1)
                .WillOnce(Return(mockFlightplan2));

            EXPECT_CALL(mockEuroscopePlugin, GetRadarTargetForCallsign("EZY456"))
                .Times(1)
                .WillOnce(Return(mockRadarTarget2));

            StoredFlightplanCollection flightplans;
            flightplans.UpdatePlan(StoredFlightplan("BAW123", "EGKK", "EDDM"));
            flightplans.UpdatePlan(StoredFlightplan("EZY456", "EGLL", "EIDW"));

            std::shared_ptr<StrictMock<MockInitialAltitudeEventHandler>> initialAltitudeEventHandler(
                new StrictMock<MockInitialAltitudeEventHandler>
            );
            EXPECT_CALL(*initialAltitudeEventHandler, FlightPlanEvent(Ref(*mockFlightplan1), Ref(*mockRadarTarget1)))
                .Times(1);

            EXPECT_CALL(*initialAltitudeEventHandler, FlightPlanEvent(Ref(*mockFlightplan2), Ref(*mockRadarTarget2)))
                .Times(1);

            std::shared_ptr<StrictMock<MockSquawkEventHandler>> mockSquawkEventHandler(
                new StrictMock<MockSquawkEventHandler>
            );

            MassEvent mass(mockEuroscopePlugin, initialAltitudeEventHandler, flightplans);
            mass.SetAllInitialAltitudes();
        }

        TEST(MassEvent, SetAllInitialAltitudesSetsDoesNothingIfHandlerNull)
        {
            std::shared_ptr<MockEuroScopeCFlightPlanInterface> mockFlightplan1(
                new NiceMock<MockEuroScopeCFlightPlanInterface>
            );
            std::shared_ptr<MockEuroScopeCRadarTargetInterface> mockRadarTarget1(
                new NiceMock<MockEuroScopeCRadarTargetInterface>
            );
            std::shared_ptr<MockEuroScopeCFlightPlanInterface> mockFlightplan2(
                new NiceMock<MockEuroScopeCFlightPlanInterface>
            );
            std::shared_ptr<MockEuroScopeCRadarTargetInterface> mockRadarTarget2(
                new NiceMock<MockEuroScopeCRadarTargetInterface>
            );

            StrictMock<MockEuroscopePluginLoopbackInterface> mockEuroscopePlugin;

            EXPECT_CALL(mockEuroscopePlugin, GetFlightplanForCallsign("BAW123"))
                .Times(0);

            StoredFlightplanCollection flightplans;
            flightplans.UpdatePlan(StoredFlightplan("BAW123", "EGKK", "EDDM"));
            flightplans.UpdatePlan(StoredFlightplan("EZY456", "EGLL", "EIDW"));

            std::shared_ptr<StrictMock<MockInitialAltitudeEventHandler>> initialAltitudeEventHandler;
            std::shared_ptr<StrictMock<MockSquawkEventHandler>> mockSquawkEventHandler(
                new StrictMock<MockSquawkEventHandler>
            );

            MassEvent mass(mockEuroscopePlugin, initialAltitudeEventHandler, flightplans);
            mass.SetAllInitialAltitudes();
        }
    }  // namespace EventHandler
}  // namespace UKControllerPluginTest
