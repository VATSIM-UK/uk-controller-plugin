#include "aircraft/AircraftType.h"

using UKControllerPlugin::Aircraft::AircraftType;

namespace UKControllerPluginTest::Aircraft {
    class AircraftTypeTest : public testing::Test
    {
        public:
        AircraftTypeTest() : type(1, "B738", {1, 2})
        {
        }

        AircraftType type;
    };

    TEST_F(AircraftTypeTest, ItHasAnId)
    {
        EXPECT_EQ(1, type.Id());
    }

    TEST_F(AircraftTypeTest, ItHasAnIcaoCode)
    {
        EXPECT_EQ("B738", type.IcaoCode());
    }

    TEST_F(AircraftTypeTest, ItHasWakeCategories)
    {
        EXPECT_EQ(std::set<int>({1, 2}), type.WakeCategories());
    }
} // namespace UKControllerPluginTest::Aircraft
