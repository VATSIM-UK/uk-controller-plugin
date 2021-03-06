#include "pch/pch.h"
#include "flightinformationservice/FlightInformationServiceModule.h"
#include "tag/TagItemCollection.h"
#include "bootstrap/PersistenceContainer.h"
#include "flightplan/FlightPlanEventHandlerCollection.h"

using ::testing::Test;
using UKControllerPlugin::Bootstrap::PersistenceContainer;
using UKControllerPlugin::Tag::TagItemCollection;
using UKControllerPlugin::FlightInformationService::BootstrapPlugin;
using UKControllerPlugin::Flightplan::FlightPlanEventHandlerCollection;

namespace UKControllerPluginTest {
    namespace FlightInformationService {

        class FlightInformationServiceModuleTest : public Test
        {
            public:
                FlightInformationServiceModuleTest()
                {
                    this->container.tagHandler.reset(new TagItemCollection);
                    this->container.flightplanHandler.reset(new FlightPlanEventHandlerCollection);
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
    }  // namespace FlightInformationService
}  // namespace UKControllerPluginTest
