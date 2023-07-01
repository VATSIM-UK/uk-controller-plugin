#include "historytrail/AircraftHistoryTrail.h"
#include "historytrail/HistoryTrailRepository.h"

using ::testing::Test;
using UKControllerPlugin::HistoryTrail::AircraftHistoryTrail;
using UKControllerPlugin::HistoryTrail::HistoryTrailRepository;

namespace UKControllerPluginTest::HistoryTrail {
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
        EXPECT_EQ(1, repository.trailData.size());
    }

    TEST_F(HistoryTrailRepositoryTest, ItCanUnregisterAnAircraft)
    {
        repository.RegisterAircraft(std::make_shared<AircraftHistoryTrail>("test"));
        repository.UnregisterAircraft("test");
        EXPECT_FALSE(repository.HasAircraft("test"));
        EXPECT_EQ(0, repository.trailData.size());
    }

    TEST_F(HistoryTrailRepositoryTest, ItCanReturnAircraft)
    {
        std::shared_ptr<AircraftHistoryTrail> trail = std::make_shared<AircraftHistoryTrail>("test");
        repository.RegisterAircraft(trail);
        EXPECT_EQ(trail, repository.GetAircraft("test"));
    }
} // namespace UKControllerPluginTest::HistoryTrail
