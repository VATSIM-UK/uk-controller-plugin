#include "aircraft/AircraftModule.h"
#include "aircraft/AircraftTypeCollection.h"
#include "bootstrap/PersistenceContainer.h"

using UKControllerPlugin::Aircraft::BootstrapPlugin;
using UKControllerPlugin::Bootstrap::PersistenceContainer;

namespace UKControllerPluginTest::Wake {

    class AircraftModuleTest : public testing::Test
    {
        public:
        PersistenceContainer container;
        testing::NiceMock<Dependency::MockDependencyLoader> dependencies;
    };

    TEST_F(AircraftModuleTest, ItSetsUpAircraftTypeCollection)
    {
        EXPECT_CALL(dependencies, LoadDependency("DEPENDENCY_AIRCRAFT", nlohmann::json::array()))
            .Times(1)
            .WillOnce(testing::Return(nlohmann::json::array()));

        BootstrapPlugin(container, dependencies);
        EXPECT_EQ(0, container.aircraftTypes->Count());
    }

    TEST_F(AircraftModuleTest, ItCreatesAircraftTypeMapper)
    {
        BootstrapPlugin(container, dependencies);
        EXPECT_NE(nullptr, container.aircraftTypeMapper);
    }
} // namespace UKControllerPluginTest::Wake
