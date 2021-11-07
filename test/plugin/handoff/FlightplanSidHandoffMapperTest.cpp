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
        FlightplanSidHandoffMapperTest()
            : handoffs(std::make_shared<HandoffCollection>()), sids(std::make_shared<SidCollection>()),
              mapper(handoffs, sids)
        {
            ON_CALL(mockFlightplan, GetOrigin).WillByDefault(testing::Return("EGKK"));

            sids->AddSid(std::make_shared<StandardInstrumentDeparture>("EGKK", "CLN3X", 5000, 200, 1));
            sids->AddSid(std::make_shared<StandardInstrumentDeparture>("EGKK", "CLN3Y", 5000, 200, 55));
            sids->AddSid(std::make_shared<StandardInstrumentDeparture>("EGKK", "CLN3Z", 5000, 200, 0));
            handoffs->Add(std::make_shared<HandoffOrder>(1, nullptr));
            handoffs->Add(std::make_shared<HandoffOrder>(2, nullptr));
        }

        testing::NiceMock<Euroscope::MockEuroScopeCFlightPlanInterface> mockFlightplan;
        std::shared_ptr<HandoffCollection> handoffs;
        std::shared_ptr<SidCollection> sids;
        FlightplanSidHandoffMapper mapper;
    };

    TEST_F(FlightplanSidHandoffMapperTest, ItReturnsNullptrIfNoSidFound)
    {
        ON_CALL(mockFlightplan, GetSidName).WillByDefault(testing::Return("LAM5M"));
        EXPECT_EQ(nullptr, mapper.MapForFlightplan(mockFlightplan));
    }

    TEST_F(FlightplanSidHandoffMapperTest, ItReturnsNullptrIfSidDoesntHaveHandoff)
    {
        ON_CALL(mockFlightplan, GetSidName).WillByDefault(testing::Return("CLN3Z"));
        EXPECT_EQ(nullptr, mapper.MapForFlightplan(mockFlightplan));
    }

    TEST_F(FlightplanSidHandoffMapperTest, ItReturnsNullptrIfHandoffDoesntExist)
    {
        ON_CALL(mockFlightplan, GetSidName).WillByDefault(testing::Return("CLN3Y"));
        EXPECT_EQ(nullptr, mapper.MapForFlightplan(mockFlightplan));
    }

    TEST_F(FlightplanSidHandoffMapperTest, ItReturnsMappedHandoff)
    {
        ON_CALL(mockFlightplan, GetSidName).WillByDefault(testing::Return("CLN3X"));
        EXPECT_EQ(sids->GetByAirfieldAndIdentifier("EGKK", "CLN3X"), mapper.MapForFlightplan(mockFlightplan));
    }
} // namespace UKControllerPluginTest::Handoff
