#include "airfield/AirfieldCollection.h"
#include "airfield/AirfieldModel.h"
#include "controller/ControllerPositionHierarchy.h"
#include "handoff/FlightplanAirfieldHandoffMapper.h"
#include "handoff/HandoffCollection.h"
#include "handoff/HandoffOrder.h"
#include "prenote/PairedAirfieldPrenote.h"

using UKControllerPlugin::Airfield::AirfieldCollection;
using UKControllerPlugin::Airfield::AirfieldModel;
using UKControllerPlugin::Handoff::FlightplanAirfieldHandoffMapper;
using UKControllerPlugin::Handoff::HandoffCollection;
using UKControllerPlugin::Handoff::HandoffOrder;
using UKControllerPlugin::Prenote::PairedAirfieldPrenote;

namespace UKControllerPluginTest::Handoff {
    class FlightplanAirfieldHandoffMapperTest : public testing::Test
    {
        public:
        FlightplanAirfieldHandoffMapperTest() : mapper(handoffs, airfields)
        {
            airfields.AddAirfield(std::make_shared<AirfieldModel>(1, "EGKK", nullptr));
            airfields.AddAirfield(std::make_shared<AirfieldModel>(
                2, "EGLL", nullptr, std::vector<std::shared_ptr<PairedAirfieldPrenote>>{}, 1));
            airfields.AddAirfield(std::make_shared<AirfieldModel>(
                3, "EGBB", nullptr, std::vector<std::shared_ptr<PairedAirfieldPrenote>>{}, 3));
            handoffs.Add(std::make_shared<HandoffOrder>(1, nullptr));
            handoffs.Add(std::make_shared<HandoffOrder>(2, nullptr));
        }

        testing::NiceMock<Euroscope::MockEuroScopeCFlightPlanInterface> mockFlightplan;
        AirfieldCollection airfields;
        HandoffCollection handoffs;
        FlightplanAirfieldHandoffMapper mapper;
    };

    TEST_F(FlightplanAirfieldHandoffMapperTest, ItReturnsNullptrIfAirfieldNotFound)
    {
        ON_CALL(mockFlightplan, GetOrigin).WillByDefault(testing::Return("EGCC"));
        EXPECT_EQ(nullptr, mapper.MapForFlightplan(mockFlightplan));
    }

    TEST_F(FlightplanAirfieldHandoffMapperTest, ItReturnsNullptrIfAirfieldDoesntHaveHandoff)
    {
        ON_CALL(mockFlightplan, GetOrigin).WillByDefault(testing::Return("EGKK"));
        EXPECT_EQ(nullptr, mapper.MapForFlightplan(mockFlightplan));
    }

    TEST_F(FlightplanAirfieldHandoffMapperTest, ItReturnsNullptrIfHandoffDoesntExist)
    {
        ON_CALL(mockFlightplan, GetOrigin).WillByDefault(testing::Return("EGBB"));
        EXPECT_EQ(nullptr, mapper.MapForFlightplan(mockFlightplan));
    }

    TEST_F(FlightplanAirfieldHandoffMapperTest, ItReturnsMappedHandoff)
    {
        ON_CALL(mockFlightplan, GetOrigin).WillByDefault(testing::Return("EGLL"));
        EXPECT_EQ(handoffs.Get(1), mapper.MapForFlightplan(mockFlightplan));
    }
} // namespace UKControllerPluginTest::Handoff
