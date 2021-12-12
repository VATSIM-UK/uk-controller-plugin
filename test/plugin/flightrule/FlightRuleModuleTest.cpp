#include "bootstrap/PersistenceContainer.h"
#include "flightrule/FlightRuleCollection.h"
#include "flightrule/FlightRuleModule.h"

using ::testing::NiceMock;
using ::testing::Test;
using UKControllerPlugin::Bootstrap::PersistenceContainer;
using UKControllerPlugin::FlightRules::BootstrapPlugin;
using UKControllerPluginTest::Dependency::MockDependencyLoader;

namespace UKControllerPluginTest::Wake {

    class FlightRuleModuleTest : public Test
    {
        public:
        FlightRuleModuleTest()
        {
        }

        PersistenceContainer container;
        NiceMock<MockDependencyLoader> dependencies;
    };

    TEST_F(FlightRuleModuleTest, ItCreatesFlightRuleCollection)
    {
        EXPECT_CALL(dependencies, LoadDependency("DEPENDENCY_FLIGHT_RULES", nlohmann::json::array()))
            .Times(1)
            .WillOnce(testing::Return(nlohmann::json::array()));
        BootstrapPlugin(container, dependencies);

        EXPECT_EQ(0, container.flightRules->Count());
    }
} // namespace UKControllerPluginTest::Wake
