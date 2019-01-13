#include "pch/pch.h"
#include "euroscope/CallbackFunction.h"

using UKControllerPlugin::Euroscope::CallbackFunction;

namespace UKControllerPluginTest {
    namespace Function {

        TEST(CallbackFunction, EqualityOperatorReturnsTrueIfIdsSame)
        {
            CallbackFunction first = { 1, "test", [](std::string test) {} };
            CallbackFunction second = { 1, "test2", [](std::string test) {} };
            EXPECT_TRUE(first == second);
        }

        TEST(CallbackFunction, EqualityOperatorReturnsFalseIfDifferentIds)
        {
            CallbackFunction first = { 1, "test", [](std::string test) {} };
            CallbackFunction second = { 2, "test2", [](std::string test) {} };
            EXPECT_FALSE(first == second);
        }

        TEST(CallbackFunction, AssignmentOperatorSetsAllFields)
        {
            int testCount = 0;
            CallbackFunction first = { 1, "test", [&testCount](std::string test) {testCount++; } };
            CallbackFunction second = first;

            EXPECT_EQ(first.functionId, second.functionId);
            EXPECT_TRUE(first.description == second.description);
            first.function("test");
            EXPECT_EQ(1, testCount);
            second.function("test");
            EXPECT_EQ(2, testCount);
        }
    }  // namespace Function
}  // namespace UKControllerPluginTest
