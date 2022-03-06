#include "string/StringTrimFunctions.h"

using UKControllerPluginUtils::String::ltrim;
using UKControllerPluginUtils::String::rtrim;
using UKControllerPluginUtils::String::trim;

namespace UKControllerPluginUtilsTest::String {
    class StringTrimFunctionsTest : public testing::Test
    {
    };

    TEST_F(StringTrimFunctionsTest, TestLeftTrimRemovesCharactersFromLeftOfString)
    {
        EXPECT_EQ("abc", ltrim("xabc", "x"));
    }

    TEST_F(StringTrimFunctionsTest, TestLeftTrimRemovesMultipleCharactersFromLeftOfString)
    {
        EXPECT_EQ("abc", ltrim("xyzabc", "xyz"));
    }

    TEST_F(StringTrimFunctionsTest, TestLeftTrimDoesntRemoveFromMiddleOfString)
    {
        EXPECT_EQ("axbxc", ltrim("axbxc", "x"));
    }

    TEST_F(StringTrimFunctionsTest, TestLeftTrimHasDefaultCharacters)
    {
        EXPECT_EQ("abc", ltrim("\n\r\f\v\tabc"));
    }

    TEST_F(StringTrimFunctionsTest, TestRightTrimRemovesCharactersFromRightOfString)
    {
        EXPECT_EQ("abc", rtrim("abcx", "x"));
    }

    TEST_F(StringTrimFunctionsTest, TestRightTrimRemovesMultipleCharactersFromRightOfString)
    {
        EXPECT_EQ("abc", rtrim("abcxyz", "xyz"));
    }

    TEST_F(StringTrimFunctionsTest, TestRightTrimDoesntRemoveFromMiddleOfString)
    {
        EXPECT_EQ("axbxc", rtrim("axbxc", "x"));
    }

    TEST_F(StringTrimFunctionsTest, TestRightTrimHasDefaultCharacters)
    {
        EXPECT_EQ("abc", rtrim("abc\n\r\f\v\t"));
    }

    TEST_F(StringTrimFunctionsTest, TestTrimRemovesCharactersFromBothSidesOfString)
    {
        EXPECT_EQ("abc", trim("xabcx", "x"));
    }

    TEST_F(StringTrimFunctionsTest, TestTrimRemovesMultipleCharactersFromBothSidesOfString)
    {
        EXPECT_EQ("abc", trim("xyzabcxyz", "xyz"));
    }

    TEST_F(StringTrimFunctionsTest, TestTrimDoesntRemoveFromMiddleOfString)
    {
        EXPECT_EQ("axbxc", trim("axbxc", "x"));
    }

    TEST_F(StringTrimFunctionsTest, TestTrimHasDefaultCharacters)
    {
        EXPECT_EQ("abc", trim("\n\r\f\v\tabc\n\r\f\v\t"));
    }
} // namespace UKControllerPluginUtilsTest::String
