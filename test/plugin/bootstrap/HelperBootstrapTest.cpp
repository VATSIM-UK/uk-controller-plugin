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

    TEST_F(HelperBootstrapTest, BootstrapCreatesSettingsRepository)
    {
        HelperBootstrap::Bootstrap(this->container);
        EXPECT_NE(nullptr, this->container.settingsRepository);
    }

    TEST_F(HelperBootstrapTest, BootstrapCreatesTaskRunner)
    {
        HelperBootstrap::Bootstrap(this->container);
        EXPECT_EQ(3, this->container.taskRunner->CountThreads());
    }
} // namespace UKControllerPluginTest::Bootstrap
