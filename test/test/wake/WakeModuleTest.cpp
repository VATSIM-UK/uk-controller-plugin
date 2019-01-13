#include "pch/pch.h"
#include "wake/WakeModule.h"
#include "bootstrap/PersistenceContainer.h"
#include "dependency/DependencyCache.h"
#include "flightplan/FlightPlanEventHandlerInterface.h"
#include "tag/TagItemCollection.h"

using UKControllerPlugin::Bootstrap::PersistenceContainer;
using UKControllerPlugin::Dependency::DependencyCache;
using UKControllerPlugin::Flightplan::FlightPlanEventHandlerCollection;
using UKControllerPlugin::Tag::TagItemCollection;
using UKControllerPlugin::Wake::BootstrapPlugin;
using ::testing::Test;

namespace UKControllerPluginTest {
    namespace Wake {

        class WakeModuleTest : public Test
        {
            public:
                WakeModuleTest()
                {
                    nlohmann::json data;
                    data["A333"] = "H";
                    data["B738"] = "LM";
                    dependencies.AddDependency("wake-categories.json", data.dump());

                    container.flightplanHandler.reset(new FlightPlanEventHandlerCollection);
                    container.tagHandler.reset(new TagItemCollection);
                }

                PersistenceContainer container;
                DependencyCache dependencies;
        };

        TEST_F(WakeModuleTest, ItAddsToFlightplanHandler)
        {
            BootstrapPlugin(this->container, this->dependencies);
            EXPECT_EQ(1, this->container.flightplanHandler->CountHandlers());
        }

        TEST_F(WakeModuleTest, ItAddsToTagHandler)
        {
            BootstrapPlugin(this->container, this->dependencies);
            EXPECT_EQ(1, this->container.tagHandler->CountHandlers());
        }

        TEST_F(WakeModuleTest, ItAddsToTagHandlerWithCorrectId)
        {
            BootstrapPlugin(this->container, this->dependencies);
            EXPECT_TRUE(this->container.tagHandler->HasHandlerForItemId(UKControllerPlugin::Wake::tagItemId));
        }

    }  // namespace Wake
}  // namespace UKControllerPluginTest
