#include "euroscope/CallbackFunction.h"
#include "euroscope/EuroScopeCFlightPlanInterface.h"
#include "euroscope/EuroScopeCRadarTargetInterface.h"
#include "euroscope/EuroscopeRadarLoopbackInterface.h"
#include "euroscope/RadarScreenCallbackFunction.h"
#include "mock/MockEuroscopeRadarScreenLoopbackInterface.h"
#include "plugin/FunctionCallEventHandler.h"
#include "tag/RadarScreenTagFunction.h"
#include "tag/TagFunction.h"

using ::testing::StrictMock;
using ::testing::Test;

using UKControllerPlugin::Euroscope::CallbackFunction;
using UKControllerPlugin::Euroscope::EuroScopeCFlightPlanInterface;
using UKControllerPlugin::Euroscope::EuroScopeCRadarTargetInterface;
using UKControllerPlugin::Euroscope::EuroscopeRadarLoopbackInterface;
using UKControllerPlugin::Euroscope::RadarScreenCallbackFunction;
using UKControllerPlugin::Plugin::FunctionCallEventHandler;
using UKControllerPlugin::Tag::RadarScreenTagFunction;
using UKControllerPlugin::Tag::TagFunction;
using UKControllerPluginTest::Euroscope::MockEuroScopeCFlightPlanInterface;
using UKControllerPluginTest::Euroscope::MockEuroScopeCRadarTargetInterface;
using UKControllerPluginTest::Euroscope::MockEuroscopePluginLoopbackInterface;
using UKControllerPluginTest::Euroscope::MockEuroscopeRadarScreenLoopbackInterface;

namespace UKControllerPluginTest::Plugin {

    class FunctionCallEventHandlerTest : public Test
    {
        public:
        StrictMock<MockEuroScopeCFlightPlanInterface> flightplan;
        StrictMock<MockEuroScopeCRadarTargetInterface> radarTarget;
        StrictMock<MockEuroscopeRadarScreenLoopbackInterface> radarScreen;
    };

    TEST_F(FunctionCallEventHandlerTest, ReserveNextDynamicFunctionIdReturnsConsecutiveIds)
    {
        FunctionCallEventHandler handler;
        EXPECT_EQ(10000, handler.ReserveNextDynamicFunctionId());
        EXPECT_EQ(10001, handler.ReserveNextDynamicFunctionId());
        EXPECT_EQ(10002, handler.ReserveNextDynamicFunctionId());
        EXPECT_EQ(10003, handler.ReserveNextDynamicFunctionId());
        EXPECT_EQ(10004, handler.ReserveNextDynamicFunctionId());
        EXPECT_EQ(10005, handler.ReserveNextDynamicFunctionId());
    }

    TEST_F(FunctionCallEventHandlerTest, RegisterFunctionCallThrowExceptionIfAlreadyRegistered)
    {
        FunctionCallEventHandler handler;
        CallbackFunction function(10000, "Test Function", [](int, std::string, RECT) {});
        CallbackFunction function2(10000, "Test Function", [](int, std::string, RECT) {});

        EXPECT_NO_THROW(handler.RegisterFunctionCall(function));
        EXPECT_THROW(handler.RegisterFunctionCall(function2), std::invalid_argument);
    }

    TEST_F(FunctionCallEventHandlerTest, RegisterFunctionCallRegistersCallbackFunctionForCalling)
    {
        FunctionCallEventHandler handler;
        std::string output;
        CallbackFunction function(10000, "Test Function", [&output](int, std::string input, RECT) { output = input; });

        handler.RegisterFunctionCall(function);
        EXPECT_EQ(1, handler.CountCallbacks());
        handler.CallFunction(10000, "some test", flightplan, radarTarget, POINT(), RECT());
        EXPECT_TRUE(output == "some test");
    }

    TEST_F(FunctionCallEventHandlerTest, RegisterRadarScreenFunctionCallRegistersRadarScreenTagFunctionForCalling)
    {
        FunctionCallEventHandler handler;
        std::string output;
        RadarScreenCallbackFunction function(
            10000,
            "Test Radar Screen Callback Function",
            [&output](int, EuroscopeRadarLoopbackInterface&, std::string, const POINT&, const RECT&) {
                output = "some test";
            });

        handler.RegisterFunctionCall(function);
        EXPECT_EQ(1, handler.CountRadarScreenCallbacks());
        handler.CallFunction(radarScreen, 10000, "some test", flightplan, radarTarget, POINT(), RECT());
        EXPECT_TRUE(output == "some test");
    }

    TEST_F(FunctionCallEventHandlerTest, HasRadarScreenCallbackFunctionFunctionReturnsTrueIfItExists)
    {
        FunctionCallEventHandler handler;
        std::string output;
        RadarScreenCallbackFunction function(
            10000,
            "Test Radar Screen Callback Function",
            [&output](int, EuroscopeRadarLoopbackInterface&, std::string, const POINT&, const RECT&) {
                output = "some test";
            });

        handler.RegisterFunctionCall(function);
        EXPECT_TRUE(handler.HasRadarScreenCallbackFunction(10000));
    }

    TEST_F(FunctionCallEventHandlerTest, HasRadarScreenCallbackByDescriptionReturnsTrueIfItExists)
    {
        FunctionCallEventHandler handler;
        std::string output;
        RadarScreenCallbackFunction function(
            10000,
            "Test Radar Screen Callback Function",
            [&output](int, EuroscopeRadarLoopbackInterface&, std::string, const POINT&, const RECT&) {
                output = "some test";
            });

        handler.RegisterFunctionCall(function);
        EXPECT_TRUE(handler.HasRadarScreenCallbackByDescription("Test Radar Screen Callback Function"));
    }

    TEST_F(
        FunctionCallEventHandlerTest,
        RegisterFunctionCallThrowsExceptionIfSameRadarScreenCallbackFunctionRegisteredTwice)
    {
        FunctionCallEventHandler handler;
        std::string output;
        RadarScreenCallbackFunction function(
            10000,
            "Test Radar Screen Callback Function",
            [&output](int, EuroscopeRadarLoopbackInterface&, std::string, const POINT&, const RECT&) {
                output = "some test";
            });

        handler.RegisterFunctionCall(function);
        EXPECT_THROW(handler.RegisterFunctionCall(function), std::invalid_argument);
    }

    TEST_F(FunctionCallEventHandlerTest, RegisterFunctionCallRegistersTagFunctionForCalling)
    {
        FunctionCallEventHandler handler;
        std::string output;
        TagFunction function(
            9000,
            "Test Tag Function",
            [&output](EuroScopeCFlightPlanInterface&, EuroScopeCRadarTargetInterface&, std::string, POINT) {
                output = "some test";
            });

        handler.RegisterFunctionCall(function);
        EXPECT_EQ(1, handler.CountTagFunctions());
        handler.CallFunction(9000, "some test", flightplan, radarTarget, POINT(), RECT());
        EXPECT_TRUE(output == "some test");
    }

    TEST_F(FunctionCallEventHandlerTest, HasCallbackFunctionReturnsTrueIfItExists)
    {
        FunctionCallEventHandler handler;
        CallbackFunction function(10000, "Test Function", [](int, std::string input, RECT) {});

        handler.RegisterFunctionCall(function);
        EXPECT_TRUE(handler.HasCallbackFunction(10000));
    }

    TEST_F(FunctionCallEventHandlerTest, HasTagFunctionFunctionReturnsTrueIfItExists)
    {
        FunctionCallEventHandler handler;
        TagFunction function(
            9000,
            "Test Tag Function",
            [](EuroScopeCFlightPlanInterface&, EuroScopeCRadarTargetInterface&, std::string, POINT) {});

        handler.RegisterFunctionCall(function);
        EXPECT_TRUE(handler.HasTagFunction(9000));
    }

    TEST_F(FunctionCallEventHandlerTest, RegisterFunctionsWithEuroscopeRegistersTagFunctions)
    {
        FunctionCallEventHandler handler;
        CallbackFunction function(10000, "Test Function", [](int, std::string, RECT) {});
        TagFunction function2(
            9000,
            "Test Tag Function",
            [](EuroScopeCFlightPlanInterface&, EuroScopeCRadarTargetInterface&, std::string, POINT) {});
        StrictMock<MockEuroscopePluginLoopbackInterface> mockPlugin;

        RadarScreenTagFunction radarScreenTagFunction(
            9001,
            "Test Radar Screen Function",
            [](EuroscopeRadarLoopbackInterface&,
               EuroScopeCFlightPlanInterface&,
               EuroScopeCRadarTargetInterface&,
               std::string,
               const POINT&,
               const RECT&) {});

        EXPECT_CALL(mockPlugin, RegisterTagFunction(function.functionId, function.description)).Times(0);
        EXPECT_CALL(mockPlugin, RegisterTagFunction(function2.functionId, function2.description)).Times(1);
        EXPECT_CALL(
            mockPlugin, RegisterTagFunction(radarScreenTagFunction.functionId, radarScreenTagFunction.description))
            .Times(1);

        handler.RegisterFunctionCall(function);
        handler.RegisterFunctionCall(function2);
        handler.RegisterFunctionCall(radarScreenTagFunction);
        handler.RegisterTagFunctionsWithEuroscope(mockPlugin);
    }

    TEST_F(FunctionCallEventHandlerTest, FixedFunctionCallHandlesNonExistant)
    {
        FunctionCallEventHandler handler;
        EXPECT_NO_THROW(handler.CallFunction(9000, "some test", flightplan, radarTarget, POINT(), RECT()));
    }

    TEST_F(FunctionCallEventHandlerTest, DynamicFunctionCallHandlesNonExistant)
    {
        FunctionCallEventHandler handler;
        EXPECT_NO_THROW(handler.CallFunction(10000, "some test", flightplan, radarTarget, POINT(), RECT()));
    }

    TEST_F(FunctionCallEventHandlerTest, DynamicRadarScreenFunctionCallHandlesNonExistant)
    {
        FunctionCallEventHandler handler;
        EXPECT_NO_THROW(
            handler.CallFunction(radarScreen, 10000, "some test", flightplan, radarTarget, POINT(), RECT()));
    }

    TEST_F(FunctionCallEventHandlerTest, RadarScreenFunctionCallHandlesNonExistant)
    {
        FunctionCallEventHandler handler;
        EXPECT_NO_THROW(
            handler.CallFunction(radarScreen, 10000, "some test", flightplan, radarTarget, POINT(), RECT()));
    }

    TEST_F(FunctionCallEventHandlerTest, RegisterFunctionCallRegistersRadarScreenTagFunctionForCalling)
    {
        FunctionCallEventHandler handler;
        std::string output;
        RadarScreenTagFunction function(
            9000,
            "Test Radar Screen Function",
            [&output](
                EuroscopeRadarLoopbackInterface&,
                EuroScopeCFlightPlanInterface&,
                EuroScopeCRadarTargetInterface&,
                std::string,
                const POINT&,
                const RECT&) { output = "some test"; });

        handler.RegisterFunctionCall(function);
        EXPECT_EQ(1, handler.CountRadarScreenTagFunctions());
        handler.CallFunction(radarScreen, 9000, "some test", flightplan, radarTarget, POINT(), RECT());
        EXPECT_TRUE(output == "some test");
    }

    TEST_F(FunctionCallEventHandlerTest, HasRadarScreenTagFunctionFunctionReturnsTrueIfItExists)
    {
        FunctionCallEventHandler handler;
        RadarScreenTagFunction function(
            9000,
            "Test Radar Screen Function",
            [](EuroscopeRadarLoopbackInterface&,
               EuroScopeCFlightPlanInterface&,
               EuroScopeCRadarTargetInterface&,
               std::string,
               const POINT&,
               const RECT&) {});

        handler.RegisterFunctionCall(function);
        EXPECT_TRUE(handler.HasRadarScreenTagFunction(9000));
    }

    TEST_F(FunctionCallEventHandlerTest, RegisterFunctionCallThrowsExceptionIfSameRadarScreenTagFunctionRegisteredTwice)
    {
        FunctionCallEventHandler handler;
        RadarScreenTagFunction function(
            9000,
            "Test Radar Screen Function",
            [](EuroscopeRadarLoopbackInterface&,
               EuroScopeCFlightPlanInterface&,
               EuroScopeCRadarTargetInterface&,
               std::string,
               const POINT&,
               const RECT&) {});

        handler.RegisterFunctionCall(function);
        EXPECT_THROW(handler.RegisterFunctionCall(function), std::invalid_argument);
    }
} // namespace UKControllerPluginTest::Plugin
