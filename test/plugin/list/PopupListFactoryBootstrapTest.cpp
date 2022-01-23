#include "bootstrap/PersistenceContainer.h"
#include "list/PopupListFactoryBootstrap.h"

using UKControllerPlugin::Bootstrap::PersistenceContainer;
using UKControllerPlugin::List::BootstrapPlugin;

namespace UKControllerPluginTest::List {
    class PopupListFactoryBootstrapTest : public testing::Test
    {
        public:
        PersistenceContainer container;
    };

    TEST_F(PopupListFactoryBootstrapTest, ItSetsFactoryOnContainer)
    {
        BootstrapPlugin(container);
        EXPECT_NE(nullptr, container.popupListFactory);
    }
} // namespace UKControllerPluginTest::List
