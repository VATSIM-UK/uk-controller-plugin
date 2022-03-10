#include "time/ParseTimeStrings.h"

using ::testing::Test;
using UKControllerPlugin::Time::invalidTime;
using UKControllerPlugin::Time::ParseIsoZuluString;
using UKControllerPlugin::Time::ParseTimeString;
using UKControllerPlugin::Time::ToDateTimeString;

namespace UKControllerPluginTest::Time {

    class ParseTimeStringsTest : public Test
    {
        public:
        static std::chrono::system_clock::time_point
        GetFromTimeNumbers(int year, int month, int day, int hours, int minutes, int seconds)
        {
            tm timeInfo;
            timeInfo.tm_year = year - 1900; // Years since 1900
            timeInfo.tm_mon = month - 1;    // Months since January (0-11)
            timeInfo.tm_mday = day;
            timeInfo.tm_hour = hours;
            timeInfo.tm_min = minutes;
            timeInfo.tm_sec = seconds;
            timeInfo.tm_isdst = 0;
            return std::chrono::system_clock::from_time_t(mktime(&timeInfo));
        }
    };

    TEST_F(ParseTimeStringsTest, ItReturnsParsesCorrectTimeSingleDigits)
    {
        EXPECT_EQ(this->GetFromTimeNumbers(2021, 1, 9, 1, 2, 3), ParseTimeString("2021-01-09 01:02:03"));
    }

    TEST_F(ParseTimeStringsTest, ItReturnsParsesCorrectTimeDoubleDigits)
    {
        EXPECT_EQ(this->GetFromTimeNumbers(2021, 11, 12, 13, 14, 15), ParseTimeString("2021-11-12 13:14:15"));
    }

    TEST_F(ParseTimeStringsTest, ItReturnsInvalidTimeOnNotFormat)
    {
        EXPECT_EQ(invalidTime, ParseTimeString("abc"));
    }

    TEST_F(ParseTimeStringsTest, ItReturnsInvalidTimeOnBadFormat)
    {
        EXPECT_EQ(invalidTime, ParseTimeString("2021/01/31 15:28:33"));
    }

    TEST_F(ParseTimeStringsTest, ItConvertsTimePointsToString)
    {
        EXPECT_EQ("2021-11-12 13:14:15", ToDateTimeString(ParseTimeString("2021-11-12 13:14:15")));
    }

    TEST_F(ParseTimeStringsTest, ItConvertsDaylightSavingsTimePointsToString)
    {
        EXPECT_EQ("2021-06-12 13:14:15", ToDateTimeString(ParseTimeString("2021-06-12 13:14:15")));
    }

    TEST_F(ParseTimeStringsTest, ItReturnsParsesCorrectIso8601ZuluTimeSingleDigits)
    {
        EXPECT_EQ(this->GetFromTimeNumbers(2021, 1, 9, 1, 2, 3), ParseIsoZuluString("2021-01-09T01:02:03.000000Z"));
    }

    TEST_F(ParseTimeStringsTest, ItReturnsParsesCorrectIso8601ZuluTimeDoubleDigits)
    {
        EXPECT_EQ(
            this->GetFromTimeNumbers(2021, 11, 12, 13, 14, 15), ParseIsoZuluString("2021-11-12T13:14:15.000000Z"));
    }
} // namespace UKControllerPluginTest::Time
