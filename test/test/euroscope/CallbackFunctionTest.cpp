#include "pch/pch.h"
#include "euroscope/CallbackFunction.h"

using UKControllerPlugin::Euroscope::CallbackFunction;

namespace UKControllerPluginTest {
    namespace Function {

        TEST(CallbackFunction, EqualityOperatorReturnsTrueIfIdsSame)
        {
            CallbackFunction first = { 1, "test", [](int, std::string test, RECT) {} };
            CallbackFunction second = { 1, "test2", [](int, std::string test, RECT) {} };
            EXPECT_TRUE(first == second);
        }

        TEST(CallbackFunction, EqualityOperatorReturnsFalseIfDifferentIds)
        {
            CallbackFunction first = { 1, "test", [](int, std::string test, RECT) {} };
            CallbackFunction second = { 2, "test2", [](int, std::string test, RECT) {} };
            EXPECT_FALSE(first == second);
        }

        TEST(CallbackFunction, AssignmentOperatorSetsAllFields)
        {
            int testCount = 0;
            CallbackFunction first = { 1, "test", [&testCount](int, std::string test, RECT) {testCount++; } };
            CallbackFunction second = first;

            EXPECT_EQ(first.functionId, second.functionId);
            EXPECT_TRUE(first.description == second.description);
            first.function(0, "test", {});
            EXPECT_EQ(1, testCount);
            second.function(0, "test", {});
            EXPECT_EQ(2, testCount);
        }
    }  // namespace Function
}  // namespace UKControllerPluginTest
