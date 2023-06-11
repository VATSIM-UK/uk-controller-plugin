#include "datablock/DatablockFunctions.h"

using UKControllerPlugin::Datablock::ConvertAltitudeToFlightLevel;
using UKControllerPlugin::Datablock::ConvertFlightLevelToAltitude;
using UKControllerPlugin::Datablock::NormaliseFlightLevelFromString;

namespace UKControllerPluginTest::Datablock {

    TEST(DatablockFunctions, AltitudesCanBeConvertedToFlightLevels)
    {
        EXPECT_EQ(350, ConvertAltitudeToFlightLevel(35000));
    }

    TEST(DatablockFunctions, FlightLevelsCanBeConvertedToAltitudes)
    {
        EXPECT_EQ(35000, ConvertFlightLevelToAltitude(350));
    }

    TEST(DatablockFunctions, ItConvertsStandardThreeDigitFlightLevelStringToInteger)
    {
        EXPECT_EQ(350, NormaliseFlightLevelFromString("350"));
    }

    TEST(DatablockFunctions, ItConvertsStandardThreeDigitFlightLevelStringToIntegerWithPrefix)
    {
        EXPECT_EQ(350, NormaliseFlightLevelFromString("FL350"));
    }

    TEST(DatablockFunctions, ItConvertsStandardTwoDigitFlightLevelStringToInteger)
    {
        EXPECT_EQ(90, NormaliseFlightLevelFromString("90"));
    }

    TEST(DatablockFunctions, ItConvertsStandardTwoDigitFlightLevelStringToIntegerWithPrefix)
    {
        EXPECT_EQ(90, NormaliseFlightLevelFromString("FL90"));
    }

    TEST(DatablockFunctions, PrefixIsCaseInsensitiveAllLower)
    {
        EXPECT_EQ(90, NormaliseFlightLevelFromString("fl90"));
    }

    TEST(DatablockFunctions, PrefixIsCaseInsensitiveFirstLower)
    {
        EXPECT_EQ(90, NormaliseFlightLevelFromString("fL90"));
    }

    TEST(DatablockFunctions, PrefixIsCaseInsensitiveSecondLower)
    {
        EXPECT_EQ(90, NormaliseFlightLevelFromString("Fl90"));
    }

    TEST(DatablockFunctions, ItConvertsStandardFourDigitAltitudeStringToInteger)
    {
        EXPECT_EQ(90, NormaliseFlightLevelFromString("9000"));
    }

    TEST(DatablockFunctions, ItConvertsStandardFiveDigitAltitudeStringToInteger)
    {
        EXPECT_EQ(350, NormaliseFlightLevelFromString("35000"));
    }

    TEST(DatablockFunctions, ItConvertsDoublesToFrequencyStrings)
    {
        EXPECT_EQ("123.456", UKControllerPlugin::Datablock::FrequencyStringFromDouble(123.456));
    }

    TEST(DatablockFunctions, ItHandlesFloatingPointErrorsConvertingDoublesToFrequencyStrings)
    {
        EXPECT_EQ("123.456", UKControllerPlugin::Datablock::FrequencyStringFromDouble(123.45600000000001));
    }
} // namespace UKControllerPluginTest::Datablock
