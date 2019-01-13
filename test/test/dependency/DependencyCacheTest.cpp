#include "pch/pch.h"
#include "dependency/DependencyCache.h"
#include "json/json.hpp"

using UKControllerPlugin::Dependency::DependencyCache;

namespace UKControllerPluginTest {
    namespace Dependency {

        TEST(DependencyCache, AddDependencyThrowsExceptionIfAlreadySet)
        {
            DependencyCache dependency;
            dependency.AddDependency("Test", "Test");
            EXPECT_THROW(dependency.AddDependency("Test", "Test2"), std::invalid_argument);
        }

        TEST(DependencyCache, GetDependencyThrowsExceptionIfNotSet)
        {
            DependencyCache dependency;
            EXPECT_THROW(dependency.GetDependency("Test"), std::out_of_range);
        }

        TEST(DependencyCache, AddSetDependencyAllowsSingleDependency)
        {
            DependencyCache dependency;
            dependency.AddDependency("Test", "Test2");
            EXPECT_EQ(0, dependency.GetDependency("Test").compare("Test2"));
        }

        TEST(DependencyCache, AddSetDependencyAllowsMultipleDifferentDependencies)
        {
            DependencyCache dependency;
            dependency.AddDependency("Test1", "Test2");
            dependency.AddDependency("Test3", "Test4");
            EXPECT_EQ(0, dependency.GetDependency("Test1").compare("Test2"));
            EXPECT_EQ(0, dependency.GetDependency("Test3").compare("Test4"));
        }

        TEST(DependencyCache, HasDependencyReturnsTrueIfExists)
        {
            DependencyCache dependency;
            dependency.AddDependency("Test", "Test");
            EXPECT_TRUE(dependency.HasDependency("Test"));
        }

        TEST(DependencyCache, HasDependencyReturnsFalseIfNotExists)
        {
            DependencyCache dependency;
            EXPECT_FALSE(dependency.HasDependency("Test"));
        }

        TEST(DependencyCache, DependencyCountReturnsNumberInCache)
        {
            DependencyCache dependency;
            EXPECT_EQ(0, dependency.DependencyCount());
            dependency.AddDependency("Test1", "Test1");
            EXPECT_EQ(1, dependency.DependencyCount());
            dependency.AddDependency("Test2", "Test2");
            EXPECT_EQ(2, dependency.DependencyCount());
            dependency.AddDependency("Test3", "Test3");
            EXPECT_EQ(3, dependency.DependencyCount());
        }

    }  // namespace Dependency
}  // namespace UKControllerPluginTest
