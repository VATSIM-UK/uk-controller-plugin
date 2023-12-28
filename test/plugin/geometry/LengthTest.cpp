#include "geometry/Length.h"

namespace UKControllerPluginTest::Approach {
    class LengthTest : public ::testing::Test
    {
    };

    TEST_F(LengthTest, ItConvertsNauticalMilesToFeet)
    {
        EXPECT_DOUBLE_EQ(6076.115, UKControllerPlugin::Geometry::NauticalMilesToFeet(1));
    }

    TEST_F(LengthTest, ItConvertsFeetToNauticalMiles)
    {
        EXPECT_DOUBLE_EQ(1, UKControllerPlugin::Geometry::FeetToNauticalMiles(6076.115));
    }
} // namespace UKControllerPluginTest::Approach
