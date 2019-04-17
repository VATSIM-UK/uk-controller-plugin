#include "pch/pch.h"
#include "metar/MetarParsingFunctions.h"

using UKControllerPlugin::Metar::noQnh;
using UKControllerPlugin::Metar::GetQnhString;

namespace UKControllerPluginTest {
    namespace Metar {

        TEST(MetarParsingFunctionsTest, GetQnhStringReturnsNothingIfNoQnhInMetar)
        {
            EXPECT_EQ(noQnh, GetQnhString("EGKK 05010KT SCT010"));
        }

        TEST(MetarParsingFunctionsTest, GetQnhStringReturnsNothingIfQnhNotSeparatedBySpacesLeft)
        {
            EXPECT_EQ(noQnh, GetQnhString("EGKK 05010KTQ1010 SCT010"));
        }

        TEST(MetarParsingFunctionsTest, GetQnhStringReturnsNothingIfQnhNotSeparatedBySpacesRight)
        {
            EXPECT_EQ(noQnh, GetQnhString("EGKK 05010KT Q1010SCT010"));
        }

        TEST(MetarParsingFunctionsTest, GetQnhStringReturnsFourDigitQnh)
        {
            EXPECT_EQ("1010", GetQnhString("EGKK 05010KT Q1010 Q1010SCT010"));
        }

        TEST(MetarParsingFunctionsTest, GetQnhStringReturnsThreeDigitQnh)
        {
            EXPECT_EQ("0987", GetQnhString("EGKK 05010KT Q0987 Q1010SCT010"));
        }
    }  // namespace Metar
}  // namespace UKControllerPluginTest
