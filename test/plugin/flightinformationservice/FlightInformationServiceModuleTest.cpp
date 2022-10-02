#include "flightinformationservice/FlightInformationServiceModule.h"
#include "tag/TagItemCollection.h"
#include "bootstrap/PersistenceContainer.h"
#include "flightplan/FlightPlanEventHandlerCollection.h"
#include "plugin/FunctionCallEventHandler.h"

using ::testing::Test;
using UKControllerPlugin::Bootstrap::PersistenceContainer;
using UKControllerPlugin::Tag::TagItemCollection;
using UKControllerPlugin::FlightInformationService::BootstrapPlugin;
using UKControllerPlugin::Flightplan::FlightPlanEventHandlerCollection;
using UKControllerPlugin::Plugin::FunctionCallEventHandler;

namespace UKControllerPluginTest {
    namespace FlightInformationService {

        class FlightInformationServiceModuleTest : public Test
        {
            public:
                FlightInformationServiceModuleTest()
                {
                    this->container.tagHandler.reset(new TagItemCollection);
                    this->container.flightplanHandler.reset(new FlightPlanEventHandlerCollection);
                    this->container.pluginFunctionHandlers.reset(new FunctionCallEventHandler);
                }

                PersistenceContainer container;
        };

        TEST_F(FlightInformationServiceModuleTest, BootstrapPluginRegistersBaseTagItem)
        {
            BootstrapPlugin(this->container);
            EXPECT_TRUE(this->container.tagHandler->HasHandlerForItemId(116));
        }

        TEST_F(FlightInformationServiceModuleTest, BootstrapPluginRegistersOrBlankTagItem)
        {
            BootstrapPlugin(this->container);
            EXPECT_TRUE(this->container.tagHandler->HasHandlerForItemId(117));
        }

        TEST_F(FlightInformationServiceModuleTest, BootstrapPluginRegistersForFlightplanEvents)
        {
            BootstrapPlugin(this->container);
            EXPECT_EQ(1, this->container.flightplanHandler->CountHandlers());
        }

        TEST_F(FlightInformationServiceModuleTest, BootstrapPluginRegistersSelectionMenuTagItem)
        {
            BootstrapPlugin(this->container);
            EXPECT_EQ(1, this->container.pluginFunctionHandlers->CountTagFunctions());
            EXPECT_TRUE(this->container.pluginFunctionHandlers->HasTagFunction(9009));
        }

        TEST_F(FlightInformationServiceModuleTest, BootstrapPluginRegistersUkfisSelectedCallback)
        {
            BootstrapPlugin(this->container);
            EXPECT_EQ(1, this->container.pluginFunctionHandlers->CountCallbacks());
            EXPECT_TRUE(this->container.pluginFunctionHandlers->HasCallbackFunction(5000));
        }
    }  // namespace FlightInformationService
}  // namespace UKControllerPluginTest
