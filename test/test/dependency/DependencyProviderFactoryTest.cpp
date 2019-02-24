#include "pch/pch.h"
#include "dependency/DependencyProviderFactory.h"
#include "mock/MockWinApi.h"
#include "mock/MockApiInterface.h"

using UKControllerPlugin::Dependency::GetDependencyProvider;
using UKControllerPluginTest::Api::MockApiInterface;
using UKControllerPluginTest::Windows::MockWinApi;
using ::testing::Test;
using ::testing::NiceMock;

namespace UKControllerPluginTest {
    namespace Dependency {

        class DependencyProviderFactoryTest : public Test
        {
            public:
                NiceMock<MockWinApi> mockWindowsApi;
                NiceMock<MockApiInterface> mockWebApi;
        };

        TEST_F(DependencyProviderFactoryTest, ItReturnsLocalProviderIfApiNotAuthorised)
        {
            std::string type = GetDependencyProvider(this->mockWebApi, this->mockWindowsApi, false)->GetProviderType();
            EXPECT_TRUE("LocalDependencyProvider" == type);
        }

        TEST_F(DependencyProviderFactoryTest, ItReturnsApiProviderIfApiAuthorised)
        {
            std::string type = GetDependencyProvider(this->mockWebApi, this->mockWindowsApi, true)->GetProviderType();
            EXPECT_TRUE("ApiDependencyProvider" == type);
        }
    }  // namespace Dependency
}  // namespace UKControllerPluginTest
