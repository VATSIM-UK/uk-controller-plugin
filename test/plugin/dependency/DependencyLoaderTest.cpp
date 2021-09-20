#include "api/ApiException.h"
#include "dependency/DependencyLoader.h"

using ::testing::_;
using ::testing::NiceMock;
using ::testing::Return;
using ::testing::Test;
using ::testing::Throw;
using UKControllerPlugin::Api::ApiException;
using UKControllerPlugin::Dependency::DependencyLoader;
using UKControllerPluginTest::Windows::MockWinApi;

namespace UKControllerPluginTest {
    namespace Dependency {

        class DependencyLoaderTest : public Test
        {
            public:
            DependencyLoaderTest()
            {
            }

            NiceMock<MockWinApi> mockWindows;

            nlohmann::json dependency1 = {{"foo", "bar "}};

            nlohmann::json dependency2 = {{"baz", "noot "}};
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
                }};

            ON_CALL(this->mockWindows, FileExists(std::wstring(L"dependencies/dependency-list.json")))
                .WillByDefault(Return(true));

            ON_CALL(this->mockWindows, ReadFromFileMock(std::wstring(L"dependencies/dependency-list.json"), true))
                .WillByDefault(Return(dependencyList.dump()));

            ON_CALL(this->mockWindows, FileExists(std::wstring(L"dependencies/test1.json")))
                .WillByDefault(Return(true));

            ON_CALL(this->mockWindows, ReadFromFileMock(std::wstring(L"dependencies/test1.json"), true))
                .WillByDefault(Return(this->dependency1.dump()));

            ON_CALL(this->mockWindows, FileExists(std::wstring(L"dependencies/test2.json")))
                .WillByDefault(Return(true));

            ON_CALL(this->mockWindows, ReadFromFileMock(std::wstring(L"dependencies/test2.json"), true))
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
                }};

            ON_CALL(this->mockWindows, FileExists(std::wstring(L"dependencies/dependency-list.json")))
                .WillByDefault(Return(false));

            ON_CALL(this->mockWindows, ReadFromFileMock(std::wstring(L"dependencies/dependency-list.json"), true))
                .WillByDefault(Return(dependencyList.dump()));

            ON_CALL(this->mockWindows, FileExists(std::wstring(L"dependencies/test1.json")))
                .WillByDefault(Return(true));

            ON_CALL(this->mockWindows, ReadFromFileMock(std::wstring(L"dependencies/test1.json"), true))
                .WillByDefault(Return(this->dependency1.dump()));

            ON_CALL(this->mockWindows, FileExists(std::wstring(L"dependencies/test2.json")))
                .WillByDefault(Return(true));

            ON_CALL(this->mockWindows, ReadFromFileMock(std::wstring(L"dependencies/test2.json"), true))
                .WillByDefault(Return(this->dependency2.dump()));

            DependencyLoader loader(this->mockWindows);
            EXPECT_EQ("{}", loader.LoadDependency("DEPENDENCY_ONE", "{}"));
            EXPECT_EQ("{}", loader.LoadDependency("DEPENDENCY_TWO", "{}"));
        }

        TEST_F(DependencyLoaderTest, ItDoesntLoadIfDependencyListInvalid)
        {
            ON_CALL(this->mockWindows, FileExists(std::wstring(L"dependencies/dependency-list.json")))
                .WillByDefault(Return(true));

            ON_CALL(this->mockWindows, ReadFromFileMock(std::wstring(L"dependencies/dependency-list.json"), true))
                .WillByDefault(Return("{]"));

            ON_CALL(this->mockWindows, FileExists(std::wstring(L"dependencies/test1.json")))
                .WillByDefault(Return(true));

            ON_CALL(this->mockWindows, ReadFromFileMock(std::wstring(L"dependencies/test1.json"), true))
                .WillByDefault(Return(this->dependency1.dump()));

            ON_CALL(this->mockWindows, FileExists(std::wstring(L"dependencies/test2.json")))
                .WillByDefault(Return(true));

            ON_CALL(this->mockWindows, ReadFromFileMock(std::wstring(L"dependencies/test2.json"), true))
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
                }};

            ON_CALL(this->mockWindows, FileExists(std::wstring(L"dependencies/dependency-list.json")))
                .WillByDefault(Return(true));

            ON_CALL(this->mockWindows, ReadFromFileMock(std::wstring(L"dependencies/dependency-list.json"), true))
                .WillByDefault(Return(dependencyList.dump()));

            ON_CALL(this->mockWindows, FileExists(std::wstring(L"dependencies/test1.json")))
                .WillByDefault(Return(true));

            ON_CALL(this->mockWindows, ReadFromFileMock(std::wstring(L"dependencies/test1.json"), true))
                .WillByDefault(Return(this->dependency1.dump()));

            ON_CALL(this->mockWindows, FileExists(std::wstring(L"dependencies/test2.json")))
                .WillByDefault(Return(true));

            ON_CALL(this->mockWindows, ReadFromFileMock(std::wstring(L"dependencies/test2.json"), true))
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
                }};

            ON_CALL(this->mockWindows, FileExists(std::wstring(L"dependencies/dependency-list.json")))
                .WillByDefault(Return(true));

            ON_CALL(this->mockWindows, ReadFromFileMock(std::wstring(L"dependencies/dependency-list.json"), true))
                .WillByDefault(Return(dependencyList.dump()));

            ON_CALL(this->mockWindows, FileExists(std::wstring(L"dependencies/test1.json")))
                .WillByDefault(Return(true));

            ON_CALL(this->mockWindows, ReadFromFileMock(std::wstring(L"dependencies/test1.json"), true))
                .WillByDefault(Return(this->dependency1.dump()));

            ON_CALL(this->mockWindows, FileExists(std::wstring(L"dependencies/test2.json")))
                .WillByDefault(Return(true));

            ON_CALL(this->mockWindows, ReadFromFileMock(std::wstring(L"dependencies/test2.json"), true))
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
                }};

            ON_CALL(this->mockWindows, FileExists(std::wstring(L"dependencies/dependency-list.json")))
                .WillByDefault(Return(true));

            ON_CALL(this->mockWindows, ReadFromFileMock(std::wstring(L"dependencies/dependency-list.json"), true))
                .WillByDefault(Return(dependencyList.dump()));

            ON_CALL(this->mockWindows, FileExists(std::wstring(L"dependencies/test1.json")))
                .WillByDefault(Return(false));

            ON_CALL(this->mockWindows, ReadFromFileMock(std::wstring(L"dependencies/test1.json"), true))
                .WillByDefault(Return(this->dependency1.dump()));

            ON_CALL(this->mockWindows, FileExists(std::wstring(L"dependencies/test2.json")))
                .WillByDefault(Return(true));

            ON_CALL(this->mockWindows, ReadFromFileMock(std::wstring(L"dependencies/test2.json"), true))
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
                }};

            ON_CALL(this->mockWindows, FileExists(std::wstring(L"dependencies/dependency-list.json")))
                .WillByDefault(Return(true));

            ON_CALL(this->mockWindows, ReadFromFileMock(std::wstring(L"dependencies/dependency-list.json"), true))
                .WillByDefault(Return(dependencyList.dump()));

            ON_CALL(this->mockWindows, FileExists(std::wstring(L"dependencies/test1.json")))
                .WillByDefault(Return(true));

            ON_CALL(this->mockWindows, ReadFromFileMock(std::wstring(L"dependencies/test1.json"), true))
                .WillByDefault(Return("{]"));

            ON_CALL(this->mockWindows, FileExists(std::wstring(L"dependencies/test2.json")))
                .WillByDefault(Return(true));

            ON_CALL(this->mockWindows, ReadFromFileMock(std::wstring(L"dependencies/test2.json"), true))
                .WillByDefault(Return(this->dependency2.dump()));

            DependencyLoader loader(this->mockWindows);
            EXPECT_EQ("{}", loader.LoadDependency("DEPENDENCY_ONE", "{}"));
        }
    } // namespace Dependency
} // namespace UKControllerPluginTest
