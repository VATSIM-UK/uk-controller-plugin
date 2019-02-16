#include "pch/pch.h"
#include "plugin/FunctionCallEventHandler.h"
#include "euroscope/CallbackFunction.h"
#include "mock/MockEuroscopePluginLoopbackInterface.h"
#include "mock/MockEuroScopeCFlightplanInterface.h"
#include "mock/MockEuroScopeCRadarTargetInterface.h"
#include "tag/TagFunction.h"
#include "euroscope/EuroScopeCRadarTargetInterface.h"
#include "euroscope/EuroScopeCFlightPlanInterface.h"

using UKControllerPlugin::Plugin::FunctionCallEventHandler;
using UKControllerPlugin::Euroscope::CallbackFunction;
using UKControllerPluginTest::Euroscope::MockEuroscopePluginLoopbackInterface;
using UKControllerPluginTest::Euroscope::MockEuroScopeCFlightPlanInterface;
using UKControllerPluginTest::Euroscope::MockEuroScopeCRadarTargetInterface;
using UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface;
using UKControllerPlugin::Euroscope::EuroScopeCRadarTargetInterface;
using UKControllerPlugin::Tag::TagFunction;
using ::testing::StrictMock;

namespace UKControllerPluginTest {
    namespace Plugin {

        TEST(FunctionCallEventHandler, ReserveNextDynamicFunctionIdReturnsConsecutiveIds)
        {
            FunctionCallEventHandler handler;
            EXPECT_EQ(5000, handler.ReserveNextDynamicFunctionId());
            EXPECT_EQ(5001, handler.ReserveNextDynamicFunctionId());
            EXPECT_EQ(5002, handler.ReserveNextDynamicFunctionId());
            EXPECT_EQ(5003, handler.ReserveNextDynamicFunctionId());
            EXPECT_EQ(5004, handler.ReserveNextDynamicFunctionId());
            EXPECT_EQ(5005, handler.ReserveNextDynamicFunctionId());
        }

        TEST(FunctionCallEventHandler, RegisterFunctionCallThrowExceptionIfAlreadyRegistered)
        {
            FunctionCallEventHandler handler;
            CallbackFunction function(5000, "Test Function", [](int, std::string) {});
            CallbackFunction function2(5000, "Test Function", [](int, std::string) {});

            EXPECT_NO_THROW(handler.RegisterFunctionCall(function));
            EXPECT_THROW(handler.RegisterFunctionCall(function2), std::invalid_argument);
        }

        TEST(FunctionCallEventHandler, RegisterFunctionCallRegistersCallbackFunctionForCalling)
        {
            FunctionCallEventHandler handler;
            std::string output = "";
            CallbackFunction function(5000, "Test Function", [&output](int, std::string input) {output = input; });

            handler.RegisterFunctionCall(function);
            EXPECT_EQ(1, handler.CountCallbacks());
            handler.CallFunction(
                5000,
                "some test",
                StrictMock<MockEuroScopeCFlightPlanInterface>(),
                StrictMock<MockEuroScopeCRadarTargetInterface>(),
                POINT()
            );
            EXPECT_TRUE(output == "some test");
        }

        TEST(FunctionCallEventHandler, RegisterFunctionCallRegistersTagFunctionForCalling)
        {
            FunctionCallEventHandler handler;
            std::string output = "";
            TagFunction function(
                9000,
                "Test Tag Function",
                [&output]
                (EuroScopeCFlightPlanInterface &, EuroScopeCRadarTargetInterface &, std::string, POINT) {
                    output = "some test"; 
                }
            );

            handler.RegisterFunctionCall(function);
            EXPECT_EQ(1, handler.CountTagFunctions());
            handler.CallFunction(
                9000,
                "some test",
                StrictMock<MockEuroScopeCFlightPlanInterface>(),
                StrictMock<MockEuroScopeCRadarTargetInterface>(),
                POINT()
            );
            EXPECT_TRUE(output == "some test");
        }

        TEST(FunctionCallEventHandler, HasCallbackFunctionReturnsTrueIfItExists)
        {
            FunctionCallEventHandler handler;
            CallbackFunction function(5000, "Test Function", [](int, std::string input) {});

            handler.RegisterFunctionCall(function);
            EXPECT_TRUE(handler.HasCallbackFunction(5000));
        }

        TEST(FunctionCallEventHandler, HasTagFunctionFunctionReturnsTrueIfItExists)
        {
            FunctionCallEventHandler handler;
            TagFunction function(
                9000,
                "Test Tag Function",
                [](EuroScopeCFlightPlanInterface &, EuroScopeCRadarTargetInterface &, std::string, POINT) {}
            );

            handler.RegisterFunctionCall(function);
            EXPECT_TRUE(handler.HasTagFunction(9000));
        }

        TEST(FunctionCallEventHandler, RegisterFunctionsWithEuroscopeRegistersTagFunctions)
        {
            FunctionCallEventHandler handler;
            CallbackFunction function(5000, "Test Function", [](int, std::string) {});
            TagFunction function2(
                9000,
                "Test Tag Function",
                [](EuroScopeCFlightPlanInterface &, EuroScopeCRadarTargetInterface &, std::string, POINT) {}
            );
            StrictMock<MockEuroscopePluginLoopbackInterface> mockPlugin;

            EXPECT_CALL(mockPlugin, RegisterTagFunction(function2.functionId, function2.description))
                .Times(1);

            handler.RegisterFunctionCall(function);
            handler.RegisterFunctionCall(function2);
            handler.RegisterTagFunctionsWithEuroscope(mockPlugin);
        }

        TEST(FunctionCallEventHandler, FixedFunctionCallHandlesNonExistant)
        {
            FunctionCallEventHandler handler;
            EXPECT_NO_THROW(
                handler.CallFunction(
                    9000,
                    "some test",
                    StrictMock<MockEuroScopeCFlightPlanInterface>(),
                    StrictMock<MockEuroScopeCRadarTargetInterface>(),
                    POINT()
                )
            );
        }

        TEST(FunctionCallEventHandler, DynamicFunctionCallHandlesNonExistant)
        {
            FunctionCallEventHandler handler;
            EXPECT_NO_THROW(
                handler.CallFunction(
                    5000,
                    "some test",
                    StrictMock<MockEuroScopeCFlightPlanInterface>(),
                    StrictMock<MockEuroScopeCRadarTargetInterface>(),
                    POINT()
                )
            );
        }
    }  // namespace Plugin
}  // namespace UKControllerPluginTest
