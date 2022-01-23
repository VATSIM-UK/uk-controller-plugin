#include "airfield/AirfieldCollection.h"
#include "airfield/AirfieldModel.h"
#include "controller/ControllerPositionHierarchy.h"
#include "runway/Runway.h"
#include "runway/RunwayCollection.h"
#include "sid/FlightplanSidMapper.h"
#include "sid/SidCollection.h"
#include "sid/StandardInstrumentDeparture.h"

using ::testing::Test;
using UKControllerPlugin::Airfield::AirfieldCollection;
using UKControllerPlugin::Airfield::AirfieldModel;
using UKControllerPlugin::Prenote::PairedAirfieldPrenote;
using UKControllerPlugin::Runway::Runway;
using UKControllerPlugin::Runway::RunwayCollection;
using UKControllerPlugin::Sid::FlightplanSidMapper;
using UKControllerPlugin::Sid::SidCollection;
using UKControllerPlugin::Sid::StandardInstrumentDeparture;

namespace UKControllerPluginTest::Sid {

    class FlightplanSidMapperTest : public Test
    {
        public:
        FlightplanSidMapperTest() : mapper(airfields, runways, sids)
        {
            this->airfields.AddAirfield(std::make_shared<AirfieldModel>(
                2, "EGLL", nullptr, std::vector<std::shared_ptr<PairedAirfieldPrenote>>{}, 1));
            EuroScopePlugIn::CPosition threshold;
            threshold.m_Latitude = 1;
            threshold.m_Latitude = 2;
            this->runways.Add(std::make_shared<class Runway>(2, 2, "27L", 123, threshold));
            this->sids.AddSid(std::make_shared<StandardInstrumentDeparture>(1, 1, "TEST1A", 1, 2, 3));
            this->sids.AddSid(std::make_shared<StandardInstrumentDeparture>(2, 2, "TEST1B", 3, 4, 5));

            ON_CALL(mockFlightplan, GetOrigin()).WillByDefault(testing::Return("EGLL"));

            ON_CALL(mockFlightplan, GetDepartureRunway()).WillByDefault(testing::Return("27L"));

            ON_CALL(mockFlightplan, GetSidName()).WillByDefault(testing::Return("TEST1B"));
        }

        testing::NiceMock<Euroscope::MockEuroScopeCFlightPlanInterface> mockFlightplan;
        AirfieldCollection airfields;
        RunwayCollection runways;
        SidCollection sids;
        FlightplanSidMapper mapper;
    };

    TEST_F(FlightplanSidMapperTest, ItMapsFlightplansToSids)
    {
        EXPECT_EQ(this->sids.GetById(2), this->mapper.MapFlightplanToSid(mockFlightplan));
    }

    TEST_F(FlightplanSidMapperTest, ItReturnsNullptrIfSidNotFound)
    {
        ON_CALL(mockFlightplan, GetSidName()).WillByDefault(testing::Return("TEST1C"));
        EXPECT_EQ(nullptr, this->mapper.MapFlightplanToSid(mockFlightplan));
    }

    TEST_F(FlightplanSidMapperTest, ItReturnsNullptrIfRunwayNotFound)
    {
        ON_CALL(mockFlightplan, GetDepartureRunway()).WillByDefault(testing::Return("27R"));
        EXPECT_EQ(nullptr, this->mapper.MapFlightplanToSid(mockFlightplan));
    }

    TEST_F(FlightplanSidMapperTest, ItReturnsNullptrIfAirfieldNotFound)
    {
        ON_CALL(mockFlightplan, GetOrigin()).WillByDefault(testing::Return("EGKK"));
        EXPECT_EQ(nullptr, this->mapper.MapFlightplanToSid(mockFlightplan));
    }
} // namespace UKControllerPluginTest::Sid
