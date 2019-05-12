#include "pch/pch.h"
#include "bootstrap/HelperBootstrap.h"
#include "bootstrap/PersistenceContainer.h"
#include "mock/MockWinApi.h"
#include "radarscreen/ConfigurableDisplayCollection.h"
#include "plugin/FunctionCallEventHandler.h"

using UKControllerPlugin::Bootstrap::HelperBootstrap;
using UKControllerPlugin::Bootstrap::PersistenceContainer;
using UKControllerPlugin::Plugin::FunctionCallEventHandler;
using UKControllerPluginTest::Windows::MockWinApi;
using UKControllerPlugin::RadarScreen::ConfigurableDisplayCollection;
using ::testing::Return;
using ::testing::NiceMock;
using ::testing::Test;

namespace UKControllerPluginTest {
    namespace Bootstrap {

        class HelperBootstrapTest : public Test
        {
            public:

                void SetUp(void)
                {
                    this->mockWinApi = std::make_unique<NiceMock<MockWinApi>>();
                    this->container.pluginFunctionHandlers = std::make_unique<FunctionCallEventHandler>();
                }

                ConfigurableDisplayCollection configurables;
                PersistenceContainer container;
                std::unique_ptr<NiceMock<MockWinApi>> mockWinApi;
        };

        TEST_F(HelperBootstrapTest, BootstrapCreatesApiHelper)
        {
            EXPECT_CALL(*this->mockWinApi, FileExists("settings/api-settings.json"))
                .Times(1)
                .WillOnce(Return(true));

            EXPECT_CALL(*this->mockWinApi, ReadFromFileMock("settings/api-settings.json", true))
                .Times(1)
                .WillOnce(Return("{\"api-url\": \"testurl\", \"api-key\": \"testkey\"}"));
            this->container.windows = std::move(this->mockWinApi);

            HelperBootstrap::Bootstrap(container);
            EXPECT_TRUE("testurl" == this->container.api->GetApiDomain());
        }

        TEST_F(HelperBootstrapTest, BootstrapCreatesSettingsRepository)
        {
            EXPECT_CALL(*this->mockWinApi, FileExists("settings/api-settings.json"))
                .Times(1)
                .WillOnce(Return(true));

            EXPECT_CALL(*this->mockWinApi, ReadFromFileMock("settings/api-settings.json", true))
                .Times(1)
                .WillOnce(Return("{\"api-url\": \"testurl\", \"api-key\": \"testkey\"}"));
            this->container.windows = std::move(this->mockWinApi);

            HelperBootstrap::Bootstrap(container);
            EXPECT_NO_THROW(this->container.settingsRepository->GetSetting("foo"));
        }

        TEST_F(HelperBootstrapTest, BootstrapCreatesTaskRunner)
        {
            EXPECT_CALL(*this->mockWinApi, FileExists("settings/api-settings.json"))
                .Times(1)
                .WillOnce(Return(true));

            EXPECT_CALL(*this->mockWinApi, ReadFromFileMock("settings/api-settings.json", true))
                .Times(1)
                .WillOnce(Return("{\"api-key\": \"testkey\", \"api-url\": \"testurl\"}"));
            this->container.windows = std::move(this->mockWinApi);

            HelperBootstrap::Bootstrap(this->container);
            EXPECT_NO_THROW(this->container.taskRunner->QueueInlineTask([]() {
                bool test = true;
            }));
        }

        TEST_F(HelperBootstrapTest, BootstrapApiConfigurationItemAddsToConfigurables)
        {
            this->container.windows = std::move(this->mockWinApi);
            HelperBootstrap::BootstrapApiConfigurationItem(
                this->container,
                this->configurables
            );

            EXPECT_EQ(1, this->configurables.CountDisplays());
        }

        TEST_F(HelperBootstrapTest, BootstrapApiConfigurationItemAddsCallbackFunctions)
        {
            this->container.windows = std::move(this->mockWinApi);
            HelperBootstrap::BootstrapApiConfigurationItem(
                this->container,
                this->configurables
            );

            EXPECT_EQ(1, this->container.pluginFunctionHandlers->CountCallbacks());
            EXPECT_TRUE(this->container.pluginFunctionHandlers->HasCallbackFunction(5000));
        }
    }  // namespace Bootstrap
}  // namespace UKControllerPluginTest
