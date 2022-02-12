#include "bootstrap/PersistenceContainer.h"
#include "runway/RunwayCollection.h"
#include "runway/RunwayModule.h"

using UKControllerPlugin::Bootstrap::PersistenceContainer;
using UKControllerPlugin::Runway::BootstrapPlugin;

namespace UKControllerPluginTest::Runway {
    class RunwayModuleTest : public testing::Test
    {
        public:
        RunwayModuleTest()
        {
        }

        testing::NiceMock<Dependency::MockDependencyLoader> dependencyLoader;
        PersistenceContainer container;
    };

    TEST_F(RunwayModuleTest, BootstrapPluginCreatesRunwayCollection)
    {
        EXPECT_CALL(dependencyLoader, LoadDependency("DEPENDENCY_RUNWAYS", nlohmann::json::array()))
            .Times(1)
            .WillOnce(testing::Return(nlohmann::json::array()));

        BootstrapPlugin(container, dependencyLoader);
        EXPECT_EQ(0, container.runwayCollection->Count());
    }
} // namespace UKControllerPluginTest::Runway
