#include "pch/pch.h"
#include "wake/WakeModule.h"
#include "bootstrap/PersistenceContainer.h"
#include "mock/MockDependencyLoader.h"
#include "flightplan/FlightPlanEventHandlerInterface.h"
#include "tag/TagItemCollection.h"

using UKControllerPlugin::Bootstrap::PersistenceContainer;
using UKControllerPluginTest::Dependency::MockDependencyLoader;
using UKControllerPlugin::Flightplan::FlightPlanEventHandlerCollection;
using UKControllerPlugin::Tag::TagItemCollection;
using UKControllerPlugin::Wake::BootstrapPlugin;
using ::testing::Test;
using ::testing::NiceMock;

namespace UKControllerPluginTest {
    namespace Wake {

        class WakeModuleTest : public Test
        {
            public:
                WakeModuleTest()
                {
                    container.flightplanHandler.reset(new FlightPlanEventHandlerCollection);
                    container.tagHandler.reset(new TagItemCollection);
                }

                PersistenceContainer container;
                NiceMock<MockDependencyLoader> dependencies;
        };

        TEST_F(WakeModuleTest, ItAddsToFlightplanHandler)
        {
            BootstrapPlugin(this->container, this->dependencies);
            EXPECT_EQ(1, this->container.flightplanHandler->CountHandlers());
        }

        TEST_F(WakeModuleTest, ItAddsToTagHandler)
        {
            BootstrapPlugin(this->container, this->dependencies);
            EXPECT_EQ(2, this->container.tagHandler->CountHandlers());
        }

        TEST_F(WakeModuleTest, ItAddsToTagHandlerWithCorrectId)
        {
            BootstrapPlugin(this->container, this->dependencies);
            EXPECT_TRUE(this->container.tagHandler->HasHandlerForItemId(UKControllerPlugin::Wake::tagItemId));
        }

        TEST_F(WakeModuleTest, ItAddsToTagHandlerWithCorrectIdRecat)
        {
            BootstrapPlugin(this->container, this->dependencies);
            EXPECT_TRUE(this->container.tagHandler->HasHandlerForItemId(UKControllerPlugin::Wake::tagItemRecatId));
        }

    }  // namespace Wake
}  // namespace UKControllerPluginTest
