#include "pch/pch.h"
#include "squawk/SquawkValidator.h"

using UKControllerPlugin::Squawk::SquawkValidator;

namespace UKControllerPluginTest {
    namespace Squawk {
        TEST(SquawkValidator, ValidSquawkReturnsFalseTooShort)
        {
            EXPECT_FALSE(SquawkValidator::ValidSquawk("123"));
        }

        TEST(SquawkValidator, ValidSquawkReturnsFalseTooLong)
        {
            EXPECT_FALSE(SquawkValidator::ValidSquawk("12345"));
        }

        TEST(SquawkValidator, ValidSquawkReturnsFalseContainsLetter)
        {
            EXPECT_FALSE(SquawkValidator::ValidSquawk("1X48"));
        }

        TEST(SquawkValidator, ValidSquawkReturnsFalseContainsEight)
        {
            EXPECT_FALSE(SquawkValidator::ValidSquawk("1348"));
        }

        TEST(SquawkValidator, ValidSquawkReturnsFalseContainsNine)
        {
            EXPECT_FALSE(SquawkValidator::ValidSquawk("1348"));
        }

        TEST(SquawkValidator, ValidSquawkReturnsFalseContainsDecimal)
        {
            EXPECT_FALSE(SquawkValidator::ValidSquawk("13.8"));
        }

        TEST(SquawkValidator, ValidSquawkReturnsTrueIfValid)
        {
            EXPECT_TRUE(SquawkValidator::ValidSquawk("7654"));
        }

        TEST(SquawkValidator, ValidSquawkReturnsTrueValidSquawkStartingWithZero)
        {
            EXPECT_TRUE(SquawkValidator::ValidSquawk("0123"));
        }

        TEST(SquawkValidator, AllowedSquawkReturnsTrueOnNormalSquawk)
        {
            EXPECT_TRUE(SquawkValidator::AllowedSquawk("2521"));
        }

        TEST(SquawkValidator, AllowedSquawkReturnsFalseOnEvmergency)
        {
            EXPECT_FALSE(SquawkValidator::AllowedSquawk("7700"));
        }

        TEST(SquawkValidator, AllowedSquawkReturnsFalseOnFadioFailure)
        {
            EXPECT_FALSE(SquawkValidator::AllowedSquawk("7600"));
        }

        TEST(SquawkValidator, AllowedSquawkReturnsFalseOnHijack)
        {
            EXPECT_FALSE(SquawkValidator::AllowedSquawk("0200"));
        }

        TEST(SquawkValidator, AllowedSquawkReturnsFalseOn2200)
        {
            EXPECT_FALSE(SquawkValidator::AllowedSquawk("2200"));
        }

        TEST(SquawkValidator, AllowedSquawkReturnsFalseOn1200)
        {
            EXPECT_FALSE(SquawkValidator::AllowedSquawk("1200"));
        }

        TEST(SquawkValidator, AllowedSquawkReturnsFalseOn0200)
        {
            EXPECT_FALSE(SquawkValidator::AllowedSquawk("0200"));
        }
    }  // namespace Squawk
}  // namespace UKControllerPluginTest
