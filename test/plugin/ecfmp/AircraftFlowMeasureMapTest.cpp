#include "controller/ActiveCallsign.h"
#include "controller/ControllerPosition.h"
#include "ecfmp/AircraftFlowMeasureMap.h"
#include "ECFMP/SdkEvents.h"
#include "mock/FlowMeasureMock.h"
#include "mock/MockEuroScopeCRadarTargetInterface.h"
#include "mock/MockEuroscopePluginLoopbackInterface.h"

namespace UKControllerPluginTest::ECFMP {
    class AircraftFlowMeasureMapTest : public testing::Test
    {
        public:
        AircraftFlowMeasureMapTest()
            : mockFlowMeasure1(std::make_shared<::testing::NiceMock<::ECFMP::Mock::FlowMeasure::FlowMeasureMock>>()),
              mockFlowMeasure2(std::make_shared<::testing::NiceMock<::ECFMP::Mock::FlowMeasure::FlowMeasureMock>>()),
              mockFlightplan1(std::make_shared<testing::NiceMock<Euroscope::MockEuroScopeCFlightPlanInterface>>()),
              mockRadarTarget1(std::make_shared<testing::NiceMock<Euroscope::MockEuroScopeCRadarTargetInterface>>()),
              mockFlightplan2(std::make_shared<testing::NiceMock<Euroscope::MockEuroScopeCFlightPlanInterface>>()),
              mockRadarTarget2(std::make_shared<testing::NiceMock<Euroscope::MockEuroScopeCRadarTargetInterface>>()),
              map(mockPlugin)
        {
            ON_CALL(*mockFlightplan1, GetCallsign).WillByDefault(testing::Return("BAW123"));
            ON_CALL(*mockFlightplan2, GetCallsign).WillByDefault(testing::Return("BAW456"));

            ON_CALL(*mockFlightplan1, GetEuroScopeObject).WillByDefault(testing::ReturnRef(euroscopeFlightplan1));
            ON_CALL(*mockFlightplan2, GetEuroScopeObject).WillByDefault(testing::ReturnRef(euroscopeFlightplan2));

            ON_CALL(*mockRadarTarget1, GetEuroScopeObject).WillByDefault(testing::ReturnRef(euroscopeRadarTarget1));
            ON_CALL(*mockRadarTarget2, GetEuroScopeObject).WillByDefault(testing::ReturnRef(euroscopeRadarTarget2));

            mockPlugin.AddAllFlightplansItem({mockFlightplan1, mockRadarTarget1});
            mockPlugin.AddAllFlightplansItem({mockFlightplan2, mockRadarTarget2});

            ON_CALL(*mockFlowMeasure1, Id).WillByDefault(testing::Return(1));
            ON_CALL(*mockFlowMeasure2, Id).WillByDefault(testing::Return(2));
        }

        std::shared_ptr<::ECFMP::Mock::FlowMeasure::FlowMeasureMock> mockFlowMeasure1;
        std::shared_ptr<::ECFMP::Mock::FlowMeasure::FlowMeasureMock> mockFlowMeasure2;
        EuroScopePlugIn::CFlightPlan euroscopeFlightplan1;
        EuroScopePlugIn::CFlightPlan euroscopeFlightplan2;
        EuroScopePlugIn::CRadarTarget euroscopeRadarTarget1;
        EuroScopePlugIn::CRadarTarget euroscopeRadarTarget2;
        std::shared_ptr<testing::NiceMock<Euroscope::MockEuroScopeCFlightPlanInterface>> mockFlightplan1;
        std::shared_ptr<testing::NiceMock<Euroscope::MockEuroScopeCRadarTargetInterface>> mockRadarTarget1;
        std::shared_ptr<testing::NiceMock<Euroscope::MockEuroScopeCFlightPlanInterface>> mockFlightplan2;
        std::shared_ptr<testing::NiceMock<Euroscope::MockEuroScopeCRadarTargetInterface>> mockRadarTarget2;
        testing::NiceMock<Euroscope::MockEuroscopePluginLoopbackInterface> mockPlugin;
        UKControllerPlugin::ECFMP::AircraftFlowMeasureMap map;
    };

    TEST_F(AircraftFlowMeasureMapTest, FlowMeasuresActivatingUpdateAircraftsActiveFlowMeasures)
    {
        // Flightplan 1 will be applicable to the measure, flightplan 2, not so
        EXPECT_CALL(*mockFlowMeasure1, ApplicableToAircraft(testing::_, testing::_))
            .WillOnce(testing::Return(true))
            .WillOnce(testing::Return(false));

        map.OnEvent(::ECFMP::Plugin::FlowMeasureActivatedEvent{mockFlowMeasure1});

        // Check that flightplan 1 has the measure as applicable, but flightplan does not
        EXPECT_EQ(1, map.GetFlowMeasuresForCallsign("BAW123").size());
        EXPECT_EQ(mockFlowMeasure1, *map.GetFlowMeasuresForCallsign("BAW123").begin());
        EXPECT_EQ(0, map.GetFlowMeasuresForCallsign("BAW456").size());
    }

    TEST_F(AircraftFlowMeasureMapTest, FlowMeasuresWithdrawingRemoveThemFromAircraftsActiveFlowMeasures)
    {
        // Flightplan 1 will be applicable to the measure, flightplan 2, not so
        EXPECT_CALL(*mockFlowMeasure1, ApplicableToAircraft(testing::_, testing::_))
            .WillOnce(testing::Return(true))
            .WillOnce(testing::Return(false));

        EXPECT_CALL(*mockFlowMeasure2, ApplicableToAircraft(testing::_, testing::_))
            .WillOnce(testing::Return(true))
            .WillOnce(testing::Return(true));

        map.OnEvent(::ECFMP::Plugin::FlowMeasureActivatedEvent{mockFlowMeasure1});
        map.OnEvent(::ECFMP::Plugin::FlowMeasureActivatedEvent{mockFlowMeasure2});

        // Check that flightplan 1 has both measures, but flightplan 2 has only the second
        EXPECT_EQ(2, map.GetFlowMeasuresForCallsign("BAW123").size());
        EXPECT_EQ(mockFlowMeasure1, *map.GetFlowMeasuresForCallsign("BAW123").begin());
        EXPECT_EQ(mockFlowMeasure2, *(++map.GetFlowMeasuresForCallsign("BAW123").begin()));
        EXPECT_EQ(1, map.GetFlowMeasuresForCallsign("BAW456").size());
        EXPECT_EQ(mockFlowMeasure2, *map.GetFlowMeasuresForCallsign("BAW456").begin());

        // Withdraw the first measure
        map.OnEvent(::ECFMP::Plugin::FlowMeasureWithdrawnEvent{mockFlowMeasure1});

        // Check that flightplan 1 now has just the second measure, but flightplan 2 has only the second
        EXPECT_EQ(1, map.GetFlowMeasuresForCallsign("BAW123").size());
        EXPECT_EQ(mockFlowMeasure2, *map.GetFlowMeasuresForCallsign("BAW123").begin());
        EXPECT_EQ(1, map.GetFlowMeasuresForCallsign("BAW456").size());
        EXPECT_EQ(mockFlowMeasure2, *map.GetFlowMeasuresForCallsign("BAW456").begin());
    }

    TEST_F(AircraftFlowMeasureMapTest, FlowMeasuresWithdrawingRemovesAllFromActiveAircraftFlowMeasures)
    {
        // Flightplan 1 will be applicable to the measure, flightplan 2, not so
        EXPECT_CALL(*mockFlowMeasure1, ApplicableToAircraft(testing::_, testing::_))
            .WillOnce(testing::Return(true))
            .WillOnce(testing::Return(false));

        EXPECT_CALL(*mockFlowMeasure2, ApplicableToAircraft(testing::_, testing::_))
            .WillOnce(testing::Return(true))
            .WillOnce(testing::Return(true));

        map.OnEvent(::ECFMP::Plugin::FlowMeasureActivatedEvent{mockFlowMeasure1});
        map.OnEvent(::ECFMP::Plugin::FlowMeasureActivatedEvent{mockFlowMeasure2});

        // Check that flightplan 1 has both measures, but flightplan 2 has only the second
        EXPECT_EQ(2, map.GetFlowMeasuresForCallsign("BAW123").size());
        EXPECT_EQ(mockFlowMeasure1, *map.GetFlowMeasuresForCallsign("BAW123").begin());
        EXPECT_EQ(mockFlowMeasure2, *(++map.GetFlowMeasuresForCallsign("BAW123").begin()));
        EXPECT_EQ(1, map.GetFlowMeasuresForCallsign("BAW456").size());
        EXPECT_EQ(mockFlowMeasure2, *map.GetFlowMeasuresForCallsign("BAW456").begin());

        // Withdraw the measures
        map.OnEvent(::ECFMP::Plugin::FlowMeasureWithdrawnEvent{mockFlowMeasure1});
        map.OnEvent(::ECFMP::Plugin::FlowMeasureWithdrawnEvent{mockFlowMeasure2});

        // Check that no flightplan has any measures
        EXPECT_EQ(0, map.GetFlowMeasuresForCallsign("BAW123").size());
        EXPECT_EQ(0, map.GetFlowMeasuresForCallsign("BAW456").size());
    }

    TEST_F(AircraftFlowMeasureMapTest, FlowMeasuresExpiringRemoveThemFromAircraftsActiveFlowMeasures)
    {
        // Flightplan 1 will be applicable to the measure, flightplan 2, not so
        EXPECT_CALL(*mockFlowMeasure1, ApplicableToAircraft(testing::_, testing::_))
            .WillOnce(testing::Return(true))
            .WillOnce(testing::Return(false));

        EXPECT_CALL(*mockFlowMeasure2, ApplicableToAircraft(testing::_, testing::_))
            .WillOnce(testing::Return(true))
            .WillOnce(testing::Return(true));

        map.OnEvent(::ECFMP::Plugin::FlowMeasureActivatedEvent{mockFlowMeasure1});
        map.OnEvent(::ECFMP::Plugin::FlowMeasureActivatedEvent{mockFlowMeasure2});

        // Check that flightplan 1 has both measures, but flightplan 2 has only the second
        EXPECT_EQ(2, map.GetFlowMeasuresForCallsign("BAW123").size());
        EXPECT_EQ(mockFlowMeasure1, *map.GetFlowMeasuresForCallsign("BAW123").begin());
        EXPECT_EQ(mockFlowMeasure2, *(++map.GetFlowMeasuresForCallsign("BAW123").begin()));
        EXPECT_EQ(1, map.GetFlowMeasuresForCallsign("BAW456").size());
        EXPECT_EQ(mockFlowMeasure2, *map.GetFlowMeasuresForCallsign("BAW456").begin());

        // Withdraw the first measure
        map.OnEvent(::ECFMP::Plugin::FlowMeasureExpiredEvent{mockFlowMeasure1});

        // Check that flightplan 1 now has just the second measure, but flightplan 2 has only the second
        EXPECT_EQ(1, map.GetFlowMeasuresForCallsign("BAW123").size());
        EXPECT_EQ(mockFlowMeasure2, *map.GetFlowMeasuresForCallsign("BAW123").begin());
        EXPECT_EQ(1, map.GetFlowMeasuresForCallsign("BAW456").size());
        EXPECT_EQ(mockFlowMeasure2, *map.GetFlowMeasuresForCallsign("BAW456").begin());
    }

    TEST_F(AircraftFlowMeasureMapTest, FlowMeasuresExpiringRemovesAllFromActiveAircraftFlowMeasures)
    {
        // Flightplan 1 will be applicable to the measure, flightplan 2, not so
        EXPECT_CALL(*mockFlowMeasure1, ApplicableToAircraft(testing::_, testing::_))
            .WillOnce(testing::Return(true))
            .WillOnce(testing::Return(false));

        EXPECT_CALL(*mockFlowMeasure2, ApplicableToAircraft(testing::_, testing::_))
            .WillOnce(testing::Return(true))
            .WillOnce(testing::Return(true));

        map.OnEvent(::ECFMP::Plugin::FlowMeasureActivatedEvent{mockFlowMeasure1});
        map.OnEvent(::ECFMP::Plugin::FlowMeasureActivatedEvent{mockFlowMeasure2});

        // Check that flightplan 1 has both measures, but flightplan 2 has only the second
        EXPECT_EQ(2, map.GetFlowMeasuresForCallsign("BAW123").size());
        EXPECT_EQ(mockFlowMeasure1, *map.GetFlowMeasuresForCallsign("BAW123").begin());
        EXPECT_EQ(mockFlowMeasure2, *(++map.GetFlowMeasuresForCallsign("BAW123").begin()));
        EXPECT_EQ(1, map.GetFlowMeasuresForCallsign("BAW456").size());
        EXPECT_EQ(mockFlowMeasure2, *map.GetFlowMeasuresForCallsign("BAW456").begin());

        // Withdraw the measures
        map.OnEvent(::ECFMP::Plugin::FlowMeasureExpiredEvent{mockFlowMeasure1});
        map.OnEvent(::ECFMP::Plugin::FlowMeasureExpiredEvent{mockFlowMeasure2});

        // Check that no flightplan has any measures
        EXPECT_EQ(0, map.GetFlowMeasuresForCallsign("BAW123").size());
        EXPECT_EQ(0, map.GetFlowMeasuresForCallsign("BAW456").size());
    }

    TEST_F(AircraftFlowMeasureMapTest, FlightplansDisconnectingRemoveTheirActiveFlowMeasures)
    {
        // Flightplan 1 will be applicable to the measure, flightplan 2, not so
        EXPECT_CALL(*mockFlowMeasure1, ApplicableToAircraft(testing::_, testing::_))
            .WillOnce(testing::Return(true))
            .WillOnce(testing::Return(false));

        EXPECT_CALL(*mockFlowMeasure2, ApplicableToAircraft(testing::_, testing::_))
            .WillOnce(testing::Return(true))
            .WillOnce(testing::Return(true));

        map.OnEvent(::ECFMP::Plugin::FlowMeasureActivatedEvent{mockFlowMeasure1});
        map.OnEvent(::ECFMP::Plugin::FlowMeasureActivatedEvent{mockFlowMeasure2});

        // Check that flightplan 1 has both measures, but flightplan 2 has only the second
        EXPECT_EQ(2, map.GetFlowMeasuresForCallsign("BAW123").size());
        EXPECT_EQ(mockFlowMeasure1, *map.GetFlowMeasuresForCallsign("BAW123").begin());
        EXPECT_EQ(mockFlowMeasure2, *(++map.GetFlowMeasuresForCallsign("BAW123").begin()));
        EXPECT_EQ(1, map.GetFlowMeasuresForCallsign("BAW456").size());
        EXPECT_EQ(mockFlowMeasure2, *map.GetFlowMeasuresForCallsign("BAW456").begin());

        // Flightplan 1 disconnects
        map.FlightPlanDisconnectEvent(*mockFlightplan1);

        // Check that flightplan 1 has no measures and flightplan 2 is unaffected
        EXPECT_EQ(0, map.GetFlowMeasuresForCallsign("BAW123").size());
        EXPECT_EQ(1, map.GetFlowMeasuresForCallsign("BAW456").size());
        EXPECT_EQ(mockFlowMeasure2, *map.GetFlowMeasuresForCallsign("BAW456").begin());
    }

    TEST_F(AircraftFlowMeasureMapTest, FlightplansChangingRecalculateTheFlowMeasures)
    {
        // Flightplan 1 will be applicable to the measure, flightplan 2, not so
        EXPECT_CALL(*mockFlowMeasure1, ApplicableToAircraft(testing::_, testing::_))
            .WillOnce(testing::Return(true))
            .WillOnce(testing::Return(false))
            .WillOnce(testing::Return(false));

        EXPECT_CALL(*mockFlowMeasure2, ApplicableToAircraft(testing::_, testing::_))
            .WillOnce(testing::Return(true))
            .WillOnce(testing::Return(true))
            .WillOnce(testing::Return(true));

        map.OnEvent(::ECFMP::Plugin::FlowMeasureActivatedEvent{mockFlowMeasure1});
        map.OnEvent(::ECFMP::Plugin::FlowMeasureActivatedEvent{mockFlowMeasure2});

        // Check that flightplan 1 has both measures, but flightplan 2 has only the second
        EXPECT_EQ(2, map.GetFlowMeasuresForCallsign("BAW123").size());
        EXPECT_EQ(mockFlowMeasure1, *map.GetFlowMeasuresForCallsign("BAW123").begin());
        EXPECT_EQ(mockFlowMeasure2, *(++map.GetFlowMeasuresForCallsign("BAW123").begin()));
        EXPECT_EQ(1, map.GetFlowMeasuresForCallsign("BAW456").size());
        EXPECT_EQ(mockFlowMeasure2, *map.GetFlowMeasuresForCallsign("BAW456").begin());

        // Flightplan 1 changes
        map.FlightPlanEvent(*mockFlightplan1, *mockRadarTarget1);

        // Check that flightplan 1 now only has the second measure flightplan 2 is unaffected
        EXPECT_EQ(1, map.GetFlowMeasuresForCallsign("BAW123").size());
        EXPECT_EQ(mockFlowMeasure2, *map.GetFlowMeasuresForCallsign("BAW123").begin());
        EXPECT_EQ(1, map.GetFlowMeasuresForCallsign("BAW456").size());
        EXPECT_EQ(mockFlowMeasure2, *map.GetFlowMeasuresForCallsign("BAW456").begin());
    }

    TEST_F(AircraftFlowMeasureMapTest, ItHandlesUnknownCallsignsWhenGettingMeasures)
    {
        EXPECT_EQ(0, map.GetFlowMeasuresForCallsign("BAW999").size());
    }

    TEST_F(AircraftFlowMeasureMapTest, ActiveCallsignAddedClearsMapIfIsUserCallsign)
    {
        UKControllerPlugin::Controller::ControllerPosition position(1, "EGKK_GND", 121.800, {"EGKK"}, true, false);
        UKControllerPlugin::Controller::ActiveCallsign callsign("EGGD_TWR", "Testy McTestface", position, true);

        // Flightplan 1 will be applicable to the measure, flightplan 2, not so
        EXPECT_CALL(*mockFlowMeasure1, ApplicableToAircraft(testing::_, testing::_))
            .WillOnce(testing::Return(true))
            .WillOnce(testing::Return(false));

        EXPECT_CALL(*mockFlowMeasure2, ApplicableToAircraft(testing::_, testing::_))
            .WillOnce(testing::Return(true))
            .WillOnce(testing::Return(true));

        map.OnEvent(::ECFMP::Plugin::FlowMeasureActivatedEvent{mockFlowMeasure1});
        map.OnEvent(::ECFMP::Plugin::FlowMeasureActivatedEvent{mockFlowMeasure2});

        // Check that flightplan 1 has both measures, but flightplan 2 has only the second
        EXPECT_EQ(2, map.GetFlowMeasuresForCallsign("BAW123").size());
        EXPECT_EQ(mockFlowMeasure1, *map.GetFlowMeasuresForCallsign("BAW123").begin());
        EXPECT_EQ(mockFlowMeasure2, *(++map.GetFlowMeasuresForCallsign("BAW123").begin()));
        EXPECT_EQ(1, map.GetFlowMeasuresForCallsign("BAW456").size());
        EXPECT_EQ(mockFlowMeasure2, *map.GetFlowMeasuresForCallsign("BAW456").begin());

        // Trigger callsign event
        map.ActiveCallsignAdded(callsign);

        // Check that everything is gone
        EXPECT_EQ(0, map.GetFlowMeasuresForCallsign("BAW123").size());
        EXPECT_EQ(0, map.GetFlowMeasuresForCallsign("BAW456").size());
    }

    TEST_F(AircraftFlowMeasureMapTest, ActiveCallsignAddedDoesntClearMapIfIsNotUserCallsign)
    {
        UKControllerPlugin::Controller::ControllerPosition position(1, "EGKK_GND", 121.800, {"EGKK"}, true, false);
        UKControllerPlugin::Controller::ActiveCallsign callsign("EGGD_TWR", "Testy McTestface", position, false);

        // Flightplan 1 will be applicable to the measure, flightplan 2, not so
        EXPECT_CALL(*mockFlowMeasure1, ApplicableToAircraft(testing::_, testing::_))
            .WillOnce(testing::Return(true))
            .WillOnce(testing::Return(false));

        EXPECT_CALL(*mockFlowMeasure2, ApplicableToAircraft(testing::_, testing::_))
            .WillOnce(testing::Return(true))
            .WillOnce(testing::Return(true));

        map.OnEvent(::ECFMP::Plugin::FlowMeasureActivatedEvent{mockFlowMeasure1});
        map.OnEvent(::ECFMP::Plugin::FlowMeasureActivatedEvent{mockFlowMeasure2});

        // Check that flightplan 1 has both measures, but flightplan 2 has only the second
        EXPECT_EQ(2, map.GetFlowMeasuresForCallsign("BAW123").size());
        EXPECT_EQ(mockFlowMeasure1, *map.GetFlowMeasuresForCallsign("BAW123").begin());
        EXPECT_EQ(mockFlowMeasure2, *(++map.GetFlowMeasuresForCallsign("BAW123").begin()));
        EXPECT_EQ(1, map.GetFlowMeasuresForCallsign("BAW456").size());
        EXPECT_EQ(mockFlowMeasure2, *map.GetFlowMeasuresForCallsign("BAW456").begin());

        // Trigger callsign event
        map.ActiveCallsignAdded(callsign);

        // Check that everything is still there
        EXPECT_EQ(2, map.GetFlowMeasuresForCallsign("BAW123").size());
        EXPECT_EQ(mockFlowMeasure1, *map.GetFlowMeasuresForCallsign("BAW123").begin());
        EXPECT_EQ(mockFlowMeasure2, *(++map.GetFlowMeasuresForCallsign("BAW123").begin()));
        EXPECT_EQ(1, map.GetFlowMeasuresForCallsign("BAW456").size());
        EXPECT_EQ(mockFlowMeasure2, *map.GetFlowMeasuresForCallsign("BAW456").begin());
    }

    TEST_F(AircraftFlowMeasureMapTest, ActiveCallsignRemovedClearsMapIfIsUserCallsign)
    {
        UKControllerPlugin::Controller::ControllerPosition position(1, "EGKK_GND", 121.800, {"EGKK"}, true, false);
        UKControllerPlugin::Controller::ActiveCallsign callsign("EGGD_TWR", "Testy McTestface", position, true);

        // Flightplan 1 will be applicable to the measure, flightplan 2, not so
        EXPECT_CALL(*mockFlowMeasure1, ApplicableToAircraft(testing::_, testing::_))
            .WillOnce(testing::Return(true))
            .WillOnce(testing::Return(false));

        EXPECT_CALL(*mockFlowMeasure2, ApplicableToAircraft(testing::_, testing::_))
            .WillOnce(testing::Return(true))
            .WillOnce(testing::Return(true));

        map.OnEvent(::ECFMP::Plugin::FlowMeasureActivatedEvent{mockFlowMeasure1});
        map.OnEvent(::ECFMP::Plugin::FlowMeasureActivatedEvent{mockFlowMeasure2});

        // Check that flightplan 1 has both measures, but flightplan 2 has only the second
        EXPECT_EQ(2, map.GetFlowMeasuresForCallsign("BAW123").size());
        EXPECT_EQ(mockFlowMeasure1, *map.GetFlowMeasuresForCallsign("BAW123").begin());
        EXPECT_EQ(mockFlowMeasure2, *(++map.GetFlowMeasuresForCallsign("BAW123").begin()));
        EXPECT_EQ(1, map.GetFlowMeasuresForCallsign("BAW456").size());
        EXPECT_EQ(mockFlowMeasure2, *map.GetFlowMeasuresForCallsign("BAW456").begin());

        // Trigger callsign event
        map.ActiveCallsignRemoved(callsign);

        // Check that everything is gone
        EXPECT_EQ(0, map.GetFlowMeasuresForCallsign("BAW123").size());
        EXPECT_EQ(0, map.GetFlowMeasuresForCallsign("BAW456").size());
    }

    TEST_F(AircraftFlowMeasureMapTest, ActiveCallsignRemovedDoesntClearMapIfIsNotUserCallsign)
    {
        UKControllerPlugin::Controller::ControllerPosition position(1, "EGKK_GND", 121.800, {"EGKK"}, true, false);
        UKControllerPlugin::Controller::ActiveCallsign callsign("EGGD_TWR", "Testy McTestface", position, false);

        // Flightplan 1 will be applicable to the measure, flightplan 2, not so
        EXPECT_CALL(*mockFlowMeasure1, ApplicableToAircraft(testing::_, testing::_))
            .WillOnce(testing::Return(true))
            .WillOnce(testing::Return(false));

        EXPECT_CALL(*mockFlowMeasure2, ApplicableToAircraft(testing::_, testing::_))
            .WillOnce(testing::Return(true))
            .WillOnce(testing::Return(true));

        map.OnEvent(::ECFMP::Plugin::FlowMeasureActivatedEvent{mockFlowMeasure1});
        map.OnEvent(::ECFMP::Plugin::FlowMeasureActivatedEvent{mockFlowMeasure2});

        // Check that flightplan 1 has both measures, but flightplan 2 has only the second
        EXPECT_EQ(2, map.GetFlowMeasuresForCallsign("BAW123").size());
        EXPECT_EQ(mockFlowMeasure1, *map.GetFlowMeasuresForCallsign("BAW123").begin());
        EXPECT_EQ(mockFlowMeasure2, *(++map.GetFlowMeasuresForCallsign("BAW123").begin()));
        EXPECT_EQ(1, map.GetFlowMeasuresForCallsign("BAW456").size());
        EXPECT_EQ(mockFlowMeasure2, *map.GetFlowMeasuresForCallsign("BAW456").begin());

        // Trigger callsign event
        map.ActiveCallsignRemoved(callsign);

        // Check that everything is still there
        EXPECT_EQ(2, map.GetFlowMeasuresForCallsign("BAW123").size());
        EXPECT_EQ(mockFlowMeasure1, *map.GetFlowMeasuresForCallsign("BAW123").begin());
        EXPECT_EQ(mockFlowMeasure2, *(++map.GetFlowMeasuresForCallsign("BAW123").begin()));
        EXPECT_EQ(1, map.GetFlowMeasuresForCallsign("BAW456").size());
        EXPECT_EQ(mockFlowMeasure2, *map.GetFlowMeasuresForCallsign("BAW456").begin());
    }
} // namespace UKControllerPluginTest::ECFMP
