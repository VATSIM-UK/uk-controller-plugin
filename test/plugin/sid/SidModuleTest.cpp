#include "pch/pch.h"
#include "sid/SidModule.h"
#include "bootstrap/PersistenceContainer.h"
#include "mock/MockDependencyLoader.h"

using ::testing::Test;
using testing::NiceMock;
using testing::Return;
using UKControllerPlugin::Sid::BootstrapPlugin;
using UKControllerPlugin::Bootstrap::PersistenceContainer;
using UKControllerPluginTest::Dependency::MockDependencyLoader;

namespace UKControllerPluginTest {
    namespace Sid {

        class SidModuleTest : public Test
        {
            public:
                NiceMock<MockDependencyLoader> dependencyLoader;
                PersistenceContainer container;
        };

        TEST_F(SidModuleTest, ItCreatesSidCollection)
        {
            nlohmann::json sidData = nlohmann::json::array({
                nlohmann::json::object({
                    {"airfield", "EGGD"},
                    {"identifier", "TEST1Y"},
                    {"initial_altitude", 6000},
                    {"initial_heading", 350},
                }),
                nlohmann::json::object({
                    {"airfield", "EGGD"},
                    {"identifier", "TEST1Y"},
                    {"initial_altitude", 6000},
                    {"initial_heading", nlohmann::json::value_t::null},
                })
            });

            EXPECT_CALL(dependencyLoader, LoadDependency("DEPENDENCY_SIDS", nlohmann::json::array()))
                .Times(1)
            .WillOnce(Return(sidData));

            BootstrapPlugin(container, dependencyLoader);
            EXPECT_EQ(2, container.sids->CountSids());
        }
    }  // namespace Sid
}  // namespace UKControllerPluginTest
