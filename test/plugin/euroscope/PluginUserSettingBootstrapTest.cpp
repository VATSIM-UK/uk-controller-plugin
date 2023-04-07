#include "bootstrap/PersistenceContainer.h"
#include "euroscope/PluginUserSettingBootstrap.h"

using UKControllerPlugin::Bootstrap::PersistenceContainer;
using UKControllerPlugin::Euroscope::PluginUserSettingBootstrap;

namespace UKControllerPluginTest::Euroscope {
    class PluginUserSettingBootstrapTest : public testing::Test
    {
        public:
        PersistenceContainer container;
    };

    TEST_F(PluginUserSettingBootstrapTest, ItSetsThePluginLevelSettingsHandler)
    {
        PluginUserSettingBootstrap::BootstrapPlugin(container);
        EXPECT_NE(nullptr, container.pluginUserSettingHandler);
    }

    TEST_F(PluginUserSettingBootstrapTest, ItSetsThePluginSettingsProviderCollection)
    {
        PluginUserSettingBootstrap::BootstrapPlugin(container);
        EXPECT_NE(nullptr, container.pluginSettingsProviders);
    }
} // namespace UKControllerPluginTest::Euroscope
