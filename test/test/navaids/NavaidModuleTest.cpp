#include "pch/pch.h"
#include "navaids/NavaidModule.h"
#include "bootstrap/PersistenceContainer.h"
#include "mock/MockDependencyLoader.h"

using UKControllerPlugin::Bootstrap::PersistenceContainer;
using UKControllerPlugin::Navaids::BootstrapPlugin;
using UKControllerPlugin::Navaids::NavaidValid;
using UKControllerPluginTest::Dependency::MockDependencyLoader;
using ::testing::Test;
using ::testing::NiceMock;
using ::testing::Return;

namespace UKControllerPluginTest {
    namespace Navaids {

        class NavaidModuleTest : public Test
        {
            public:
                NiceMock<MockDependencyLoader> dependency;
                PersistenceContainer container;
        };

        TEST_F(NavaidModuleTest, NavaidValidReturnsTrueIfValid)
        {
            nlohmann::json data = {
                {"id", 1},
                {"identifier", "TIMBA"}
            };
            EXPECT_TRUE(NavaidValid(data));
        }

        TEST_F(NavaidModuleTest, NavaidValidReturnsFalseIfIdNotInteger)
        {
            nlohmann::json data = {
                {"id", "test"},
                {"identifier", "TIMBA"}
            };
            EXPECT_FALSE(NavaidValid(data));
        }

        TEST_F(NavaidModuleTest, NavaidValidReturnsFalseIfNoId)
        {
            nlohmann::json data = {
                {"identifier", "TIMBA"}
            };
            EXPECT_FALSE(NavaidValid(data));
        }

        TEST_F(NavaidModuleTest, NavaidValidReturnsFalseIfIdentifierNotString)
        {
            nlohmann::json data = {
                {"id", 1},
                {"identifier", 1}
            };
            EXPECT_FALSE(NavaidValid(data));
        }

        TEST_F(NavaidModuleTest, NavaidValidReturnsFalseIfIdentifierMissing)
        {
            nlohmann::json data = {
                {"id", 1},
            };
            EXPECT_FALSE(NavaidValid(data));
        }

        TEST_F(NavaidModuleTest, NavaidValidReturnsFalseIfNotObject)
        {
            EXPECT_FALSE(NavaidValid(nlohmann::json::array()));
        }

        TEST_F(NavaidModuleTest, BootstrapPluginHandlesInvalidDependency)
        {
            ON_CALL(this->dependency, LoadDependency("DEPENDENCY_NAVAIDS", "{}"_json))
                .WillByDefault(Return(nlohmann::json::object()));

            BootstrapPlugin(this->container, this->dependency);
            EXPECT_EQ(0, this->container.navaids->Count());
        }

        TEST_F(NavaidModuleTest, BootstrapPluginHandlesInvalidNavaid)
        {
            nlohmann::json data = nlohmann::json::array();
            data.push_back(
                {
                    {"id", "nah"}
                }
            );

            ON_CALL(this->dependency, LoadDependency("DEPENDENCY_NAVAIDS", "{}"_json))
                .WillByDefault(Return(data));

            BootstrapPlugin(this->container, this->dependency);
            EXPECT_EQ(0, this->container.navaids->Count());
        }

        TEST_F(NavaidModuleTest, BootstrapPluginLoadsNavaids)
        {
            nlohmann::json data = nlohmann::json::array();
            data.push_back(
                {
                    {"id", 1},
                    {"identifier", "TIMBA"}
                }
            );
            data.push_back(
                {
                    {"id", 2},
                    {"identifier", "WILLO"}
                }
            );

            ON_CALL(this->dependency, LoadDependency("DEPENDENCY_NAVAIDS", "{}"_json))
                .WillByDefault(Return(data));

            BootstrapPlugin(this->container, this->dependency);
            EXPECT_EQ(2, this->container.navaids->Count());
            EXPECT_EQ("TIMBA", this->container.navaids->GetByIdentifier("TIMBA").identifier);
            EXPECT_EQ("WILLO", this->container.navaids->GetByIdentifier("WILLO").identifier);
        }
    }  // namespace Navaids
}  // namespace UKControllerPluginTest
