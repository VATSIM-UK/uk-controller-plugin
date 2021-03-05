#include "pch/pch.h"
#include "flightinformationservice/FlightInformationServiceModule.h"
#include "tag/TagItemCollection.h"
#include "bootstrap/PersistenceContainer.h"

using ::testing::Test;
using UKControllerPlugin::Bootstrap::PersistenceContainer;
using UKControllerPlugin::Tag::TagItemCollection;
using UKControllerPlugin::FlightInformationService::BootstrapPlugin;

namespace UKControllerPluginTest {
    namespace FlightInformationService {

        class FlightInformationServiceModuleTest : public Test
        {
            public:
                FlightInformationServiceModuleTest()
                {
                    this->container.tagHandler.reset(new TagItemCollection);
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
    }  // namespace FlightInformationService
}  // namespace UKControllerPluginTest
