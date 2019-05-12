#include "pch/pch.h"
#include "dependency/DependencyData.h"

using UKControllerPlugin::Dependency::DependencyData;

namespace UKControllerPluginTest {
    namespace Dependency {

        TEST(DependencyDataTest, EqualityReturnsTrueIfEqual)
        {
            DependencyData dependency1 = { "local", "remote", nlohmann::json("flubadubdub") };
            DependencyData dependency2 = { "local", "remote", nlohmann::json("flubadubdub") };
            EXPECT_TRUE(dependency1 == dependency2);
        }

        TEST(DependencyDataTest, EqualityReturnsFalseDifferentLocalPaths)
        {
            DependencyData dependency1 = { "local", "remote", nlohmann::json("flubadubdub") };
            DependencyData dependency2 = { "notlocal", "remote", nlohmann::json("flubadubdub") };
            EXPECT_FALSE(dependency1 == dependency2);
        }

        TEST(DependencyDataTest, EqualityReturnsFalseDifferentRemotePaths)
        {
            DependencyData dependency1 = { "local", "remote", nlohmann::json("flubadubdub") };
            DependencyData dependency2 = { "local", "notremote", nlohmann::json("flubadubdub") };
            EXPECT_FALSE(dependency1 == dependency2);
        }

        TEST(DependencyDataTest, EqualityReturnsFalseDifferentDefaults)
        {
            DependencyData dependency1 = { "local", "remote", nlohmann::json("flubadubdub") };
            DependencyData dependency2 = { "local", "remote", nlohmann::json("flubadoodoo") };
            EXPECT_FALSE(dependency1 == dependency2);
        }
    }  // namespace Dependency
}  // namespace UKControllerPluginTest
