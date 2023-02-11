#include "geometry/MeasurementUnit.h"
#include "geometry/MeasurementUnitType.h"
#include "geometry/MeasurementUnitFactory.h"

using UKControllerPlugin::Geometry::MeasurementUnitType;
using UKControllerPlugin::Geometry::UnitFromString;
using UKControllerPlugin::Geometry::UnitStringValid;

namespace UKControllerPluginTest::Geometry {
    class MeasurementUnitFactoryTest : public testing::Test
    {
    };

    TEST_F(MeasurementUnitFactoryTest, ItReturnsSecondsFromString)
    {
        const auto unit = UnitFromString("s");
        EXPECT_EQ(*unit, MeasurementUnitType::Seconds);
        EXPECT_EQ(unit->description, "Seconds");
    }

    TEST_F(MeasurementUnitFactoryTest, ItReturnsMinutesFromString)
    {
        const auto unit = UnitFromString("min");
        EXPECT_EQ(*unit, MeasurementUnitType::Minutes);
        EXPECT_EQ(unit->description, "Minutes");
    }

    TEST_F(MeasurementUnitFactoryTest, ItReturnsNauticalMilesFromString)
    {
        const auto unit = UnitFromString("nm");
        EXPECT_EQ(*unit, MeasurementUnitType::NauticalMiles);
        EXPECT_EQ(unit->description, "NM");
    }

    TEST_F(MeasurementUnitFactoryTest, ItReturnsUnknownUnitFromString)
    {
        const auto unit = UnitFromString("abc");
        EXPECT_EQ(*unit, MeasurementUnitType::None);
        EXPECT_EQ(unit->description, "??");
    }

    TEST_F(MeasurementUnitFactoryTest, StringIsValidSeconds)
    {
        EXPECT_TRUE(UnitStringValid("s"));
    }

    TEST_F(MeasurementUnitFactoryTest, StringIsValidMinutes)
    {
        EXPECT_TRUE(UnitStringValid("min"));
    }

    TEST_F(MeasurementUnitFactoryTest, StringIsValidNauticalMiles)
    {
        EXPECT_TRUE(UnitStringValid("nm"));
    }

    TEST_F(MeasurementUnitFactoryTest, StringIsNotValidUnknownString)
    {
        EXPECT_FALSE(UnitStringValid("abc"));
    }
} // namespace UKControllerPluginTest::Geometry
