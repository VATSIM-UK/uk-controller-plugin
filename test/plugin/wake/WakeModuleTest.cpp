#include "bootstrap/PersistenceContainer.h"
#include "flightplan/FlightPlanEventHandlerCollection.h"
#include "tag/TagItemCollection.h"
#include "wake/WakeModule.h"
#include "wake/WakeSchemeCollection.h"

using ::testing::NiceMock;
using ::testing::Test;
using UKControllerPlugin::Bootstrap::PersistenceContainer;
using UKControllerPlugin::Flightplan::FlightPlanEventHandlerCollection;
using UKControllerPlugin::Tag::TagItemCollection;
using UKControllerPlugin::Wake::BootstrapPlugin;
using UKControllerPluginTest::Dependency::MockDependencyLoader;

namespace UKControllerPluginTest::Wake {

    class WakeModuleTest : public Test
    {
        public:
        WakeModuleTest()
        {
            container.flightplanHandler = std::make_unique<FlightPlanEventHandlerCollection>();
            container.tagHandler = std::make_unique<TagItemCollection>();
        }

        PersistenceContainer container;
        NiceMock<MockDependencyLoader> dependencies;
    };

    TEST_F(WakeModuleTest, ItRegistersTheSchemeCollection)
    {
        EXPECT_CALL(dependencies, LoadDependency(testing::_, nlohmann::json::object()))
            .Times(2)
            .WillRepeatedly(testing::Return(nlohmann::json::object()));

        EXPECT_CALL(dependencies, LoadDependency("DEPENDENCY_WAKE_SCHEME", nlohmann::json::array()))
            .Times(1)
            .WillOnce(testing::Return(nlohmann::json::array()));

        BootstrapPlugin(this->container, this->dependencies);

        EXPECT_EQ(0, this->container.wakeSchemes->Count());
    }

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
} // namespace UKControllerPluginTest::Wake
