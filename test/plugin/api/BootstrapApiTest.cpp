#include "api/BootstrapApi.h"
#include "bootstrap/PersistenceContainer.h"
#include "plugin/FunctionCallEventHandler.h"
#include "radarscreen/ConfigurableDisplayCollection.h"
#include "setting/SettingRepository.h"
#include "test/ApiTestCase.h"
#include "test/EventBusTestCase.h"
#include <gtest/internal/gtest-internal.h>

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
        }
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

    TEST_F(BootstrapApiTest, ItBootstrapsTheConfigurationMenuItem)
    {
        BootstrapApi(container);
        BootstrapConfigurationMenuItem(container, configurableDisplays);
        EXPECT_EQ(1, this->configurableDisplays.CountDisplays());
        EXPECT_TRUE(container.pluginFunctionHandlers->HasCallbackByDescription("API Configuration Menu Item Selected"));
    }
} // namespace UKControllerPluginTest::Api
