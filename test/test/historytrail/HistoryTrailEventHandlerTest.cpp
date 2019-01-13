#include "pch/pch.h"
#include "historytrail/HistoryTrailRepository.h"
#include "historytrail/HistoryTrailEventHandler.h"
#include "mock/MockEuroScopeCRadarTargetInterface.h"
#include "mock/MockEuroScopeCFlightplanInterface.h"

using UKControllerPlugin::HistoryTrail::HistoryTrailRepository;
using UKControllerPlugin::HistoryTrail::HistoryTrailEventHandler;
using UKControllerPluginTest::Euroscope::MockEuroScopeCRadarTargetInterface;
using UKControllerPluginTest::Euroscope::MockEuroScopeCFlightPlanInterface;
using ::testing::StrictMock;
using ::testing::Return;

namespace UKControllerPluginTest {
    namespace EventHandler {
         TEST(HistoryTrailEventHandler, RadarTargetPositionUpdateEventAddsAnAircraftIfNotRegistered)
         {
             StrictMock<MockEuroScopeCRadarTargetInterface> radarTarget;
             EXPECT_CALL(radarTarget, GetCallsign())
                 .Times(3)
                 .WillRepeatedly(Return("Test"));

             EXPECT_CALL(radarTarget, GetPosition())
                 .Times(1)
                 .WillOnce(Return(EuroScopePlugIn::CPosition::CPosition()));

             HistoryTrailRepository repo;
             HistoryTrailEventHandler handler(repo);

             handler.RadarTargetPositionUpdateEvent(radarTarget);
             EXPECT_TRUE(repo.HasAircraft("Test"));
         }

         TEST(HistoryTrailEventHandler, RadarTargetPositionUpdateEventDoesNotAddAnAircraftTwice)
         {
             StrictMock<MockEuroScopeCRadarTargetInterface> radarTarget;
             EXPECT_CALL(radarTarget, GetCallsign())
                 .Times(5)
                 .WillRepeatedly(Return("Test"));

             EXPECT_CALL(radarTarget, GetPosition())
                 .Times(2)
                 .WillRepeatedly(Return(EuroScopePlugIn::CPosition::CPosition()));

             HistoryTrailRepository repo;
             HistoryTrailEventHandler handler(repo);

             handler.RadarTargetPositionUpdateEvent(radarTarget);
             handler.RadarTargetPositionUpdateEvent(radarTarget);
         }

         TEST(HistoryTrailEventHandler, OnFlightplanDisconnectRemovesAircraft)
         {
             StrictMock<MockEuroScopeCRadarTargetInterface> radarTarget;
             EXPECT_CALL(radarTarget, GetCallsign())
                 .Times(3)
                 .WillRepeatedly(Return("Test"));

             EXPECT_CALL(radarTarget, GetPosition())
                 .Times(1)
                 .WillOnce(Return(EuroScopePlugIn::CPosition::CPosition()));

             StrictMock<MockEuroScopeCFlightPlanInterface> flightPlan;
             EXPECT_CALL(flightPlan, GetCallsign())
                 .Times(1)
                 .WillOnce(Return("Test"));

             HistoryTrailRepository repo;
             HistoryTrailEventHandler handler(repo);

             handler.RadarTargetPositionUpdateEvent(radarTarget);
             handler.FlightPlanDisconnectEvent(flightPlan);
             EXPECT_FALSE(repo.HasAircraft("Test"));
         }
    }  // namespace EventHandler
}  // namespace UKControllerPluginTest
