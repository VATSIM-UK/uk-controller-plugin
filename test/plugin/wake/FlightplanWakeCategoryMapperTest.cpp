#include "aircraft/AircraftType.h"
#include "wake/FlightplanWakeCategoryMapper.h"
#include "wake/WakeScheme.h"
#include "wake/WakeCategory.h"

using UKControllerPlugin::Aircraft::AircraftType;
using UKControllerPlugin::Wake::ArrivalWakeInterval;
using UKControllerPlugin::Wake::DepartureWakeInterval;
using UKControllerPlugin::Wake::FlightplanWakeCategoryMapper;
using UKControllerPlugin::Wake::WakeCategory;
using UKControllerPlugin::Wake::WakeScheme;

namespace UKControllerPluginTest::Wake {
    class FlightplanWakeCategoryMapperTest : public testing::Test
    {
        public:
        FlightplanWakeCategoryMapperTest()
            : category(std::make_shared<WakeCategory>(
                  1,
                  "LM",
                  "Lower Medium",
                  15,
                  std::list<std::shared_ptr<DepartureWakeInterval>>{},
                  std::list<std::shared_ptr<ArrivalWakeInterval>>{})),
              wakeScheme(1, "TEST", "TEST", std::list<std::shared_ptr<WakeCategory>>({category})),
              mapper(wakeScheme, aircraftMapper)
        {
            ON_CALL(mockFlightplan, GetAircraftType).WillByDefault(testing::Return("B738"));
        }

        testing::NiceMock<Euroscope::MockEuroScopeCFlightPlanInterface> mockFlightplan;
        std::shared_ptr<WakeCategory> category;
        WakeScheme wakeScheme;
        testing::NiceMock<Aircraft::MockAircraftTypeMapper> aircraftMapper;
        FlightplanWakeCategoryMapper mapper;
    };

    TEST_F(FlightplanWakeCategoryMapperTest, ItReturnsNullptrNoAircraftTypeFound)
    {
        ON_CALL(aircraftMapper, MapForFlightplan(testing::Ref(mockFlightplan))).WillByDefault(testing::Return(nullptr));

        EXPECT_EQ(nullptr, mapper.MapForFlightplan(mockFlightplan));
    }

    TEST_F(FlightplanWakeCategoryMapperTest, ItReturnsNullptrNoMatchingWakeCategoryFound)
    {
        const auto aircraftType = std::make_shared<AircraftType>(1, "B738", std::set<int>{3, 4});
        ON_CALL(aircraftMapper, MapForFlightplan(testing::Ref(mockFlightplan)))
            .WillByDefault(testing::Return(aircraftType));

        EXPECT_EQ(nullptr, mapper.MapForFlightplan(mockFlightplan));
    }

    TEST_F(FlightplanWakeCategoryMapperTest, ItReturnsMappedCategory)
    {
        const auto aircraftType = std::make_shared<AircraftType>(1, "B738", std::set<int>{1, 2});
        ON_CALL(aircraftMapper, MapForFlightplan(testing::Ref(mockFlightplan)))
            .WillByDefault(testing::Return(aircraftType));

        EXPECT_EQ(category, mapper.MapForFlightplan(mockFlightplan));
    }

    TEST_F(FlightplanWakeCategoryMapperTest, ItReturnsValueViaCache)
    {
        const auto aircraftType = std::make_shared<AircraftType>(1, "B738", std::set<int>{1, 2});
        ON_CALL(aircraftMapper, MapForFlightplan(testing::Ref(mockFlightplan)))
            .WillByDefault(testing::Return(aircraftType));

        EXPECT_EQ(category, mapper.MapForFlightplan(mockFlightplan));
        EXPECT_EQ(category, mapper.MapForFlightplan(mockFlightplan));
        EXPECT_EQ(category, mapper.MapForFlightplan(mockFlightplan));
    }
} // namespace UKControllerPluginTest::Wake
