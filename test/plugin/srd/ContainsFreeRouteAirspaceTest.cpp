#include "srd/ContainsFreeRouteAirspace.h"

using UKControllerPlugin::Srd::RouteContainsFreeRouteAirspace;

namespace UKControllerPluginTest::Srd {
    class ContainsFreeRouteAirspaceTest : public testing::Test
    {
    };

    TEST_F(ContainsFreeRouteAirspaceTest, ReturnsTrueIfRouteStringHasFreeRouteAirspace)
    {
        EXPECT_TRUE(RouteContainsFreeRouteAirspace("ADN <FRA> LAKEY DCT BNN"));
    }

    TEST_F(ContainsFreeRouteAirspaceTest, ReturnsFalseIfRouteStringDoesNotHaveFreeRouteAirspace)
    {
        EXPECT_FALSE(RouteContainsFreeRouteAirspace("ADN DCT LAKEY DCT BNN"));
    }

    TEST_F(ContainsFreeRouteAirspaceTest, IgnoresNavaidsWithSameName)
    {
        EXPECT_FALSE(RouteContainsFreeRouteAirspace("ADN DCT FRA DCT LAKEY DCT BNN"));
    }
} // namespace UKControllerPluginTest::Srd
