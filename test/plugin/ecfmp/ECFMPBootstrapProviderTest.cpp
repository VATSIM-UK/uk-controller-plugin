#include "bootstrap/ModuleFactories.h"
#include "controller/ActiveCallsignCollection.h"
#include "dialog/DialogManager.h"
#include "ecfmp/AircraftFlowMeasureMap.h"
#include "ecfmp/ECFMPBootstrapProvider.h"
#include "ecfmp/ECFMPModuleFactory.h"
#include "flightplan/FlightPlanEventHandlerCollection.h"
#include "plugin/FunctionCallEventHandler.h"
#include "tag/TagItemCollection.h"
#include "test/BootstrapProviderTestCase.h"
#include "timedevent/TimedEventCollection.h"

namespace UKControllerPluginTest::ECFMP {
    class ECFMPBootstrapProviderTest : public UKControllerPluginTest::BootstrapProviderTestCase
    {
        public:
        ECFMPBootstrapProviderTest()
        {
            container.dialogManager = std::make_unique<UKControllerPlugin::Dialog::DialogManager>(mockDialogProvider);
            container.flightplanHandler =
                std::make_unique<UKControllerPlugin::Flightplan::FlightPlanEventHandlerCollection>();
        }

        testing::NiceMock<Curl::MockCurlApi> mockCurlApi;
        UKControllerPlugin::Controller::ActiveCallsignCollection activeCallsigns;
        testing::NiceMock<Dialog::MockDialogProvider> mockDialogProvider;
        UKControllerPlugin::ECFMP::ECFMPBootstrapProvider provider;
    };

    TEST_F(ECFMPBootstrapProviderTest, ItRegistersForTimedEventsOnBootstrapPlugin)
    {
        RunBootstrapPlugin(provider);
        EXPECT_EQ(1, container.timedHandler->CountHandlers());
        EXPECT_EQ(1, container.timedHandler->CountHandlersForFrequency(1));
    }

    TEST_F(ECFMPBootstrapProviderTest, ItRegistersTagItemForRelevantFlowMeasures)
    {
        RunBootstrapPlugin(provider);
        EXPECT_EQ(1, container.tagHandler->CountHandlers());
        EXPECT_EQ(1, container.tagHandler->HasHandlerForItemId(131));
    }

    TEST_F(ECFMPBootstrapProviderTest, ItRegistersTheFlowMeasuresDialog)
    {
        RunBootstrapPlugin(provider);
        EXPECT_EQ(1, container.dialogManager->CountDialogs());
        EXPECT_EQ(1, container.dialogManager->HasDialog(IDD_FLOW_MEASURE_LIST));
    }

    TEST_F(ECFMPBootstrapProviderTest, ItRegistersTheListFlowMeasuresTagFunction)
    {
        RunBootstrapPlugin(provider);
        EXPECT_EQ(1, container.pluginFunctionHandlers->CountTagFunctions());
        EXPECT_EQ(1, container.pluginFunctionHandlers->HasTagFunction(9023));
    }

    TEST_F(ECFMPBootstrapProviderTest, ItRegistersTheFlowMeasureMapForECFMPActivatedEvents)
    {
        RunBootstrapPlugin(provider);
        auto hasListener = container.moduleFactories->ECFMP()
                               .Sdk(mockCurlApi, activeCallsigns)
                               ->EventBus()
                               .HasListenerOfType<
                                   UKControllerPlugin::ECFMP::AircraftFlowMeasureMap,
                                   ::ECFMP::Plugin::FlowMeasureActivatedEvent>();
        EXPECT_TRUE(hasListener);
    }

    TEST_F(ECFMPBootstrapProviderTest, ItRegistersTheFlowMeasureMapForECFMPWithdrawnEvents)
    {
        RunBootstrapPlugin(provider);
        auto hasListener = container.moduleFactories->ECFMP()
                               .Sdk(mockCurlApi, activeCallsigns)
                               ->EventBus()
                               .HasListenerOfType<
                                   UKControllerPlugin::ECFMP::AircraftFlowMeasureMap,
                                   ::ECFMP::Plugin::FlowMeasureWithdrawnEvent>();
        EXPECT_TRUE(hasListener);
    }

    TEST_F(ECFMPBootstrapProviderTest, ItRegistersTheFlowMeasureMapForECFMPExpiredEvents)
    {
        RunBootstrapPlugin(provider);
        auto hasListener = container.moduleFactories->ECFMP()
                               .Sdk(mockCurlApi, activeCallsigns)
                               ->EventBus()
                               .HasListenerOfType<
                                   UKControllerPlugin::ECFMP::AircraftFlowMeasureMap,
                                   ::ECFMP::Plugin::FlowMeasureExpiredEvent>();
        EXPECT_TRUE(hasListener);
    }

    TEST_F(ECFMPBootstrapProviderTest, ItRegistersTheFlowMeasureMapForFlightplanEvents)
    {
        RunBootstrapPlugin(provider);
        EXPECT_EQ(1, container.flightplanHandler->CountHandlers());
    }

    TEST_F(ECFMPBootstrapProviderTest, ItRegistersTheFlowMeasureMapForActiveCallsignEvents)
    {
        RunBootstrapPlugin(provider);
        EXPECT_EQ(1, container.activeCallsigns->CountHandlers());
    }
} // namespace UKControllerPluginTest::ECFMP
