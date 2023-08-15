#include "dialog/DialogManager.h"
#include "ecfmp/ECFMPBootstrapProvider.h"
#include "mock/MockDialogProvider.h"
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
        }

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
} // namespace UKControllerPluginTest::ECFMP
