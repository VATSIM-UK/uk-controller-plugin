#include "airfield/AirfieldModule.h"
#include "airfield/AirfieldCollection.h"
#include "bootstrap/PersistenceContainer.h"

using UKControllerPlugin::Airfield::BootstrapPlugin;
using UKControllerPlugin::Bootstrap::PersistenceContainer;

namespace UKControllerPluginTest::Airfield {
    class AirfieldModuleTest : public testing::Test
    {
        public:
        testing::NiceMock<Dependency::MockDependencyLoader> dependencies;
        PersistenceContainer container;
    };

    TEST_F(AirfieldModuleTest, ItLoadsAirfieldCollection)
    {
        EXPECT_CALL(dependencies, LoadDependency("DEPENDENCY_AIRFIELD", nlohmann::json::array()))
            .Times(1)
            .WillOnce(testing::Return(nlohmann::json::array()));
        BootstrapPlugin(container, dependencies);

        EXPECT_EQ(0, container.airfields->GetSize());
    }
} // namespace UKControllerPluginTest::Airfield
