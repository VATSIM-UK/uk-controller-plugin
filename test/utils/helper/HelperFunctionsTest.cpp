#include "helper/HelperFunctions.h"

using UKControllerPlugin::HelperFunctions;

namespace UKControllerPluginUtilsTest {

    TEST(HelperFunctions, IsAnIntegerReturnsTrueIfInteger)
    {
        std::string qnh = "1021";
        EXPECT_TRUE(HelperFunctions::IsAnInteger(qnh));
    }

    TEST(HelperFunctions, IsAnIntegerReturnsTrueIfInteger2)
    {
        std::string qnh = "986";
        EXPECT_TRUE(HelperFunctions::IsAnInteger(qnh));
    }

    TEST(HelperFunctions, IsAnIntegerReturnsTrueIfInteger3)
    {
        std::string qnh = "0";
        EXPECT_TRUE(HelperFunctions::IsAnInteger(qnh));
    }

    TEST(HelperFunctions, IsAnIntegerReturnsTrueIfInteger4)
    {
        std::string qnh = "1000";
        EXPECT_TRUE(HelperFunctions::IsAnInteger(qnh));
    }

    TEST(HelperFunctions, IsAnIntegerReturnsFalseIfFloat)
    {
        std::string qnh = "1021.2";
        EXPECT_FALSE(HelperFunctions::IsAnInteger(qnh));
    }

    TEST(HelperFunctions, IsAnIntegerReturnsFalseIfInvalidNumber)
    {
        std::string qnh = "1021.";
        EXPECT_FALSE(HelperFunctions::IsAnInteger(qnh));
    }
    TEST(HelperFunctions, IsAnIntegerReturnsFalseIfNotANumber)
    {
        std::string qnh = "Cheese";
        EXPECT_FALSE(HelperFunctions::IsAnInteger(qnh));
    }

    TEST(HelperFunctions, IsValidSettingsColourReturnsFalseIfOnlyOneItem)
    {
        EXPECT_FALSE(HelperFunctions::IsValidSettingsColour("123"));
    }

    TEST(HelperFunctions, IsValidSettingsColourReturnsFalseIfTwoItems)
    {
        EXPECT_FALSE(HelperFunctions::IsValidSettingsColour("123,123"));
    }

    TEST(HelperFunctions, IsValidSettingsColourReturnsFalseIfMoreThanThreeItems)
    {
        EXPECT_FALSE(HelperFunctions::IsValidSettingsColour("123,123,123,123"));
    }

    TEST(HelperFunctions, IsValidSettingsColourReturnsFalseItemOneGreaterThanMax)
    {
        EXPECT_FALSE(HelperFunctions::IsValidSettingsColour("256,123,123"));
    }

    TEST(HelperFunctions, IsValidSettingsColourReturnsFalseItemTwoGreaterThanMax)
    {
        EXPECT_FALSE(HelperFunctions::IsValidSettingsColour("255,257,123"));
    }

    TEST(HelperFunctions, IsValidSettingsColourReturnsFalseItemThreeGreaterThanMax)
    {
        EXPECT_FALSE(HelperFunctions::IsValidSettingsColour("255,254,300"));
    }

    TEST(HelperFunctions, IsValidSettingsColourReturnsFalseItemOneInvalid)
    {
        EXPECT_FALSE(HelperFunctions::IsValidSettingsColour("-1,123,123"));
    }

    TEST(HelperFunctions, IsValidSettingsColourReturnsFalseItemTwoInvalid)
    {
        EXPECT_FALSE(HelperFunctions::IsValidSettingsColour("123,ABC,123"));
    }

    TEST(HelperFunctions, IsValidSettingsColourReturnsFalseItemThreeInvalid)
    {
        EXPECT_FALSE(HelperFunctions::IsValidSettingsColour("123,123,ABC"));
    }

    TEST(HelperFunctions, IsValidSettingsColourReturnsTrueIfValid)
    {
        EXPECT_TRUE(HelperFunctions::IsValidSettingsColour("231,123,212"));
    }

    TEST(HelperFunctions, GetColourFromSettingStringReturnsWhiteIfInvalid)
    {
        COLORREF color = HelperFunctions::GetColourFromSettingString("123,213,ABC");
        EXPECT_EQ(GetRValue(color), 255);
        EXPECT_EQ(GetGValue(color), 255);
        EXPECT_EQ(GetBValue(color), 255);
    }

    TEST(HelperFunctions, GetColourFromSettingStringReturnsColorIfValid)
    {
        COLORREF color = HelperFunctions::GetColourFromSettingString("123,213,221");
        EXPECT_EQ(GetRValue(color), 123);
        EXPECT_EQ(GetGValue(color), 213);
        EXPECT_EQ(GetBValue(color), 221);
    }

    TEST(HelperFunctions, TokeniseStringSplitsOnDelimiter)
    {
        std::vector<std::string> tokens = HelperFunctions::TokeniseString('_', "LON_S_CTR");
        EXPECT_EQ(3, tokens.size());
        EXPECT_EQ(0, tokens[0].compare("LON"));
        EXPECT_EQ(0, tokens[1].compare("S"));
        EXPECT_EQ(0, tokens[2].compare("CTR"));
    }

    TEST(HelperFunctions, TokeniseReturnsSingleStringIfNoDelimiter)
    {
        std::vector<std::string> tokens = HelperFunctions::TokeniseString('-', "LON_S_CTR");
        EXPECT_EQ(1, tokens.size());
        EXPECT_EQ(0, tokens[0].compare("LON_S_CTR"));
    }

    TEST(HelperFunctions, TokeniseIgnoresEmptyStringsBetweenTokens)
    {
        std::vector<std::string> tokens = HelperFunctions::TokeniseString('_', "LON_S__CTR");
        EXPECT_EQ(3, tokens.size());
        EXPECT_EQ(0, tokens[0].compare("LON"));
        EXPECT_EQ(0, tokens[1].compare("S"));
        EXPECT_EQ(0, tokens[2].compare("CTR"));
    }

    TEST(HelperFunctions, StripJsonQuotesFromStringRemovesQuotes)
    {
        EXPECT_TRUE("Test String" == HelperFunctions::StripQuotesFromJsonString("\"Test String\""));
    }

    TEST(HelperFunctions, StripJsonQuotesFromStringDoesNotRemoveDoubleQuotesS)
    {
        EXPECT_TRUE("\"Test String\"" == HelperFunctions::StripQuotesFromJsonString("\"\"Test String\"\""));
    }

    TEST(HelperFunctions, StripJsonQuotesFromStringDoesNotRemoveQuotesInMiddle)
    {
        EXPECT_TRUE("Test\"String" == HelperFunctions::StripQuotesFromJsonString("Test\"String"));
    }

    TEST(HelperFunctions, StripJsonQuotesFromStringCanHandleEmptyString)
    {
        EXPECT_TRUE("" == HelperFunctions::StripQuotesFromJsonString(""));
    }

    TEST(HelperFunctions, StripJsonQuotesFromStringCanHandleStringsWithSingleQuotes)
    {
        EXPECT_TRUE("" == HelperFunctions::StripQuotesFromJsonString("\""));
    }

    TEST(HelperFunctions, GetColourStringReturnsCorrectString)
    {
        EXPECT_TRUE("12,34,123" == HelperFunctions::GetColourString(RGB(12, 34, 123)));
    }

    TEST(HelperFunctions, IsBooleanReturnsTrueForZero)
    {
        EXPECT_TRUE(HelperFunctions::IsBoolean(std::string("0")));
    }

    TEST(HelperFunctions, IsBooleanReturnsTrueForOne)
    {
        EXPECT_TRUE(HelperFunctions::IsBoolean(std::string("1")));
    }

    TEST(HelperFunctions, IsBooleanReturnsFalseForNonBoolean)
    {
        EXPECT_FALSE(HelperFunctions::IsBoolean(std::string("2")));
    }

    TEST(HelperFunctions, IsBooleanReturnsFalseForFloatBoolean)
    {
        EXPECT_FALSE(HelperFunctions::IsBoolean(std::string("1.0")));
    }

    TEST(HelperFunctions, IsFloatReturnsTrueForFloat)
    {
        EXPECT_TRUE(HelperFunctions::IsFloat(std::string("1.2")));
    }

    TEST(HelperFunctions, IsFloatReturnsFalseForDoubleDecimal)
    {
        EXPECT_FALSE(HelperFunctions::IsFloat(std::string("1.23.22")));
    }

    TEST(HelperFunctions, IsFloatReturnsFalseForNonNumber)
    {
        EXPECT_FALSE(HelperFunctions::IsFloat(std::string("1.23a")));
    }

    TEST(HelperFunctions, IsFloatReturnsTrueForInteger)
    {
        EXPECT_TRUE(HelperFunctions::IsFloat(std::string("123")));
    }

    TEST(HelperFunctions, GetTimeFromNumberStringReturnsCorrectTimePoint)
    {
        time_t time = std::chrono::system_clock::to_time_t(HelperFunctions::GetTimeFromNumberString("1526"));
        std::tm tmObject;
        gmtime_s(&tmObject, &time);

        EXPECT_EQ(15, tmObject.tm_hour);
        EXPECT_EQ(26, tmObject.tm_min);
    }

    TEST(HelperFunctions, GetTimeFromNumberStringHandlesZuluTimes)
    {
        time_t time = std::chrono::system_clock::to_time_t(HelperFunctions::GetTimeFromNumberString("1526z"));
        std::tm tmObject;
        gmtime_s(&tmObject, &time);

        EXPECT_EQ(15, tmObject.tm_hour);
        EXPECT_EQ(26, tmObject.tm_min);
    }

    TEST(HelperFunctions, GetTimeFromNumberStringHandlesMorningTimes)
    {
        time_t time = std::chrono::system_clock::to_time_t(HelperFunctions::GetTimeFromNumberString("0235"));
        std::tm tmObject;
        gmtime_s(&tmObject, &time);

        EXPECT_EQ(2, tmObject.tm_hour);
        EXPECT_EQ(35, tmObject.tm_min);
    }

    TEST(HelperFunctions, GetTimeFromNumberStringHandlesThreeDigitTimes)
    {
        time_t time = std::chrono::system_clock::to_time_t(HelperFunctions::GetTimeFromNumberString("235"));
        std::tm tmObject;
        gmtime_s(&tmObject, &time);

        EXPECT_EQ(2, tmObject.tm_hour);
        EXPECT_EQ(35, tmObject.tm_min);
    }

    TEST(HelperFunctions, GetTimeFromNumberStringHandlesTwoDigitTimes)
    {
        time_t time = std::chrono::system_clock::to_time_t(HelperFunctions::GetTimeFromNumberString("23"));
        std::tm tmObject;
        gmtime_s(&tmObject, &time);

        EXPECT_EQ(0, tmObject.tm_hour);
        EXPECT_EQ(23, tmObject.tm_min);
    }

    TEST(HelperFunctions, GetTimeFromNumberStringHandlesOneDigitTimes)
    {
        time_t time = std::chrono::system_clock::to_time_t(HelperFunctions::GetTimeFromNumberString("4"));
        std::tm tmObject;
        gmtime_s(&tmObject, &time);

        EXPECT_EQ(0, tmObject.tm_hour);
        EXPECT_EQ(4, tmObject.tm_min);
    }

    TEST(HelperFunctions, GetTimeFromNumberStringHandlesNonTimes)
    {
        EXPECT_EQ(
            HelperFunctions::GetTimeFromNumberString("152B"),
            (std::chrono::system_clock::time_point::max)()
        );
    }

    TEST(HelperFunctions, GetTimeFromNumberStringHandlesInvalidTimes)
    {
        EXPECT_EQ(
            HelperFunctions::GetTimeFromNumberString("2503"),
            (std::chrono::system_clock::time_point::max)()
        );
    }

    TEST(HelperFunctions, GetTimeFromNumberStringHandlesEmptyTimes)
    {
        EXPECT_EQ(
            HelperFunctions::GetTimeFromNumberString(""),
            (std::chrono::system_clock::time_point::max)()
        );
    }

    TEST(HelperFunctions, ItConvertsRegularStringToWide)
    {
        EXPECT_EQ(
            HelperFunctions::ConvertToWideString("somestring"),
            std::wstring(L"somestring")
        );
    }

    TEST(HelperFunctions, ItConvertsWideStringToRegular)
    {
        EXPECT_EQ(
            HelperFunctions::ConvertToRegularString(L"somestring"),
            std::string("somestring")
        );
    }

    TEST(HelperFunctions, VectorToDelimitedStringCreatesString)
    {
        std::vector<std::string> values = { "moo", "cluck", "bell" };
        EXPECT_TRUE("moo;cluck;bell" == HelperFunctions::VectorToDelimetedString(values, ";"));
    }
}  // namespace UKControllerPluginUtilsTest
