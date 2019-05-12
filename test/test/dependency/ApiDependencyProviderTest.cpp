#include "pch/pch.h"
#include "dependency/ApiDependencyProvider.h"
#include "mock/MockWinApi.h"
#include "dependency/DependencyData.h"
#include "mock/MockApiInterface.h"
#include "api/ApiException.h"

using UKControllerPlugin::Dependency::ApiDependencyProvider;
using UKControllerPluginTest::Windows::MockWinApi;
using UKControllerPluginTest::Api::MockApiInterface;
using UKControllerPlugin::Dependency::DependencyData;
using UKControllerPlugin::Api::ApiException;
using ::testing::Test;
using ::testing::NiceMock;
using ::testing::Return;
using ::testing::Throw;
using ::testing::_;

namespace UKControllerPluginTest {
    namespace Dependency {

        class ApiDependencyProviderTest : public Test
        {
            public:
                ApiDependencyProviderTest()
                    : provider(mockWebApi, mockWinApi)
                {

                }

                DependencyData dependency{ "localpath", "remotepath", nlohmann::json("flubadubdub") };
                NiceMock<MockWinApi> mockWinApi;
                NiceMock<MockApiInterface> mockWebApi;
                ApiDependencyProvider provider;
        };

        TEST_F(ApiDependencyProviderTest, ItHasAProviderType)
        {
            EXPECT_EQ("ApiDependencyProvider", provider.GetProviderType());
        }

        TEST_F(ApiDependencyProviderTest, ItWritesDefaultValueIfFileDoesNotExist)
        {
            ON_CALL(this->mockWinApi, FileExists("localpath"))
                .WillByDefault(Return(false));

            EXPECT_CALL(this->mockWinApi, WriteToFile("localpath", dependency.defaultValue.dump(), true))
                .Times(1);

            ON_CALL(this->mockWebApi, GetDependency(_))
                .WillByDefault(Throw(ApiException("Test")));

            provider.GetDependency(dependency);
        }

        TEST_F(ApiDependencyProviderTest, ItReturnsDefaultOnApiExceptionAndLocalInvalid)
        {
            ON_CALL(this->mockWinApi, FileExists("localpath"))
                .WillByDefault(Return(true));

            ON_CALL(this->mockWinApi, ReadFromFileMock("localpath", true))
                .WillByDefault(Return("{]"));

            ON_CALL(this->mockWebApi, GetDependency(_))
                .WillByDefault(Throw(ApiException("Test")));

            EXPECT_EQ(dependency.defaultValue, provider.GetDependency(dependency));
        }

        TEST_F(ApiDependencyProviderTest, ItReturnsLocalCopyOnApiExpcetion)
        {
            nlohmann::json expected;
            expected["abc"] = "123";

            ON_CALL(this->mockWinApi, FileExists("localpath"))
                .WillByDefault(Return(true));

            ON_CALL(this->mockWinApi, ReadFromFileMock("localpath", true))
                .WillByDefault(Return(expected.dump()));

            ON_CALL(this->mockWebApi, GetDependency(_))
                .WillByDefault(Throw(ApiException("Test")));

            EXPECT_EQ(expected, provider.GetDependency(dependency));
        }

        TEST_F(ApiDependencyProviderTest, ItWritesApiResponseToFile)
        {
            nlohmann::json apiData;
            apiData["foo"] = "bar";

            ON_CALL(this->mockWinApi, FileExists("localpath"))
                .WillByDefault(Return(true));

            ON_CALL(this->mockWebApi, GetDependency(dependency))
                .WillByDefault(Return(apiData));

            EXPECT_CALL(this->mockWinApi, WriteToFile("localpath", apiData.dump(), true))
                .Times(1);

            provider.GetDependency(dependency);
        }

        TEST_F(ApiDependencyProviderTest, ItReturnsApiResponseWhenGiven)
        {
            nlohmann::json apiData;
            apiData["foo"] = "bar";

            ON_CALL(this->mockWinApi, FileExists("localpath"))
                .WillByDefault(Return(true));

            ON_CALL(this->mockWebApi, GetDependency(dependency))
                .WillByDefault(Return(apiData));

            EXPECT_EQ(apiData, provider.GetDependency(dependency));
        }
    }  // namespace Dependency
}  // namespace UKControllerPluginTest
