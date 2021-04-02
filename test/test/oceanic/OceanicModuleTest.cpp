#include "pch/pch.h"
#include "Oceanic/OceanicModule.h"
#include "tag/TagItemCollection.h"
#include "timedevent/TimedEventCollection.h"
#include "bootstrap/PersistenceContainer.h"
#include "flightplan/FlightPlanEventHandlerCollection.h"
#include "plugin/FunctionCallEventHandler.h"
#include "dialog/DialogManager.h"
#include "mock/MockDialogProvider.h"

using ::testing::Test;
using testing::NiceMock;
using UKControllerPlugin::Bootstrap::PersistenceContainer;
using UKControllerPlugin::Tag::TagItemCollection;
using UKControllerPlugin::Oceanic::BootstrapPlugin;
using UKControllerPlugin::Flightplan::FlightPlanEventHandlerCollection;
using UKControllerPlugin::Plugin::FunctionCallEventHandler;
using UKControllerPlugin::TimedEvent::TimedEventCollection;
using UKControllerPlugin::Dialog::DialogManager;
using UKControllerPluginTest::Dialog::MockDialogProvider;

namespace UKControllerPluginTest {
    namespace Oceanic {

        class OceanicModuleTest : public Test
        {
            public:
                OceanicModuleTest()
                {
                    this->container.dialogManager = std::make_unique<DialogManager>(NiceMock<MockDialogProvider>());
                    this->container.timedHandler.reset(new TimedEventCollection);
                    this->container.tagHandler.reset(new TagItemCollection);
                    this->container.flightplanHandler.reset(new FlightPlanEventHandlerCollection);
                    this->container.pluginFunctionHandlers.reset(new FunctionCallEventHandler);
                }

                PersistenceContainer container;
        };

        TEST_F(OceanicModuleTest, BootstrapPluginRegistersUpdateEventEverySixtySeconds)
        {
            BootstrapPlugin(this->container);
            EXPECT_EQ(1, this->container.timedHandler->CountHandlers());
            EXPECT_EQ(1, this->container.timedHandler->CountHandlersForFrequency(60));
        }

        TEST_F(OceanicModuleTest, BootstrapPluginRegistersClearanceIndicatorTagItem)
        {
            BootstrapPlugin(this->container);
            EXPECT_EQ(1, this->container.tagHandler->CountHandlers());
            EXPECT_TRUE(this->container.tagHandler->HasHandlerForItemId(118));
        }

        TEST_F(OceanicModuleTest, BootstrapPluginRegistersOpenClearanceDialogTagFunction)
        {
            BootstrapPlugin(this->container);
            EXPECT_EQ(1, this->container.pluginFunctionHandlers->CountTagFunctions());
            EXPECT_TRUE(this->container.pluginFunctionHandlers->HasTagFunction(9010));
        }

        TEST_F(OceanicModuleTest, BootstrapPluginRegistersDialog)
        {
            BootstrapPlugin(this->container);
            EXPECT_EQ(1, this->container.dialogManager->CountDialogs());
            EXPECT_TRUE(this->container.dialogManager->HasDialog(IDD_OCEANIC_CLEARANCE));
        }
    } // namespace Oceanic
}  // namespace UKControllerPluginTest
