#include "metar/PressureComponent.h"

using UKControllerPlugin::Metar::PressureComponent;
using UKControllerPlugin::Metar::PressureUnit;

namespace UKControllerPluginTest::Metar {
    class PressureComponentTest : public testing::Test
    {
        public:
        PressureComponentTest()
            : hectopascalComponent(1013, 1001, 29.92, 28.24, PressureUnit::Hectopascals),
              inHgComponent(1013, 1001, 29.92, 28.24, PressureUnit::InHg)
        {
        }

        PressureComponent hectopascalComponent;
        PressureComponent inHgComponent;
    };

    TEST_F(PressureComponentTest, ItHasAHectopascalQnhValue)
    {
        EXPECT_EQ(1013, hectopascalComponent.QnhHectopascals());
    }

    TEST_F(PressureComponentTest, ItHasAHectopascalQfeValue)
    {
        EXPECT_EQ(1001, hectopascalComponent.QfeHectopascals());
    }

    TEST_F(PressureComponentTest, ItHasAnInHgQnhValue)
    {
        EXPECT_FLOAT_EQ(29.92, hectopascalComponent.QnhInHg());
    }

    TEST_F(PressureComponentTest, ItHasAnInHgQfeValue)
    {
        EXPECT_FLOAT_EQ(28.24, hectopascalComponent.QfeInHg());
    }

    TEST_F(PressureComponentTest, ItIsReportedInHectopascals)
    {
        EXPECT_EQ(PressureUnit::Hectopascals, hectopascalComponent.ReportedIn());
        EXPECT_TRUE(hectopascalComponent.ReportedAsHectopascals());
        EXPECT_FALSE(hectopascalComponent.ReportedAsInHg());
    }

    TEST_F(PressureComponentTest, ItIsReportedInInHg)
    {
        EXPECT_EQ(PressureUnit::InHg, inHgComponent.ReportedIn());
        EXPECT_FALSE(inHgComponent.ReportedAsHectopascals());
        EXPECT_TRUE(inHgComponent.ReportedAsInHg());
    }
} // namespace UKControllerPluginTest::Metar
