#include "historytrail/HistoryTrailRepository.h"
#include "historytrail/AircraftHistoryTrail.h"

using UKControllerPlugin::HistoryTrail::HistoryTrailRepository;
using UKControllerPlugin::HistoryTrail::AircraftHistoryTrail;
using ::testing::Test;

namespace UKControllerPluginTest {
namespace HistoryTrail {

    class HistoryTrailRepositoryTest : public Test
    {
        public:
            HistoryTrailRepository repository;
    };

    TEST_F(HistoryTrailRepositoryTest, ItDoesntHaveAircraftItDoesntHave)
    {
        EXPECT_FALSE(repository.HasAircraft("test"));
    }

    TEST_F(HistoryTrailRepositoryTest, ItCanRegisterAnAircraft)
    {
        repository.RegisterAircraft(std::make_shared<AircraftHistoryTrail>("test"));
        EXPECT_TRUE(repository.HasAircraft("test"));
    }

    TEST_F(HistoryTrailRepositoryTest, ItCanUnregisterAnAircraft)
    {
        repository.RegisterAircraft(std::make_shared<AircraftHistoryTrail>("test"));
        repository.UnregisterAircraft("test");
        EXPECT_FALSE(repository.HasAircraft("test"));
    }

    TEST_F(HistoryTrailRepositoryTest, ItCanReturnAircraft)
    {
        std::shared_ptr<AircraftHistoryTrail> trail = std::make_shared<AircraftHistoryTrail>("test");
        repository.RegisterAircraft(trail);
        EXPECT_EQ(trail, repository.GetAircraft("test"));
    }
}  // namespace HistoryTrail
}  // namespace UKControllerPluginTest
