#include "bootstrap/PersistenceContainer.h"
#include "sid/SidCollection.h"
#include "sid/SidModule.h"

using testing::NiceMock;
using testing::Return;
using ::testing::Test;
using UKControllerPlugin::Bootstrap::PersistenceContainer;
using UKControllerPlugin::Sid::BootstrapPlugin;
using UKControllerPluginTest::Dependency::MockDependencyLoader;

namespace UKControllerPluginTest::Sid {

    class SidModuleTest : public Test
    {
        public:
        NiceMock<MockDependencyLoader> dependencyLoader;
        PersistenceContainer container;
    };

    TEST_F(SidModuleTest, ItCreatesSidCollection)
    {
        nlohmann::json sidData = nlohmann::json::array(
            {nlohmann::json::object({
                 {"airfield", "EGGD"},
                 {"identifier", "TEST1Y"},
                 {"initial_altitude", 6000},
                 {"initial_heading", 350},
                 {"handoff", 5},
                 {"prenotes", nlohmann::json::array({1, 2})},
             }),
             nlohmann::json::object(
                 {{"airfield", "EGGD"},
                  {"identifier", "TEST1Y"},
                  {"initial_altitude", 6000},
                  {"initial_heading", nlohmann::json::value_t::null},
                  {"handoff", 5},
                  {"prenotes", nlohmann::json::array({1, 2})}})});

        EXPECT_CALL(dependencyLoader, LoadDependency("DEPENDENCY_SIDS", nlohmann::json::array()))
            .Times(1)
            .WillOnce(Return(sidData));

        BootstrapPlugin(container, dependencyLoader);
        EXPECT_EQ(2, container.sids->CountSids());
    }
} // namespace UKControllerPluginTest::Sid
