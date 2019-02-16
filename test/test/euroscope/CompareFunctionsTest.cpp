#include "pch/pch.h"
#include "euroscope/CompareFunctions.h"
#include "tag/TagFunction.h"
#include "euroscope/CallbackFunction.h"
#include "euroscope/EuroScopeCFlightPlanInterface.h"
#include "euroscope/EuroScopeCRadarTargetInterface.h"

using UKControllerPlugin::Euroscope::CallbackFunction;
using UKControllerPlugin::Euroscope::CompareFunctions;
using UKControllerPlugin::Tag::TagFunction;
using UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface;
using UKControllerPlugin::Euroscope::EuroScopeCRadarTargetInterface;

namespace UKControllerPluginTest {
    namespace Function {
        TEST(CompareFunctions, OperatorReturnsTrueIfCallbackLessThanId)
        {
            CompareFunctions compare;
            CallbackFunction function = { 1, "test", [](int, std::string) {} };
            EXPECT_TRUE(compare(function, 2));
        }

        TEST(CompareFunctions, OperatorReturnsTrueIfIdLessThanCallback)
        {
            CompareFunctions compare;
            CallbackFunction function = { 1, "test", [](int, std::string) {} };
            EXPECT_TRUE(compare(0, function));
        }

        TEST(CompareFunctions, OperatorReturnsTrueIfFirstCallbackLessThanSecond)
        {
            CompareFunctions compare;
            CallbackFunction function1 = { 1, "test", [](int, std::string) {} };
            CallbackFunction function2 = { 2, "test", [](int, std::string) {} };
            EXPECT_TRUE(compare(function1, function2));
        }

        TEST(CompareFunctions, OperatorReturnsTrueIfTagFunctionLessThanId)
        {
            CompareFunctions compare;
            TagFunction function = {
                1,
                "test",
                [](EuroScopeCFlightPlanInterface &, EuroScopeCRadarTargetInterface &, std::string, POINT) {}
            };
            EXPECT_TRUE(compare(function, 2));
        }

        TEST(CompareFunctions, OperatorReturnsTrueIfIdLessThanTagFunction)
        {
            CompareFunctions compare;
            TagFunction function = {
                1,
                "test",
                [](EuroScopeCFlightPlanInterface &, EuroScopeCRadarTargetInterface &, std::string, POINT) {}
            };
            EXPECT_TRUE(compare(0, function));
        }

        TEST(CompareFunctions, OperatorReturnsTrueIfFirstTagFunctionLessThanSecond)
        {
            CompareFunctions compare;
            TagFunction function1 = {
                1,
                "test",
                [](EuroScopeCFlightPlanInterface &, EuroScopeCRadarTargetInterface &, std::string, POINT) {}
            };
            TagFunction function2 = {
                2,
                "test",
                [](EuroScopeCFlightPlanInterface &, EuroScopeCRadarTargetInterface &, std::string, POINT) {}
            };
            EXPECT_TRUE(compare(function1, function2));
        }
    }  // namespace Function
}  // namespace UKControllerPluginTest
