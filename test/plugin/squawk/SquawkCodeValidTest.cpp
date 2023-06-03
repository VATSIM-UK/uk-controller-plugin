#include "squawk/SquawkCodeValid.h"

namespace UKControllerPluginTest::Squawk {

    struct SquawkCodeValidTestCase
    {
        std::string description;
        std::string squawk;
        bool expectedResult;
    };

    class SquawkCodeValidTest : public ::testing::TestWithParam<SquawkCodeValidTestCase>
    {
    };

    TEST_P(SquawkCodeValidTest, ItValidatesSquawkCode)
    {
        const auto& param = GetParam();
        const auto squawk = param.squawk;
        const auto expectedResult = param.expectedResult;

        ASSERT_EQ(expectedResult, UKControllerPlugin::Squawk::SquawkCodeValid(squawk));
    }

    INSTANTIATE_TEST_SUITE_P(
        SquawkCodeValidTestCases,
        SquawkCodeValidTest,
        testing::Values(
            SquawkCodeValidTestCase{"valid_1234", "1234", true},
            SquawkCodeValidTestCase{"valid_0000", "0000", true},
            SquawkCodeValidTestCase{"valid_7777", "7777", true},
            SquawkCodeValidTestCase{"valid_4567", "4567", true},
            SquawkCodeValidTestCase{"valid_7654", "7654", true},
            SquawkCodeValidTestCase{"valid_2461", "2461", true},
            SquawkCodeValidTestCase{"too_short", "123", false},
            SquawkCodeValidTestCase{"too_long", "12345", false},
            SquawkCodeValidTestCase{"empty", "", false},
            SquawkCodeValidTestCase{"non_numeric", "ABCD", false},
            SquawkCodeValidTestCase{"non_octal", "8888", false},
            SquawkCodeValidTestCase{"symbols", "!@#$", false},
            SquawkCodeValidTestCase{"negative", "-123", false}),
        [](const ::testing::TestParamInfo<SquawkCodeValidTest::ParamType>& info) { return info.param.description; });
} // namespace UKControllerPluginTest::Squawk
