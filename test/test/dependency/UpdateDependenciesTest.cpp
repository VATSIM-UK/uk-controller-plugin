#include "pch/pch.h"
#include "dependency/UpdateDependencies.h"
#include "mock/MockWinApi.h"
#include "mock/MockApiInterface.h"
#include "api/ApiException.h"

using UKControllerPluginTest::Api::MockApiInterface;
using UKControllerPluginTest::Windows::MockWinApi;
using UKControllerPlugin::Dependency::UpdateDependencies;
using UKControllerPlugin::Dependency::ValidDependency;
using UKControllerPlugin::Dependency::NeedsDownload;
using UKControllerPlugin::Dependency::LoadDependencyList;
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
            data["updated_at"] = 123;

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
            data["updated_at"] = 123;

            EXPECT_FALSE(ValidDependency(data));
        }

        TEST_F(UpdateDependenciesTest, ValidDependencyReturnsFalseOnKeyNotString)
        {
            nlohmann::json data;
            data["key"] = 123;
            data["local_file"] = "test.json";
            data["uri"] = "test.json";

            data["updated_at"] = 123;
            EXPECT_FALSE(ValidDependency(data));
        }

        TEST_F(UpdateDependenciesTest, ValidDependencyReturnsFalseOnLocalFileMissing)
        {
            nlohmann::json data;
            data["key"] = "DEPENDENCY_ONE";
            data["uri"] = "test.json";
            data["updated_at"] = 123;

            EXPECT_FALSE(ValidDependency(data));
        }

        TEST_F(UpdateDependenciesTest, ValidDependencyReturnsFalseOnLocalFileNotString)
        {
            nlohmann::json data;
            data["key"] = "DEPENDENCY_ONE";
            data["local_file"] = 1;
            data["uri"] = "test.json";
            data["updated_at"] = 123;

            EXPECT_FALSE(ValidDependency(data));
        }

        TEST_F(UpdateDependenciesTest, ValidDependencyReturnsFalseOnUriMissing)
        {
            nlohmann::json data;
            data["key"] = "DEPENDENCY_ONE";
            data["local_file"] = "test.json";
            data["updated_at"] = 123;

            EXPECT_FALSE(ValidDependency(data));
        }

        TEST_F(UpdateDependenciesTest, ValidDependencyReturnsFalseOnUriNotString)
        {
            nlohmann::json data;
            data["key"] = "DEPENDENCY_ONE";
            data["local_file"] = "test.json";
            data["uri"] = 123;
            data["updated_at"] = 123;

            EXPECT_FALSE(ValidDependency(data));
        }

        TEST_F(UpdateDependenciesTest, ValidDependencyReturnsFalseOnUpdatedAtMissing)
        {
            nlohmann::json data;
            data["key"] = "DEPENDENCY_ONE";
            data["local_file"] = "test.json";
            data["uri"] = "test.json";

            EXPECT_FALSE(ValidDependency(data));
        }

        TEST_F(UpdateDependenciesTest, ValidDependencyReturnsFalseOnUriNotInteger)
        {
            nlohmann::json data;
            data["key"] = "DEPENDENCY_ONE";
            data["local_file"] = "test.json";
            data["uri"] = "test.json";
            data["updated_at"] = "abc";

            EXPECT_FALSE(ValidDependency(data));
        }

        TEST_F(UpdateDependenciesTest, NeedsDownloadReturnsTrueIfNoLocalVersion)
        {
            std::map<std::string, nlohmann::json> localData;
            std::map<std::string, nlohmann::json> remoteData;

            remoteData["DEPENDENCY_ONE"]["updated_at"] = 1;

            EXPECT_TRUE(NeedsDownload(localData, remoteData, "DEPENDENCY_ONE"));
        }

        TEST_F(UpdateDependenciesTest, NeedsDownloadReturnsTrueIfLocalVersionOlderThanRemote)
        {
            std::map<std::string, nlohmann::json> localData;
            std::map<std::string, nlohmann::json> remoteData;

            remoteData["DEPENDENCY_ONE"]["updated_at"] = 1;
            localData["DEPENDENCY_ONE"]["updated_at"] = 0;

            EXPECT_TRUE(NeedsDownload(localData, remoteData, "DEPENDENCY_ONE"));
        }

        TEST_F(UpdateDependenciesTest, NeedsDownloadReturnsFalseIfLocalVersionSameAsRemote)
        {
            std::map<std::string, nlohmann::json> localData;
            std::map<std::string, nlohmann::json> remoteData;

            remoteData["DEPENDENCY_ONE"]["updated_at"] = 1;
            localData["DEPENDENCY_ONE"]["updated_at"] = 1;

            EXPECT_FALSE(NeedsDownload(localData, remoteData, "DEPENDENCY_ONE"));
        }

        TEST_F(UpdateDependenciesTest, NeedsDownloadReturnsFalseIfLocalVersionNewerThanRemote)
        {
            std::map<std::string, nlohmann::json> localData;
            std::map<std::string, nlohmann::json> remoteData;

            remoteData["DEPENDENCY_ONE"]["updated_at"] = 1;
            localData["DEPENDENCY_ONE"]["updated_at"] = 2;

            EXPECT_FALSE(NeedsDownload(localData, remoteData, "DEPENDENCY_ONE"));
        }

        TEST_F(UpdateDependenciesTest, LoadDependencyListReturnsEmptyIfFileNotFound)
        {
            std::map<std::string, nlohmann::json> expected;
            ON_CALL(this->mockWindows, FileExists(std::wstring(L"dependencies/dependency-list.json")))
                .WillByDefault(Return(false));

            EXPECT_EQ(expected, LoadDependencyList(this->mockWindows));
        }

        TEST_F(UpdateDependenciesTest, LoadDependencyListReturnsEmptyIfInvalidJson)
        {
            std::map<std::string, nlohmann::json> expected;
            ON_CALL(this->mockWindows, FileExists(std::wstring(L"dependencies/dependency-list.json")))
                .WillByDefault(Return(true));

            ON_CALL(this->mockWindows, ReadFromFileMock(std::wstring(L"dependencies/dependency-list.json"), true))
                .WillByDefault(Return("{]"));

            EXPECT_EQ(expected, LoadDependencyList(this->mockWindows));
        }

        TEST_F(UpdateDependenciesTest, LoadDependencyListIgnoresInvalidDependencies)
        {
            std::map<std::string, nlohmann::json> expected;
            ON_CALL(this->mockWindows, FileExists(std::wstring(L"dependencies/dependency-list.json")))
                .WillByDefault(Return(true));

            nlohmann::json data;
            data["key"] = "DEPENDENCY_ONE";
            data["local_file"] = "test.json";
            data["uri"] = "test.json";
            data["updated_at"] = "NOT VALID";

            nlohmann::json dependencyList = nlohmann::json::array();
            dependencyList.push_back(data);

            ON_CALL(this->mockWindows, ReadFromFileMock(std::wstring(L"dependencies/dependency-list.json"), true))
                .WillByDefault(Return(dependencyList.dump()));

            EXPECT_EQ(expected, LoadDependencyList(this->mockWindows));
        }

        TEST_F(UpdateDependenciesTest, LoadDependencyListLoadsDependencies)
        {
            ON_CALL(this->mockWindows, FileExists(std::wstring(L"dependencies/dependency-list.json")))
                .WillByDefault(Return(true));

            nlohmann::json data;
            data["key"] = "DEPENDENCY_ONE";
            data["local_file"] = "test.json";
            data["uri"] = "test.json";
            data["updated_at"] = "NOT VALID";

            nlohmann::json data2;
            data2["key"] = "DEPENDENCY_TWO";
            data2["local_file"] = "test.json";
            data2["uri"] = "test.json";
            data2["updated_at"] = 123;

            nlohmann::json dependencyList = nlohmann::json::array();
            dependencyList.push_back(data);
            dependencyList.push_back(data2);

            ON_CALL(this->mockWindows, ReadFromFileMock(std::wstring(L"dependencies/dependency-list.json"), true))
                .WillByDefault(Return(dependencyList.dump()));

            std::map<std::string, nlohmann::json> expected;
            expected["DEPENDENCY_TWO"] = data2;

            EXPECT_EQ(expected, LoadDependencyList(this->mockWindows));
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
            nlohmann::json existingDependencyList{
                {
                    {"key", "DEPENDENCY_ONE"},
                    {"local_file", "test1.json"},
                    {"uri", "test1"},
                    {"updated_at", 1}
                },
                {
                    {"key", "DEPENDENCY_TWO"},
                    {"local_file", "test2.json"},
                    {"uri", "test2"},
                    {"updated_at", 1}
                }
            };

            nlohmann::json dependencyList{
                    {
                        {"key", "DEPENDENCY_ONE"},
                        {"local_file", "test1.json"},
                        {"uri", "test1"},
                        {"updated_at", 2}
                    },
                    {
                        {"key", "DEPENDENCY_TWO"},
                        {"local_file", "test2.json"},
                        {"uri", "test2"},
                        {"updated_at", 2}
                    }
            };

            EXPECT_CALL(this->mockApi, GetDependencyList())
                .WillRepeatedly(Return(dependencyList));

            ON_CALL(this->mockApi, GetUri("test1"))
                .WillByDefault(Return(this->dependency1));

            ON_CALL(this->mockApi, GetUri("test2"))
                .WillByDefault(Return(this->dependency2));

            ON_CALL(this->mockWindows, FileExists(std::wstring(L"dependencies/dependency-list.json")))
                .WillByDefault(Return(true));

            EXPECT_CALL(this->mockWindows, ReadFromFileMock(std::wstring(L"dependencies/dependency-list.json"), true))
                .Times(2)
                .WillOnce(Return(existingDependencyList.dump()))
                .WillOnce(Return(dependencyList.dump()));

            EXPECT_CALL(
                this->mockWindows,
                WriteToFile(std::wstring(L"dependencies/dependency-list.json"), dependencyList.dump(), true)
            )
                .Times(1);

            EXPECT_CALL(
                this->mockWindows,
                WriteToFile(std::wstring(L"dependencies/test1.json"), this->dependency1.dump(), true)
            )
                .Times(1);

            EXPECT_CALL(
                this->mockWindows,
                WriteToFile(std::wstring(L"dependencies/test2.json"), this->dependency2.dump(), true)
            )
                .Times(1);

            UpdateDependencies(this->mockApi, this->mockWindows);
        }

        TEST_F(UpdateDependenciesTest, UpdateDependenciesDoesntUpdateIfUptoDate)
        {
            nlohmann::json existingDependencyList{
                {
                    {"key", "DEPENDENCY_ONE"},
                    {"local_file", "test1.json"},
                    {"uri", "test1"},
                    {"updated_at", 3}
                },
                {
                    {"key", "DEPENDENCY_TWO"},
                    {"local_file", "test2.json"},
                    {"uri", "test2"},
                    {"updated_at", 3}
                }
            };

            nlohmann::json dependencyList{
                    {
                        {"key", "DEPENDENCY_ONE"},
                        {"local_file", "test1.json"},
                        {"uri", "test1"},
                        {"updated_at", 2}
                    },
                    {
                        {"key", "DEPENDENCY_TWO"},
                        {"local_file", "test2.json"},
                        {"uri", "test2"},
                        {"updated_at", 2}
                    }
            };

            EXPECT_CALL(this->mockApi, GetDependencyList())
                .WillRepeatedly(Return(dependencyList));

            EXPECT_CALL(this->mockApi, GetUri("test1"))
                .Times(0);

            EXPECT_CALL(this->mockApi, GetUri("test2"))
                .Times(0);

            ON_CALL(this->mockWindows, FileExists(std::wstring(L"dependencies/dependency-list.json")))
                .WillByDefault(Return(true));

            EXPECT_CALL(this->mockWindows, ReadFromFileMock(std::wstring(L"dependencies/dependency-list.json"), true))
                .Times(2)
                .WillOnce(Return(existingDependencyList.dump()))
                .WillOnce(Return(dependencyList.dump()));

            EXPECT_CALL(
                this->mockWindows,
                WriteToFile(std::wstring(L"dependencies/dependency-list.json"), dependencyList.dump(), true)
            )
                .Times(1);

            UpdateDependencies(this->mockApi, this->mockWindows);
        }

        TEST_F(UpdateDependenciesTest, UpdateDependenciesHandlesApiExceptionsOnDependencies)
        {
            nlohmann::json existingDependencyList{
                {
                    {"key", "DEPENDENCY_ONE"},
                    {"local_file", "test1.json"},
                    {"uri", "test1"},
                    {"updated_at", 1}
                },
                {
                    {"key", "DEPENDENCY_TWO"},
                    {"local_file", "test2.json"},
                    {"uri", "test2"},
                    {"updated_at", 1}
                }
            };

            nlohmann::json dependencyList{
                {
                    {"key", "DEPENDENCY_ONE"},
                    {"local_file", "test1.json"},
                    {"uri", "test1"},
                    {"updated_at", 2}
                },
                {
                    {"key", "DEPENDENCY_TWO"},
                    {"local_file", "test2.json"},
                    {"uri", "test2"},
                    {"updated_at", 2}
                }
            };

            EXPECT_CALL(this->mockApi, GetDependencyList())
                .WillRepeatedly(Return(dependencyList));

            ON_CALL(this->mockApi, GetUri("test1"))
                .WillByDefault(Throw(ApiException("nah")));

            ON_CALL(this->mockApi, GetUri("test2"))
                .WillByDefault(Throw(ApiException("nah")));

            ON_CALL(this->mockWindows, FileExists(std::wstring(L"dependencies/dependency-list.json")))
                .WillByDefault(Return(true));

            EXPECT_CALL(this->mockWindows, ReadFromFileMock(std::wstring(L"dependencies/dependency-list.json"), true))
                .Times(2)
                .WillOnce(Return(existingDependencyList.dump()))
                .WillOnce(Return(dependencyList.dump()));

            EXPECT_CALL(
                this->mockWindows,
                WriteToFile(std::wstring(L"dependencies/dependency-list.json"), dependencyList.dump(), true)
            )
                .Times(1);

            EXPECT_CALL(
                this->mockWindows,
                WriteToFile(std::wstring(L"dependencies/test1.json"), this->dependency1.dump(), true)
            )
                .Times(0);

            EXPECT_CALL(
                this->mockWindows,
                WriteToFile(std::wstring(L"dependencies/test2.json"),
                this->dependency2.dump(),
                true)
            )
                .Times(0);

            UpdateDependencies(this->mockApi, this->mockWindows);
        }

        TEST_F(UpdateDependenciesTest, UpdateDependenciesHandlesInvalidDependencies)
        {
            nlohmann::json existingDependencyList{
                {
                    {"key", "DEPENDENCY_ONE"},
                    {"local_file", "test1.json"},
                    {"updated_at", 1}
                },
                {
                    {"key", "DEPENDENCY_TWO"},
                    {"local_file", "test2.json"},
                    {"updated_at", 1}
                }
            };

            nlohmann::json dependencyList{
                {
                    {"key", "DEPENDENCY_ONE"},
                    {"local_file", "test1.json"},
                    {"updated_at", 2}
                },
                {
                    {"key", "DEPENDENCY_TWO"},
                    {"local_file", "test2.json"},
                    {"updated_at", 2}
                }
            };

            EXPECT_CALL(this->mockApi, GetDependencyList())
                .WillRepeatedly(Return(dependencyList));

            EXPECT_CALL(this->mockApi, GetUri("test1"))
                .Times(0);

            EXPECT_CALL(this->mockApi, GetUri("test2"))
                .Times(0);

            ON_CALL(this->mockWindows, FileExists(std::wstring(L"dependencies/dependency-list.json")))
                .WillByDefault(Return(true));

            EXPECT_CALL(this->mockWindows, ReadFromFileMock(std::wstring(L"dependencies/dependency-list.json"), true))
                .Times(2)
                .WillOnce(Return(existingDependencyList.dump()))
                .WillOnce(Return(dependencyList.dump()));

            EXPECT_CALL(
                this->mockWindows,
                WriteToFile(std::wstring(L"dependencies/dependency-list.json"), dependencyList.dump(), true)
            )
                .Times(1);

            UpdateDependencies(this->mockApi, this->mockWindows);
        }
    }  // namespace Dependency
}  // namespace UKControllerPluginTest
