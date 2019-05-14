#include "pch/pch.h"
#include "log/LoggerBootstrap.h"
#include "bootstrap/PersistenceContainer.h"
#include "mock/MockWinApi.h"

using UKControllerPluginTest::Windows::MockWinApi;
using UKControllerPlugin::Bootstrap::PersistenceContainer;
using UKControllerPlugin::Log::LoggerBootstrap;
using testing::Test;
using testing::NiceMock;
using testing::Return;

namespace UKControllerPluginTest {
    namespace Log {

        class LoggerBootstrapTest : public Test
        {
            public:
                PersistenceContainer container;
        };

        TEST_F(LoggerBootstrapTest, ItThrowsExceptionIfItCannotCreateMockFolder)
        {
            std::unique_ptr<NiceMock<MockWinApi>> mockWindows(new NiceMock<MockWinApi>);

            ON_CALL(*mockWindows, CreateLocalFolderRecursive)
                .WillByDefault(Return(false));

            container.windows = std::move(mockWindows);

            EXPECT_THROW(LoggerBootstrap::Bootstrap(container, false), std::runtime_error);
        }

    }  // namespace Log
}  // namespace UKControllerPluginTest
