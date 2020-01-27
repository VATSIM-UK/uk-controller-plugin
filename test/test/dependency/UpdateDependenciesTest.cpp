#include "pch/pch.h"
#include "dependency/UpdateDependencies.h"
#include "mock/MockWinApi.h"
#include "mock/MockApiInterface.h"
#include "api/ApiException.h"

using UKControllerPluginTest::Api::MockApiInterface;
using UKControllerPluginTest::Windows::MockWinApi;
using UKControllerPlugin::Dependency::UpdateDependencies;
using UKControllerPlugin::Dependency::ValidDependency;
using UKControllerPlugin::Api::ApiException;
using ::testing::NiceMock;
using ::testing::Test;
using ::testing::Throw;
using ::testing::Return;
using ::testing::_;

namespace UKControllerPluginTest {
    namespace Dependency {

        class UpdateDependenciesTest : public Test
        {
            public:

                UpdateDependenciesTest()
                {

                }

                NiceMock<MockWinApi> mockWindows;
                NiceMock<MockApiInterface> mockApi;


                nlohmann::json dependency1 = {
                    { "foo", "bar "}
                };

                nlohmann::json dependency2 = {
                    { "baz", "noot "}
                };

        };

        TEST_F(UpdateDependenciesTest, ValidDependencyReturnsTrueOnValid)
        {
            nlohmann::json data;
            data["key"] = "DEPENDENCY_ONE";
            data["local_file"] = "test.json";
            data["uri"] = "test.json";

            EXPECT_TRUE(ValidDependency(data));
        }

        TEST_F(UpdateDependenciesTest, ValidDependencyReturnsFalseOnNotObject)
        {
            nlohmann::json data;

            EXPECT_FALSE(ValidDependency(data));
        }

        TEST_F(UpdateDependenciesTest, ValidDependencyReturnsFalseOnKeyMissing)
        {
            nlohmann::json data;
            data["local_file"] = "test.json";
            data["uri"] = "test.json";

            EXPECT_FALSE(ValidDependency(data));
        }

        TEST_F(UpdateDependenciesTest, ValidDependencyReturnsFalseOnKeyNotString)
        {
            nlohmann::json data;
            data["key"] = 123;
            data["local_file"] = "test.json";
            data["uri"] = "test.json";

            EXPECT_FALSE(ValidDependency(data));
        }

        TEST_F(UpdateDependenciesTest, ValidDependencyReturnsFalseOnLocalFileMissing)
        {
            nlohmann::json data;
            data["key"] = "DEPENDENCY_ONE";
            data["uri"] = "test.json";

            EXPECT_FALSE(ValidDependency(data));
        }

        TEST_F(UpdateDependenciesTest, ValidDependencyReturnsFalseOnLocalFileNotString)
        {
            nlohmann::json data;
            data["key"] = "DEPENDENCY_ONE";
            data["local_file"] = 1;
            data["uri"] = "test.json";

            EXPECT_FALSE(ValidDependency(data));
        }

        TEST_F(UpdateDependenciesTest, ValidDependencyReturnsFalseOnUriMissing)
        {
            nlohmann::json data;
            data["key"] = "DEPENDENCY_ONE";
            data["local_file"] = "test.json";

            EXPECT_FALSE(ValidDependency(data));
        }

        TEST_F(UpdateDependenciesTest, ValidDependencyReturnsFalseOnUriNotString)
        {
            nlohmann::json data;
            data["key"] = "DEPENDENCY_ONE";
            data["local_file"] = "test.json";
            data["uri"] = 123;

            EXPECT_FALSE(ValidDependency(data));
        }

        TEST_F(UpdateDependenciesTest, UpdateDependenciesStopsIfListFailsDownload)
        {
            ON_CALL(this->mockApi, GetDependencyList())
                .WillByDefault(Throw(ApiException("nah")));

            EXPECT_CALL(this->mockWindows, WriteToFile(_, _, _))
                .Times(0);

            UpdateDependencies(this->mockApi, this->mockWindows);
        }

        TEST_F(UpdateDependenciesTest, UpdateDependenciesUpdatesData)
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

            ON_CALL(this->mockApi, GetDependencyList())
                .WillByDefault(Return(dependencyList));

            ON_CALL(this->mockApi, GetUri("test1"))
                .WillByDefault(Return(this->dependency1));

            ON_CALL(this->mockApi, GetUri("test2"))
                .WillByDefault(Return(this->dependency2));


            EXPECT_CALL(
                this->mockWindows,
                WriteToFile("dependencies/dependency-list.json", dependencyList.dump(), true)
            )
                .Times(1);

            EXPECT_CALL(this->mockWindows, WriteToFile("dependencies/test1.json", this->dependency1.dump(), true))
                .Times(1);

            EXPECT_CALL(this->mockWindows, WriteToFile("dependencies/test2.json", this->dependency2.dump(), true))
                .Times(1);

            UpdateDependencies(this->mockApi, this->mockWindows);
        }

        TEST_F(UpdateDependenciesTest, UpdateDependenciesHandlesApiExceptionsOnDependencies)
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

            ON_CALL(this->mockApi, GetDependencyList())
                .WillByDefault(Return(dependencyList));

            ON_CALL(this->mockApi, GetUri("test1"))
                .WillByDefault(Throw(ApiException("nah")));

            ON_CALL(this->mockApi, GetUri("test2"))
                .WillByDefault(Throw(ApiException("nah")));


            EXPECT_CALL(
                this->mockWindows,
                WriteToFile("dependencies/dependency-list.json", dependencyList.dump(), true)
            )
                .Times(1);

            EXPECT_CALL(
                this->mockWindows,
                WriteToFile("dependencies/test1.json", this->dependency1.dump(), true)
            )
                .Times(0);

            EXPECT_CALL(this->mockWindows, WriteToFile("dependencies/test2.json", this->dependency2.dump(), true))
                .Times(0);

            UpdateDependencies(this->mockApi, this->mockWindows);
        }

        TEST_F(UpdateDependenciesTest, UpdateDependenciesHandlesInvalidDependencies)
        {
            nlohmann::json dependencyList{
                    {
                        {"key", "DEPENDENCY_ONE"},
                        {"local_file", 1},
                        {"uri", "test1"},
                    },
                    {
                        {"key", "DEPENDENCY_TWO"},
                        {"local_file", 1},
                        {"uri", "test2"},
                    }
            };

            ON_CALL(this->mockApi, GetDependencyList())
                .WillByDefault(Return(dependencyList));

            EXPECT_CALL(this->mockApi, GetUri("test1"))
                .Times(0);

            EXPECT_CALL(this->mockApi, GetUri("test2"))
                .Times(0);

            EXPECT_CALL(
                this->mockWindows,
                WriteToFile("dependencies/dependency-list.json", dependencyList.dump(), true)
            )
                .Times(1);

            UpdateDependencies(this->mockApi, this->mockWindows);
        }
    }  // namespace Dependency
}  // namespace UKControllerPluginTest
