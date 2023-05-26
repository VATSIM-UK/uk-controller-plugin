#include "mock/MockEuroScopeCFlightplanInterface.h"
#include "mock/MockEuroscopePluginLoopbackInterface.h"
#include "mock/MockSidMapperInterface.h"
#include "sid/StandardInstrumentDeparture.h"
#include "initialheading/ClearInitialHeading.h"

namespace UKControllerPluginTest::InitialHeading {

    class ClearInitialHeadingTest : public testing::Test
    {
        public:
        ClearInitialHeadingTest()
            : sid(std::make_shared<UKControllerPlugin::Sid::StandardInstrumentDeparture>(
                  1, 2, "ADMAG2X", 6000, 125, 1)),
              sid2(std::make_shared<UKControllerPlugin::Sid::StandardInstrumentDeparture>(2, 2, "ADMAG2X", 6000, 0, 1)),
              mockFlightplan(std::make_shared<testing::NiceMock<Euroscope::MockEuroScopeCFlightPlanInterface>>()),
              clear(mockPlugin, sidMapper)
        {
            ON_CALL(mockPlugin, GetFlightplanForCallsign("BAW123")).WillByDefault(testing::Return(mockFlightplan));
        }

        std::shared_ptr<UKControllerPlugin::Sid::StandardInstrumentDeparture> sid;
        std::shared_ptr<UKControllerPlugin::Sid::StandardInstrumentDeparture> sid2;
        testing::NiceMock<Sid::MockSidMapperInterface> sidMapper;
        testing::NiceMock<Euroscope::MockEuroscopePluginLoopbackInterface> mockPlugin;
        std::shared_ptr<testing::NiceMock<Euroscope::MockEuroScopeCFlightPlanInterface>> mockFlightplan;
        UKControllerPlugin::InitialHeading::ClearInitialHeading clear;
    };

    TEST_F(ClearInitialHeadingTest, ItClearsTheInitialHeadingForUntrackedAircraft)
    {
        ON_CALL(*mockFlightplan, IsTracked).WillByDefault(testing::Return(false));

        ON_CALL(*mockFlightplan, GetAssignedHeading).WillByDefault(testing::Return(125));

        ON_CALL(sidMapper, MapFlightplanToSid(testing::Ref(*mockFlightplan))).WillByDefault(testing::Return(sid));

        EXPECT_CALL(*mockFlightplan, SetHeading(0)).Times(1);

        clear.OnEvent({"BAW123"});
    }

    TEST_F(ClearInitialHeadingTest, ItClearsTheInitialHeadingForAircraftTrackedByTheUser)
    {
        ON_CALL(*mockFlightplan, IsTracked).WillByDefault(testing::Return(true));

        ON_CALL(*mockFlightplan, IsTrackedByUser).WillByDefault(testing::Return(true));

        ON_CALL(*mockFlightplan, GetAssignedHeading).WillByDefault(testing::Return(125));

        ON_CALL(sidMapper, MapFlightplanToSid(testing::Ref(*mockFlightplan))).WillByDefault(testing::Return(sid));

        EXPECT_CALL(*mockFlightplan, SetHeading(0)).Times(1);

        clear.OnEvent({"BAW123"});
    }

    TEST_F(ClearInitialHeadingTest, ItDoesntClearHeadingIfNotSetAndSidHasNoAssignedHeading)
    {
        ON_CALL(*mockFlightplan, IsTracked).WillByDefault(testing::Return(true));

        ON_CALL(*mockFlightplan, IsTrackedByUser).WillByDefault(testing::Return(true));

        ON_CALL(*mockFlightplan, GetAssignedHeading).WillByDefault(testing::Return(0));

        ON_CALL(sidMapper, MapFlightplanToSid(testing::Ref(*mockFlightplan))).WillByDefault(testing::Return(sid2));

        EXPECT_CALL(*mockFlightplan, SetHeading(0)).Times(0);

        clear.OnEvent({"BAW123"});
    }

    TEST_F(ClearInitialHeadingTest, ItDoesntClearHeadingIfChangedByController)
    {
        ON_CALL(*mockFlightplan, IsTracked).WillByDefault(testing::Return(true));

        ON_CALL(*mockFlightplan, IsTrackedByUser).WillByDefault(testing::Return(true));

        ON_CALL(*mockFlightplan, GetAssignedHeading).WillByDefault(testing::Return(126));

        ON_CALL(sidMapper, MapFlightplanToSid(testing::Ref(*mockFlightplan))).WillByDefault(testing::Return(sid));

        EXPECT_CALL(*mockFlightplan, SetHeading(0)).Times(0);

        clear.OnEvent({"BAW123"});
    }

    TEST_F(ClearInitialHeadingTest, ItDoesntClearHeadingIfNoMappedSid)
    {
        ON_CALL(*mockFlightplan, IsTracked).WillByDefault(testing::Return(true));

        ON_CALL(*mockFlightplan, IsTrackedByUser).WillByDefault(testing::Return(true));

        ON_CALL(sidMapper, MapFlightplanToSid(testing::Ref(*mockFlightplan))).WillByDefault(testing::Return(nullptr));

        ON_CALL(*mockFlightplan, GetAssignedHeading).WillByDefault(testing::Return(125));

        EXPECT_CALL(*mockFlightplan, SetHeading(0)).Times(0);

        clear.OnEvent({"BAW123"});
    }

    TEST_F(ClearInitialHeadingTest, ItDoesntClearHeadingIfTrackedBySomeoneElse)
    {
        ON_CALL(*mockFlightplan, IsTracked).WillByDefault(testing::Return(true));

        ON_CALL(*mockFlightplan, IsTrackedByUser).WillByDefault(testing::Return(false));

        ON_CALL(sidMapper, MapFlightplanToSid(testing::Ref(*mockFlightplan))).WillByDefault(testing::Return(sid));

        ON_CALL(*mockFlightplan, GetAssignedHeading).WillByDefault(testing::Return(125));

        EXPECT_CALL(*mockFlightplan, SetHeading(0)).Times(0);

        clear.OnEvent({"BAW123"});
    }

    TEST_F(ClearInitialHeadingTest, ItDoesntClearHeadingIfFlightplanNotFound)
    {
        ON_CALL(mockPlugin, GetFlightplanForCallsign("BAW123")).WillByDefault(testing::Return(nullptr));

        ON_CALL(*mockFlightplan, IsTracked).WillByDefault(testing::Return(true));

        ON_CALL(*mockFlightplan, IsTrackedByUser).WillByDefault(testing::Return(true));

        ON_CALL(sidMapper, MapFlightplanToSid(testing::Ref(*mockFlightplan))).WillByDefault(testing::Return(sid));

        ON_CALL(*mockFlightplan, GetAssignedHeading).WillByDefault(testing::Return(125));

        EXPECT_CALL(*mockFlightplan, SetHeading(0)).Times(0);

        clear.OnEvent({"BAW123"});
    }
} // namespace UKControllerPluginTest::InitialHeading
