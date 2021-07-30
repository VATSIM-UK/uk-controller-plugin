#include "pch/pch.h"
#include "wake/WakeModule.h"
#include "bootstrap/PersistenceContainer.h"
#include "mock/MockDependencyLoader.h"
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
            EXPECT_EQ(5, this->container.tagHandler->CountHandlers());
        }

        TEST_F(WakeModuleTest, ItAddsToTagHandlerWithCorrectIdAircraftTypeUkCategory)
        {
            BootstrapPlugin(this->container, this->dependencies);
            EXPECT_TRUE(this->container.tagHandler->HasHandlerForItemId(105));
        }

        TEST_F(WakeModuleTest, ItAddsToTagHandlerWithCorrectIdStandalone)
        {
            BootstrapPlugin(this->container, this->dependencies);
            EXPECT_TRUE(this->container.tagHandler->HasHandlerForItemId(112));
        }

        TEST_F(WakeModuleTest, ItAddsToTagHandlerWithCorrectIdRecat)
        {
            BootstrapPlugin(this->container, this->dependencies);
            EXPECT_TRUE(this->container.tagHandler->HasHandlerForItemId(113));
        }

        TEST_F(WakeModuleTest, ItAddsToTagHandlerWithCorrectIdUKRecatCombined)
        {
            BootstrapPlugin(this->container, this->dependencies);
            EXPECT_TRUE(this->container.tagHandler->HasHandlerForItemId(114));
        }

        TEST_F(WakeModuleTest, ItAddsToTagHandlerWithCorrectIdAircraftTypeRecat)
        {
            BootstrapPlugin(this->container, this->dependencies);
            EXPECT_TRUE(this->container.tagHandler->HasHandlerForItemId(115));
        }
    }  // namespace Wake
}  // namespace UKControllerPluginTest
