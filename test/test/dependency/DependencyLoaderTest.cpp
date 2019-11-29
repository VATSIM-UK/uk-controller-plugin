#include "pch/pch.h"
#include "dependency/DependencyLoader.h"
#include "mock/MockWinApi.h"
#include "api/ApiException.h"

using UKControllerPluginTest::Windows::MockWinApi;
using UKControllerPlugin::Dependency::DependencyLoader;
using UKControllerPlugin::Api::ApiException;
using ::testing::NiceMock;
using ::testing::Test;
using ::testing::Throw;
using ::testing::Return;
using ::testing::_;

namespace UKControllerPluginTest {
    namespace Dependency {

        class DependencyLoaderTest : public Test
        {
            public:

            DependencyLoaderTest()
            {

            }

            NiceMock<MockWinApi> mockWindows;

            nlohmann::json dependency1 = {
                { "foo", "bar "}
            };

            nlohmann::json dependency2 = {
                { "baz", "noot "}
            };

        };

        TEST_F(DependencyLoaderTest, ItLoadsDependencies)
        {
            nlohmann::json dependencyList{
                {
                    {"key", "DEPENDENCY_ONE"},
                    {"local_file", "test1.json"},
                    {"uri", "test1"},
                },
                {
                    {"key", "DEPENDENCY_TWO"},
                    {"local_file", "test2.json"},
                    {"uri", "test2"},
                }
            };

            ON_CALL(this->mockWindows, FileExists("dependencies/dependency-list.json"))
                .WillByDefault(Return(true));

            ON_CALL(this->mockWindows, ReadFromFileMock("dependencies/dependency-list.json", true))
                .WillByDefault(Return(dependencyList.dump()));

            ON_CALL(this->mockWindows, FileExists("dependencies/test1.json"))
                .WillByDefault(Return(true));

            ON_CALL(this->mockWindows, ReadFromFileMock("dependencies/test1.json", true))
                .WillByDefault(Return(this->dependency1.dump()));

            ON_CALL(this->mockWindows, FileExists("dependencies/test2.json"))
                .WillByDefault(Return(true));

            ON_CALL(this->mockWindows, ReadFromFileMock("dependencies/test2.json", true))
                .WillByDefault(Return(this->dependency2.dump()));


            DependencyLoader loader(this->mockWindows);
            EXPECT_EQ(this->dependency1, loader.LoadDependency("DEPENDENCY_ONE", "{}"));
            EXPECT_EQ(this->dependency2, loader.LoadDependency("DEPENDENCY_TWO", "{}"));
        }   

        TEST_F(DependencyLoaderTest, ItDoesntLoadIfNoDependencyList)
        {
            nlohmann::json dependencyList{
                {
                    {"key", "DEPENDENCY_ONE"},
                    {"local_file", "test1.json"},
                    {"uri", "test1"},
                },
                {
                    {"key", "DEPENDENCY_TWO"},
                    {"local_file", "test2.json"},
                    {"uri", "test2"},
                }
            };

            ON_CALL(this->mockWindows, FileExists("dependencies/dependency-list.json"))
                .WillByDefault(Return(false));

            ON_CALL(this->mockWindows, ReadFromFileMock("dependencies/dependency-list.json", true))
                .WillByDefault(Return(dependencyList.dump()));

            ON_CALL(this->mockWindows, FileExists("dependencies/test1.json"))
                .WillByDefault(Return(true));

            ON_CALL(this->mockWindows, ReadFromFileMock("dependencies/test1.json", true))
                .WillByDefault(Return(this->dependency1.dump()));

            ON_CALL(this->mockWindows, FileExists("dependencies/test2.json"))
                .WillByDefault(Return(true));

            ON_CALL(this->mockWindows, ReadFromFileMock("dependencies/test2.json", true))
                .WillByDefault(Return(this->dependency2.dump()));


            DependencyLoader loader(this->mockWindows);
            EXPECT_EQ("{}", loader.LoadDependency("DEPENDENCY_ONE", "{}"));
            EXPECT_EQ("{}", loader.LoadDependency("DEPENDENCY_TWO", "{}"));
        }

        TEST_F(DependencyLoaderTest, ItDoesntLoadIfDependencyListInvalid)
        {
            ON_CALL(this->mockWindows, FileExists("dependencies/dependency-list.json"))
                .WillByDefault(Return(true));

            ON_CALL(this->mockWindows, ReadFromFileMock("dependencies/dependency-list.json", true))
                .WillByDefault(Return("{]"));

            ON_CALL(this->mockWindows, FileExists("dependencies/test1.json"))
                .WillByDefault(Return(true));

            ON_CALL(this->mockWindows, ReadFromFileMock("dependencies/test1.json", true))
                .WillByDefault(Return(this->dependency1.dump()));

            ON_CALL(this->mockWindows, FileExists("dependencies/test2.json"))
                .WillByDefault(Return(true));

            ON_CALL(this->mockWindows, ReadFromFileMock("dependencies/test2.json", true))
                .WillByDefault(Return(this->dependency2.dump()));


            DependencyLoader loader(this->mockWindows);
            EXPECT_EQ("{}", loader.LoadDependency("DEPENDENCY_ONE", "{}"));
            EXPECT_EQ("{}", loader.LoadDependency("DEPENDENCY_TWO", "{}"));
        }

        TEST_F(DependencyLoaderTest, ItDoesntLoadIfInvalidDependencies)
        {
            nlohmann::json dependencyList{
                {
                    {"key", "DEPENDENCY_ONE"},
                    {"local_file", 2},
                    {"uri", "test1"},
                },
                {
                    {"key", "DEPENDENCY_TWO"},
                    {"local_file", 2},
                    {"uri", "test2"},
                }
            };

            ON_CALL(this->mockWindows, FileExists("dependencies/dependency-list.json"))
                .WillByDefault(Return(true));

            ON_CALL(this->mockWindows, ReadFromFileMock("dependencies/dependency-list.json", true))
                .WillByDefault(Return(dependencyList.dump()));

            ON_CALL(this->mockWindows, FileExists("dependencies/test1.json"))
                .WillByDefault(Return(true));

            ON_CALL(this->mockWindows, ReadFromFileMock("dependencies/test1.json", true))
                .WillByDefault(Return(this->dependency1.dump()));

            ON_CALL(this->mockWindows, FileExists("dependencies/test2.json"))
                .WillByDefault(Return(true));

            ON_CALL(this->mockWindows, ReadFromFileMock("dependencies/test2.json", true))
                .WillByDefault(Return(this->dependency2.dump()));


            DependencyLoader loader(this->mockWindows);
            EXPECT_EQ("{}", loader.LoadDependency("DEPENDENCY_ONE", "{}"));
            EXPECT_EQ("{}", loader.LoadDependency("DEPENDENCY_TWO", "{}"));
        }

        TEST_F(DependencyLoaderTest, ItReturnsDefaultIfInvalidDependency)
        {
            nlohmann::json dependencyList{
                {
                    {"key", "DEPENDENCY_ONE"},
                    {"local_file", "test1.json"},
                    {"uri", "test1"},
                },
                {
                    {"key", "DEPENDENCY_TWO"},
                    {"local_file", "test2.json"},
                    {"uri", "test2"},
                }
            };

            ON_CALL(this->mockWindows, FileExists("dependencies/dependency-list.json"))
                .WillByDefault(Return(true));

            ON_CALL(this->mockWindows, ReadFromFileMock("dependencies/dependency-list.json", true))
                .WillByDefault(Return(dependencyList.dump()));

            ON_CALL(this->mockWindows, FileExists("dependencies/test1.json"))
                .WillByDefault(Return(true));

            ON_CALL(this->mockWindows, ReadFromFileMock("dependencies/test1.json", true))
                .WillByDefault(Return(this->dependency1.dump()));

            ON_CALL(this->mockWindows, FileExists("dependencies/test2.json"))
                .WillByDefault(Return(true));

            ON_CALL(this->mockWindows, ReadFromFileMock("dependencies/test2.json", true))
                .WillByDefault(Return(this->dependency2.dump()));


            DependencyLoader loader(this->mockWindows);
            EXPECT_EQ("{}", loader.LoadDependency("DEPENDENCY_THREE", "{}"));
        }

        TEST_F(DependencyLoaderTest, ItReturnsDefaultIfDependencyDoesNotExist)
        {
            nlohmann::json dependencyList{
                {
                    {"key", "DEPENDENCY_ONE"},
                    {"local_file", "test1.json"},
                    {"uri", "test1"},
                },
                {
                    {"key", "DEPENDENCY_TWO"},
                    {"local_file", "test2.json"},
                    {"uri", "test2"},
                }
            };

            ON_CALL(this->mockWindows, FileExists("dependencies/dependency-list.json"))
                .WillByDefault(Return(true));

            ON_CALL(this->mockWindows, ReadFromFileMock("dependencies/dependency-list.json", true))
                .WillByDefault(Return(dependencyList.dump()));

            ON_CALL(this->mockWindows, FileExists("dependencies/test1.json"))
                .WillByDefault(Return(false));

            ON_CALL(this->mockWindows, ReadFromFileMock("dependencies/test1.json", true))
                .WillByDefault(Return(this->dependency1.dump()));

            ON_CALL(this->mockWindows, FileExists("dependencies/test2.json"))
                .WillByDefault(Return(true));

            ON_CALL(this->mockWindows, ReadFromFileMock("dependencies/test2.json", true))
                .WillByDefault(Return(this->dependency2.dump()));


            DependencyLoader loader(this->mockWindows);
            EXPECT_EQ("{}", loader.LoadDependency("DEPENDENCY_ONE", "{}"));
        }

        TEST_F(DependencyLoaderTest, ItReturnsDefaultIfDependencyNotJson)
        {
            nlohmann::json dependencyList{
                {
                    {"key", "DEPENDENCY_ONE"},
                    {"local_file", "test1.json"},
                    {"uri", "test1"},
                },
                {
                    {"key", "DEPENDENCY_TWO"},
                    {"local_file", "test2.json"},
                    {"uri", "test2"},
                }
            };

            ON_CALL(this->mockWindows, FileExists("dependencies/dependency-list.json"))
                .WillByDefault(Return(true));

            ON_CALL(this->mockWindows, ReadFromFileMock("dependencies/dependency-list.json", true))
                .WillByDefault(Return(dependencyList.dump()));

            ON_CALL(this->mockWindows, FileExists("dependencies/test1.json"))
                .WillByDefault(Return(true));

            ON_CALL(this->mockWindows, ReadFromFileMock("dependencies/test1.json", true))
                .WillByDefault(Return("{]"));

            ON_CALL(this->mockWindows, FileExists("dependencies/test2.json"))
                .WillByDefault(Return(true));

            ON_CALL(this->mockWindows, ReadFromFileMock("dependencies/test2.json", true))
                .WillByDefault(Return(this->dependency2.dump()));


            DependencyLoader loader(this->mockWindows);
            EXPECT_EQ("{}", loader.LoadDependency("DEPENDENCY_ONE", "{}"));
        }
    }  // namespace Dependency
}  // namespace UKControllerPluginTest
