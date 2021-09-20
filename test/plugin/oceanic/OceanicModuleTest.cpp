#include "oceanic/OceanicModule.h"
#include "timedevent/TimedEventCollection.h"
#include "bootstrap/PersistenceContainer.h"
#include "flightplan/FlightPlanEventHandlerCollection.h"
#include "plugin/FunctionCallEventHandler.h"
#include "dialog/DialogManager.h"
#include "tag/TagItemCollection.h"

using testing::NiceMock;
using ::testing::Test;
using UKControllerPlugin::Bootstrap::PersistenceContainer;
using UKControllerPlugin::Dialog::DialogManager;
using UKControllerPlugin::Flightplan::FlightPlanEventHandlerCollection;
using UKControllerPlugin::Oceanic::BootstrapPlugin;
using UKControllerPlugin::Plugin::FunctionCallEventHandler;
using UKControllerPlugin::Tag::TagItemCollection;
using UKControllerPlugin::TimedEvent::TimedEventCollection;
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

        TEST_F(OceanicModuleTest, BootstrapPluginRegistersCorrectNumberOfTagItems)
        {
            BootstrapPlugin(this->container);
            EXPECT_EQ(6, this->container.tagHandler->CountHandlers());
        }

        TEST_F(OceanicModuleTest, BootstrapPluginRegistersClearanceIndicatorTagItem)
        {
            BootstrapPlugin(this->container);
            EXPECT_TRUE(this->container.tagHandler->HasHandlerForItemId(118));
        }

        TEST_F(OceanicModuleTest, BootstrapPluginRegistersClearanceClearedLevelTagItem)
        {
            BootstrapPlugin(this->container);
            EXPECT_TRUE(this->container.tagHandler->HasHandlerForItemId(119));
        }

        TEST_F(OceanicModuleTest, BootstrapPluginRegistersClearanceMachNumberTagItem)
        {
            BootstrapPlugin(this->container);
            EXPECT_TRUE(this->container.tagHandler->HasHandlerForItemId(120));
        }

        TEST_F(OceanicModuleTest, BootstrapPluginRegistersClearanceEntryPointTagItem)
        {
            BootstrapPlugin(this->container);
            EXPECT_TRUE(this->container.tagHandler->HasHandlerForItemId(121));
        }

        TEST_F(OceanicModuleTest, BootstrapPluginRegistersClearanceTrackIdentifierTagItem)
        {
            BootstrapPlugin(this->container);
            EXPECT_TRUE(this->container.tagHandler->HasHandlerForItemId(122));
        }

        TEST_F(OceanicModuleTest, BootstrapPluginRegistersClearanceEntryEstimateTagItem)
        {
            BootstrapPlugin(this->container);
            EXPECT_TRUE(this->container.tagHandler->HasHandlerForItemId(123));
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
} // namespace UKControllerPluginTest
