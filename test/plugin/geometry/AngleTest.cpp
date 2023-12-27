#include "geometry/Angle.h"

namespace UKControllerPluginTest::Geometry {
    class AngleTest : public ::testing::Test
    {
        public:
        const double pi = 3.14159265358979323846;
    };

    TEST_F(AngleTest, DegreesToRadians)
    {
        EXPECT_DOUBLE_EQ(UKControllerPlugin::Geometry::DegreesToRadians(0), 0);
        EXPECT_DOUBLE_EQ(UKControllerPlugin::Geometry::DegreesToRadians(90), pi / 2);
        EXPECT_DOUBLE_EQ(UKControllerPlugin::Geometry::DegreesToRadians(180), pi);
        EXPECT_DOUBLE_EQ(UKControllerPlugin::Geometry::DegreesToRadians(270), pi * 1.5);
        EXPECT_DOUBLE_EQ(UKControllerPlugin::Geometry::DegreesToRadians(360), pi * 2);
    }

    TEST_F(AngleTest, RadiansToDegrees)
    {
        EXPECT_EQ(UKControllerPlugin::Geometry::RadiansToDegrees(0), 0);
        EXPECT_EQ(UKControllerPlugin::Geometry::RadiansToDegrees(pi / 2), 90);
        EXPECT_EQ(UKControllerPlugin::Geometry::RadiansToDegrees(pi), 180);
        EXPECT_EQ(UKControllerPlugin::Geometry::RadiansToDegrees(pi * 1.5), 270);
        EXPECT_EQ(UKControllerPlugin::Geometry::RadiansToDegrees(pi * 2), 360);
    }

    TEST_F(AngleTest, Slope)
    {
        EXPECT_DOUBLE_EQ(UKControllerPlugin::Geometry::Slope(pi / 2), 1.633123935319537e+16);
    }
} // namespace UKControllerPluginTest::Geometry
