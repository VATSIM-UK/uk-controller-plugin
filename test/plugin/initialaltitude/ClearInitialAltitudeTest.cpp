#include "mock/MockEuroScopeCFlightplanInterface.h"
#include "mock/MockEuroscopePluginLoopbackInterface.h"
#include "mock/MockSidMapperInterface.h"
#include "sid/StandardInstrumentDeparture.h"
#include "initialaltitude/ClearInitialAltitude.h"

namespace UKControllerPluginTest::InitialAltitude {

    class ClearInitialAltitudeTest : public testing::Test
    {
        public:
        ClearInitialAltitudeTest()
            : sid(std::make_shared<UKControllerPlugin::Sid::StandardInstrumentDeparture>(
                  1, 2, "ADMAG2X", 6000, 125, 1)),
              mockFlightplan(std::make_shared<testing::NiceMock<Euroscope::MockEuroScopeCFlightPlanInterface>>()),
              clear(mockPlugin, sidMapper)
        {
            ON_CALL(mockPlugin, GetFlightplanForCallsign("BAW123")).WillByDefault(testing::Return(mockFlightplan));
        }

        std::shared_ptr<UKControllerPlugin::Sid::StandardInstrumentDeparture> sid;
        testing::NiceMock<Sid::MockSidMapperInterface> sidMapper;
        testing::NiceMock<Euroscope::MockEuroscopePluginLoopbackInterface> mockPlugin;
        std::shared_ptr<testing::NiceMock<Euroscope::MockEuroScopeCFlightPlanInterface>> mockFlightplan;
        UKControllerPlugin::InitialAltitude::ClearInitialAltitude clear;
    };

    TEST_F(ClearInitialAltitudeTest, ItClearsTheInitialAltitudeForUntrackedAircraft)
    {
        ON_CALL(*mockFlightplan, IsTracked).WillByDefault(testing::Return(false));

        ON_CALL(*mockFlightplan, GetClearedAltitude).WillByDefault(testing::Return(6000));

        ON_CALL(sidMapper, MapFlightplanToSid(testing::Ref(*mockFlightplan))).WillByDefault(testing::Return(sid));

        EXPECT_CALL(*mockFlightplan, SetClearedAltitude(0)).Times(1);

        clear.OnEvent({"BAW123"});
    }

    TEST_F(ClearInitialAltitudeTest, ItClearsTheInitialAltitudeForAircraftTrackedByTheUser)
    {
        ON_CALL(*mockFlightplan, IsTracked).WillByDefault(testing::Return(true));

        ON_CALL(*mockFlightplan, IsTrackedByUser).WillByDefault(testing::Return(true));

        ON_CALL(*mockFlightplan, GetClearedAltitude).WillByDefault(testing::Return(6000));

        ON_CALL(sidMapper, MapFlightplanToSid(testing::Ref(*mockFlightplan))).WillByDefault(testing::Return(sid));

        EXPECT_CALL(*mockFlightplan, SetClearedAltitude(0)).Times(1);

        clear.OnEvent({"BAW123"});
    }

    TEST_F(ClearInitialAltitudeTest, ItDoesntClearAltitudeIfChangedByController)
    {
        ON_CALL(*mockFlightplan, IsTracked).WillByDefault(testing::Return(true));

        ON_CALL(*mockFlightplan, IsTrackedByUser).WillByDefault(testing::Return(true));

        ON_CALL(*mockFlightplan, GetClearedAltitude).WillByDefault(testing::Return(5000));

        ON_CALL(sidMapper, MapFlightplanToSid(testing::Ref(*mockFlightplan))).WillByDefault(testing::Return(sid));

        EXPECT_CALL(*mockFlightplan, SetClearedAltitude(0)).Times(0);

        clear.OnEvent({"BAW123"});
    }

    TEST_F(ClearInitialAltitudeTest, ItDoesntClearAltitudeIfNoMappedSid)
    {
        ON_CALL(*mockFlightplan, IsTracked).WillByDefault(testing::Return(true));

        ON_CALL(*mockFlightplan, IsTrackedByUser).WillByDefault(testing::Return(true));

        ON_CALL(sidMapper, MapFlightplanToSid(testing::Ref(*mockFlightplan))).WillByDefault(testing::Return(nullptr));

        ON_CALL(*mockFlightplan, GetClearedAltitude).WillByDefault(testing::Return(6000));

        EXPECT_CALL(*mockFlightplan, SetClearedAltitude(0)).Times(0);

        clear.OnEvent({"BAW123"});
    }

    TEST_F(ClearInitialAltitudeTest, ItDoesntClearAltitudeIfTrackedBySomeoneElse)
    {
        ON_CALL(*mockFlightplan, IsTracked).WillByDefault(testing::Return(true));

        ON_CALL(*mockFlightplan, IsTrackedByUser).WillByDefault(testing::Return(false));

        ON_CALL(sidMapper, MapFlightplanToSid(testing::Ref(*mockFlightplan))).WillByDefault(testing::Return(sid));

        ON_CALL(*mockFlightplan, GetClearedAltitude).WillByDefault(testing::Return(6000));

        EXPECT_CALL(*mockFlightplan, SetClearedAltitude(0)).Times(0);

        clear.OnEvent({"BAW123"});
    }

    TEST_F(ClearInitialAltitudeTest, ItDoesntClearAltitudeIfFlightplanNotFound)
    {
        ON_CALL(mockPlugin, GetFlightplanForCallsign("BAW123")).WillByDefault(testing::Return(nullptr));

        ON_CALL(*mockFlightplan, IsTracked).WillByDefault(testing::Return(true));

        ON_CALL(*mockFlightplan, IsTrackedByUser).WillByDefault(testing::Return(true));

        ON_CALL(sidMapper, MapFlightplanToSid(testing::Ref(*mockFlightplan))).WillByDefault(testing::Return(sid));

        ON_CALL(*mockFlightplan, GetClearedAltitude).WillByDefault(testing::Return(6000));

        EXPECT_CALL(*mockFlightplan, SetClearedAltitude(0)).Times(0);

        clear.OnEvent({"BAW123"});
    }
} // namespace UKControllerPluginTest::InitialAltitude
