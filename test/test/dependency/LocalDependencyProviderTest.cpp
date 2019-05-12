#include "pch/pch.h"
#include "dependency/LocalDependencyProvider.h"
#include "mock/MockWinApi.h"
#include "dependency/DependencyData.h"

using UKControllerPlugin::Dependency::LocalDependencyProvider;
using UKControllerPluginTest::Windows::MockWinApi;
using UKControllerPlugin::Dependency::DependencyData;
using ::testing::Test;
using ::testing::NiceMock;
using ::testing::Return;

namespace UKControllerPluginTest {
    namespace Dependency {

        class LocalDependencyProviderTest : public Test
        {
            public:
                LocalDependencyProviderTest()
                    : provider(mockWinApi)
                {

                }

                DependencyData dependency{ "localpath", "remotepath", nlohmann::json("flubadubdub") };
                NiceMock<MockWinApi> mockWinApi;
                LocalDependencyProvider provider;
        };

        TEST_F(LocalDependencyProviderTest, ItHasAProviderType)
        {
            EXPECT_EQ("LocalDependencyProvider", provider.GetProviderType());
        }

        TEST_F(LocalDependencyProviderTest, ReturnsDefaultIfNoFileExists)
        {
            ON_CALL(this->mockWinApi, FileExists("localpath"))
                .WillByDefault(Return(false));

            nlohmann::json expected("flubadubdub");
            EXPECT_EQ(expected, provider.GetDependency(dependency));
        }

        TEST_F(LocalDependencyProviderTest, ReturnsWindowsDependencyIfValid)
        {
            ON_CALL(this->mockWinApi, FileExists("localpath"))
                .WillByDefault(Return(true));

            ON_CALL(this->mockWinApi, ReadFromFileMock("localpath", true))
                .WillByDefault(Return("[1, 2, 3]"));

            nlohmann::json expected({ 1, 2, 3 });
            EXPECT_EQ(expected, provider.GetDependency(dependency));
        }

        TEST_F(LocalDependencyProviderTest, ReturnsDefaultIfInvalidJson)
        {
            ON_CALL(this->mockWinApi, FileExists("localpath"))
                .WillByDefault(Return(true));

            ON_CALL(this->mockWinApi, ReadFromFileMock("localpath", true))
                .WillByDefault(Return("{]"));

            nlohmann::json expected("flubadubdub");
            EXPECT_EQ(expected, provider.GetDependency(dependency));
        }
    }  // namespace Dependency
}  // namespace UKControllerPluginTest
