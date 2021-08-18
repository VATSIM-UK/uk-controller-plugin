#include "pch/pch.h"

#include "historytrail/AircraftHistoryTrail.h"
#include "historytrail/HistoryTrailEventHandler.h"
#include "historytrail/HistoryTrailRepository.h"
#include "mock/MockEuroScopeCFlightplanInterface.h"
#include "mock/MockEuroScopeCRadarTargetInterface.h"

using UKControllerPlugin::HistoryTrail::HistoryTrailEventHandler;
using UKControllerPlugin::HistoryTrail::HistoryTrailRepository;
using UKControllerPluginTest::Euroscope::MockEuroScopeCFlightPlanInterface;
using UKControllerPluginTest::Euroscope::MockEuroScopeCRadarTargetInterface;

using testing::NiceMock;
using ::testing::Return;

namespace UKControllerPluginTest::EventHandler {
    class HistoryTrailEventHandlerTest : public ::testing::Test
    {
        public:
        HistoryTrailEventHandlerTest() : handler(repo)
        {
        }

        const int heading = 123;

        HistoryTrailRepository repo;
        HistoryTrailEventHandler handler;
    };

    TEST_F(HistoryTrailEventHandlerTest, RadarTargetPositionUpdateEventAddsAnAircraftIfNotRegistered)
    {
        NiceMock<MockEuroScopeCRadarTargetInterface> radarTarget;
        EXPECT_CALL(radarTarget, GetCallsign()).Times(3).WillRepeatedly(Return("Test"));

        EXPECT_CALL(radarTarget, GetPosition()).Times(1).WillOnce(Return(EuroScopePlugIn::CPosition()));

        const int heading = 123;
        ON_CALL(radarTarget, GetHeading).WillByDefault(Return(heading));

        handler.RadarTargetPositionUpdateEvent(radarTarget);
        EXPECT_TRUE(repo.HasAircraft("Test"));
        EXPECT_EQ(123, repo.GetAircraft("Test")->GetTrail().front().heading);
    }

    TEST_F(HistoryTrailEventHandlerTest, RadarTargetPositionUpdateEventDoesNotAddAnAircraftTwice)
    {
        NiceMock<MockEuroScopeCRadarTargetInterface> radarTarget;
        EXPECT_CALL(radarTarget, GetCallsign()).Times(5).WillRepeatedly(Return("Test"));

        EXPECT_CALL(radarTarget, GetPosition()).Times(2).WillRepeatedly(Return(EuroScopePlugIn::CPosition()));

        const int heading = 123;
        ON_CALL(radarTarget, GetHeading).WillByDefault(Return(heading));

        handler.RadarTargetPositionUpdateEvent(radarTarget);
        handler.RadarTargetPositionUpdateEvent(radarTarget);
    }

    TEST_F(HistoryTrailEventHandlerTest, OnFlightplanDisconnectRemovesAircraft)
    {
        NiceMock<MockEuroScopeCRadarTargetInterface> radarTarget;
        EXPECT_CALL(radarTarget, GetCallsign()).Times(3).WillRepeatedly(Return("Test"));

        EXPECT_CALL(radarTarget, GetPosition()).Times(1).WillOnce(Return(EuroScopePlugIn::CPosition()));

        const int heading = 123;
        ON_CALL(radarTarget, GetHeading).WillByDefault(Return(heading));

        NiceMock<MockEuroScopeCFlightPlanInterface> flightPlan;
        EXPECT_CALL(flightPlan, GetCallsign()).Times(1).WillOnce(Return("Test"));

        handler.RadarTargetPositionUpdateEvent(radarTarget);
        handler.FlightPlanDisconnectEvent(flightPlan);
        EXPECT_FALSE(repo.HasAircraft("Test"));
    }
} // namespace UKControllerPluginTest::EventHandler
