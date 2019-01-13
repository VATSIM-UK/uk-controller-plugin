#include "pch/pch.h"
#include "bootstrap/HelperBootstrap.h"
#include "bootstrap/PersistenceContainer.h"
#include "mock/MockWinApi.h"

using UKControllerPlugin::Bootstrap::HelperBootstrap;
using UKControllerPlugin::Bootstrap::PersistenceContainer;
using UKControllerPluginTest::Windows::MockWinApi;
using ::testing::Return;
using ::testing::StrictMock;

namespace UKControllerPluginTest {
    namespace Bootstrap {

        TEST(HelperBootstrap, BootstrapCreatesApiHelper)
        {
            PersistenceContainer container;

            std::unique_ptr<StrictMock<MockWinApi>> mockWinApi = std::make_unique<StrictMock<MockWinApi>>();

            EXPECT_CALL(*mockWinApi, FileExists("settings/api-settings.json"))
                .Times(1)
                .WillOnce(Return(true));

            EXPECT_CALL(*mockWinApi, ReadFromFileMock("settings/api-settings.json", true))
                .Times(1)
                .WillOnce(Return("{\"api-url\": \"testurl\", \"api-key\": \"testkey\"}"));
            container.windows = std::move(mockWinApi);

            HelperBootstrap::Bootstrap(container);
            EXPECT_TRUE("testurl" == container.api->GetApiDomain());
        }

        TEST(HelperBootstrap, BootstrapCreatesTaskRunner)
        {
            PersistenceContainer container;

            std::unique_ptr<StrictMock<MockWinApi>> mockWinApi = std::make_unique<StrictMock<MockWinApi>>();

            EXPECT_CALL(*mockWinApi, FileExists("settings/api-settings.json"))
                .Times(1)
                .WillOnce(Return(true));

            EXPECT_CALL(*mockWinApi, ReadFromFileMock("settings/api-settings.json", true))
                .Times(1)
                .WillOnce(Return("{\"api-key\": \"testkey\", \"api-url\": \"testurl\"}"));
            container.windows = std::move(mockWinApi);

            HelperBootstrap::Bootstrap(container);
            EXPECT_NO_THROW(container.taskRunner->QueueInlineTask([]() {
                bool test = true;
            }));
        }
    }  // namespace Bootstrap
}  // namespace UKControllerPluginTest
