#include "handoff/FlightplanSidHandoffMapper.h"
#include "handoff/HandoffCollection.h"
#include "handoff/HandoffOrder.h"
#include "sid/SidCollection.h"
#include "sid/StandardInstrumentDeparture.h"

using UKControllerPlugin::Handoff::FlightplanSidHandoffMapper;
using UKControllerPlugin::Handoff::HandoffCollection;
using UKControllerPlugin::Handoff::HandoffOrder;
using UKControllerPlugin::Sid::SidCollection;
using UKControllerPlugin::Sid::StandardInstrumentDeparture;

namespace UKControllerPluginTest::Handoff {
    class FlightplanSidHandoffMapperTest : public testing::Test
    {
        public:
        FlightplanSidHandoffMapperTest() : mapper(handoffs, sidMapper)
        {
            ON_CALL(mockFlightplan, GetOrigin).WillByDefault(testing::Return("EGKK"));

            sid1 = std::make_shared<StandardInstrumentDeparture>(1, 2, "CLN3X", 5000, 200, 1);
            sid2 = std::make_shared<StandardInstrumentDeparture>(2, 3, "CLN3Y", 5000, 200, 55);
            sid3 = std::make_shared<StandardInstrumentDeparture>(3, 4, "CLN3Z", 5000, 200, 0);
            handoffs.Add(std::make_shared<HandoffOrder>(1, nullptr));
            handoffs.Add(std::make_shared<HandoffOrder>(2, nullptr));
        }

        std::shared_ptr<StandardInstrumentDeparture> sid1;
        std::shared_ptr<StandardInstrumentDeparture> sid2;
        std::shared_ptr<StandardInstrumentDeparture> sid3;
        testing::NiceMock<Sid::MockSidMapperInterface> sidMapper;
        testing::NiceMock<Euroscope::MockEuroScopeCFlightPlanInterface> mockFlightplan;
        SidCollection sids;
        HandoffCollection handoffs;
        FlightplanSidHandoffMapper mapper;
    };

    TEST_F(FlightplanSidHandoffMapperTest, ItReturnsNullptrIfNoSidFound)
    {
        EXPECT_CALL(sidMapper, MapFlightplanToSid(testing::Ref(mockFlightplan)))
            .Times(1)
            .WillOnce(testing::Return(nullptr));

        EXPECT_EQ(nullptr, mapper.MapForFlightplan(mockFlightplan));
    }

    TEST_F(FlightplanSidHandoffMapperTest, ItReturnsNullptrIfSidDoesntHaveHandoff)
    {
        EXPECT_CALL(sidMapper, MapFlightplanToSid(testing::Ref(mockFlightplan)))
            .Times(1)
            .WillOnce(testing::Return(sid3));

        ON_CALL(mockFlightplan, GetSidName).WillByDefault(testing::Return("CLN3Z"));
        EXPECT_EQ(nullptr, mapper.MapForFlightplan(mockFlightplan));
    }

    TEST_F(FlightplanSidHandoffMapperTest, ItReturnsNullptrIfHandoffDoesntExist)
    {
        EXPECT_CALL(sidMapper, MapFlightplanToSid(testing::Ref(mockFlightplan)))
            .Times(1)
            .WillOnce(testing::Return(sid2));

        EXPECT_EQ(nullptr, mapper.MapForFlightplan(mockFlightplan));
    }

    TEST_F(FlightplanSidHandoffMapperTest, ItReturnsMappedHandoff)
    {
        EXPECT_CALL(sidMapper, MapFlightplanToSid(testing::Ref(mockFlightplan)))
            .Times(1)
            .WillOnce(testing::Return(sid1));

        EXPECT_EQ(handoffs.Get(1), mapper.MapForFlightplan(mockFlightplan));
    }
} // namespace UKControllerPluginTest::Handoff
