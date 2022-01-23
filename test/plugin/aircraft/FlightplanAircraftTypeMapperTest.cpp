#include "aircraft/AircraftType.h"
#include "aircraft/AircraftTypeCollection.h"
#include "aircraft/FlightplanAircraftTypeMapper.h"

using UKControllerPlugin::Aircraft::AircraftType;
using UKControllerPlugin::Aircraft::AircraftTypeCollection;
using UKControllerPlugin::Aircraft::FlightplanAircraftTypeMapper;

namespace UKControllerPluginTest::Aircraft {
    class FlightplanAircraftTypeMapperTest : public testing::Test
    {
        public:
        FlightplanAircraftTypeMapperTest() : mapper(collection)
        {
            collection.Add(std::make_shared<AircraftType>(1, "B738", std::set<int>{3, 5}));
            collection.Add(std::make_shared<AircraftType>(2, "A320", std::set<int>{3, 5}));
        }

        testing::NiceMock<Euroscope::MockEuroScopeCFlightPlanInterface> flightplan;
        AircraftTypeCollection collection;
        FlightplanAircraftTypeMapper mapper;
    };

    TEST_F(FlightplanAircraftTypeMapperTest, ItReturnsMappedType)
    {
        ON_CALL(flightplan, GetAircraftType).WillByDefault(testing::Return("B738"));

        EXPECT_EQ(collection.GetByIcaoCode("B738"), mapper.MapForFlightplan(flightplan));
    }

    TEST_F(FlightplanAircraftTypeMapperTest, ItReturnsNullptrNoMappedType)
    {
        ON_CALL(flightplan, GetAircraftType).WillByDefault(testing::Return("B739"));

        EXPECT_EQ(nullptr, mapper.MapForFlightplan(flightplan));
    }
} // namespace UKControllerPluginTest::Aircraft
