#include "api/BootstrapApi.h"
#include "api/ApiKeyReceivedEvent.h"
#include "api/ReplaceApiKeyDialog.h"
#include "bootstrap/PersistenceContainer.h"
#include "dialog/DialogManager.h"
#include "mock/MockDialogProvider.h"
#include "plugin/FunctionCallEventHandler.h"
#include "radarscreen/ConfigurableDisplayCollection.h"
#include "setting/SettingRepository.h"
#include "test/ApiTestCase.h"
#include "test/EventBusTestCase.h"
#include <gmock/gmock-nice-strict.h>
#include <gtest/internal/gtest-internal.h>
#include <memory>

using UKControllerPlugin::Api::BootstrapApi;
using UKControllerPlugin::Api::BootstrapConfigurationMenuItem;
using UKControllerPlugin::Bootstrap::PersistenceContainer;
using UKControllerPlugin::Plugin::FunctionCallEventHandler;
using UKControllerPlugin::RadarScreen::ConfigurableDisplayCollection;
using UKControllerPlugin::Setting::SettingRepository;

namespace UKControllerPluginTest::Api {
    class BootstrapApiTest : public ApiTestCase, public UKControllerPluginUtilsTest::EventBusTestCase
    {
        public:
        void SetUp() override
        {
            ApiTestCase::SetUp();
            EventBusTestCase::SetUp();
        }

        void TearDown() override
        {
            ApiTestCase::TearDown();
            EventBusTestCase::TearDown();
        }

        BootstrapApiTest() : ApiTestCase(), EventBusTestCase()
        {
            container.windows = std::make_unique<testing::NiceMock<Windows::MockWinApi>>();
            container.settingsRepository = std::make_unique<SettingRepository>();
            container.pluginFunctionHandlers = std::make_unique<FunctionCallEventHandler>();
            container.dialogManager = std::make_unique<UKControllerPlugin::Dialog::DialogManager>(dialogProvider);
        }

        testing::NiceMock<UKControllerPluginTest::Dialog::MockDialogProvider> dialogProvider;
        ConfigurableDisplayCollection configurableDisplays;
        PersistenceContainer container;
    };

    TEST_F(BootstrapApiTest, ItBootstrapsTheApiFactory)
    {
        BootstrapApi(container);
        EXPECT_NE(nullptr, container.apiFactory);
    }

    TEST_F(BootstrapApiTest, ItBootstrapsTheLegacyApiInterface)
    {
        BootstrapApi(container);
        EXPECT_NE(nullptr, container.api);
    }

    TEST_F(BootstrapApiTest, ItBootstrapsTheReplaceDialog)
    {
        BootstrapApi(container);
        EXPECT_EQ(1, container.dialogManager->CountDialogs());
        EXPECT_TRUE(container.dialogManager->HasDialog(IDD_API_KEY_REPLACE));
        AssertHandlerRegisteredForEvent<
            UKControllerPlugin::Api::ReplaceApiKeyDialog,
            UKControllerPluginUtils::Api::ApiKeyReceivedEvent>(
            UKControllerPluginUtils::EventHandler::EventHandlerFlags::Async);
    }

    TEST_F(BootstrapApiTest, ItBootstrapsTheConfigurationMenuItem)
    {
        BootstrapApi(container);
        BootstrapConfigurationMenuItem(container, configurableDisplays);
        EXPECT_EQ(1, this->configurableDisplays.CountDisplays());
        EXPECT_TRUE(container.pluginFunctionHandlers->HasCallbackByDescription("API Configuration Menu Item Selected"));
    }
} // namespace UKControllerPluginTest::Api
