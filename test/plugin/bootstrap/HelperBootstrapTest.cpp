#include "api/ApiInterface.h"
#include "bootstrap/HelperBootstrap.h"
#include "bootstrap/PersistenceContainer.h"
#include "plugin/FunctionCallEventHandler.h"
#include "radarscreen/ConfigurableDisplayCollection.h"
#include "setting/SettingRepository.h"
#include "task/TaskRunnerInterface.h"

using ::testing::NiceMock;
using ::testing::Return;
using ::testing::Test;
using UKControllerPlugin::Bootstrap::HelperBootstrap;
using UKControllerPlugin::Bootstrap::PersistenceContainer;
using UKControllerPlugin::Plugin::FunctionCallEventHandler;
using UKControllerPlugin::RadarScreen::ConfigurableDisplayCollection;
using UKControllerPluginTest::Windows::MockWinApi;

namespace UKControllerPluginTest::Bootstrap {

    class HelperBootstrapTest : public Test
    {
        public:
        void SetUp() override
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
        EXPECT_CALL(*this->mockWinApi, FileExists(std::wstring(L"settings/release-channel.json")))
            .Times(1)
            .WillOnce(Return(false));

        EXPECT_CALL(*this->mockWinApi, FileExists(std::wstring(L"settings/api-settings.json")))
            .Times(1)
            .WillOnce(Return(true));

        EXPECT_CALL(*this->mockWinApi, ReadFromFileMock(std::wstring(L"settings/api-settings.json"), true))
            .Times(1)
            .WillOnce(Return("{\"api-url\": \"testurl\", \"api-key\": \"testkey\"}"));
        this->container.windows = std::move(this->mockWinApi);

        HelperBootstrap::Bootstrap(container);
        EXPECT_TRUE("testurl" == this->container.api->GetApiDomain());
    }

    TEST_F(HelperBootstrapTest, BootstrapCreatesSettingsRepository)
    {
        EXPECT_CALL(*this->mockWinApi, FileExists(std::wstring(L"settings/release-channel.json")))
            .Times(1)
            .WillOnce(Return(false));

        EXPECT_CALL(*this->mockWinApi, FileExists(std::wstring(L"settings/api-settings.json")))
            .Times(1)
            .WillOnce(Return(true));

        EXPECT_CALL(*this->mockWinApi, ReadFromFileMock(std::wstring(L"settings/api-settings.json"), true))
            .Times(1)
            .WillOnce(Return("{\"api-url\": \"testurl\", \"api-key\": \"testkey\"}"));
        this->container.windows = std::move(this->mockWinApi);

        HelperBootstrap::Bootstrap(container);
        EXPECT_EQ("testkey", this->container.settingsRepository->GetSetting("api-key"));
    }

    TEST_F(HelperBootstrapTest, BootstrapCreatesTaskRunner)
    {
        EXPECT_CALL(*this->mockWinApi, FileExists(std::wstring(L"settings/release-channel.json")))
            .Times(1)
            .WillOnce(Return(false));

        EXPECT_CALL(*this->mockWinApi, FileExists(std::wstring(L"settings/api-settings.json")))
            .Times(1)
            .WillOnce(Return(true));

        EXPECT_CALL(*this->mockWinApi, ReadFromFileMock(std::wstring(L"settings/api-settings.json"), true))
            .Times(1)
            .WillOnce(Return("{\"api-key\": \"testkey\", \"api-url\": \"testurl\"}"));
        this->container.windows = std::move(this->mockWinApi);

        HelperBootstrap::Bootstrap(this->container);
        EXPECT_EQ(3, this->container.taskRunner->CountThreads());
    }

    TEST_F(HelperBootstrapTest, BootstrapApiConfigurationItemAddsToConfigurables)
    {
        this->container.windows = std::move(this->mockWinApi);
        HelperBootstrap::BootstrapApiConfigurationItem(this->container, this->configurables);

        EXPECT_EQ(1, this->configurables.CountDisplays());
    }

    TEST_F(HelperBootstrapTest, BootstrapApiConfigurationItemAddsCallbackFunctions)
    {
        this->container.windows = std::move(this->mockWinApi);
        HelperBootstrap::BootstrapApiConfigurationItem(this->container, this->configurables);

        EXPECT_EQ(1, this->container.pluginFunctionHandlers->CountCallbacks());
        EXPECT_TRUE(this->container.pluginFunctionHandlers->HasCallbackFunction(5000));
    }
} // namespace UKControllerPluginTest::Bootstrap
