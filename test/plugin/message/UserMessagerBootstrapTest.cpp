#include "message/UserMessagerBootstrap.h"
#include "bootstrap/PersistenceContainer.h"

using UKControllerPlugin::Bootstrap::PersistenceContainer;
using UKControllerPlugin::Message::UserMessagerBootstrap;
using UKControllerPluginTest::Euroscope::MockEuroscopePluginLoopbackInterface;

namespace UKControllerPluginTest {
    namespace Message {

        TEST(UserMessagerBootstrap, BootstrapPluginSetsUpMessager)
        {
            PersistenceContainer container;
            EXPECT_NO_THROW(UserMessagerBootstrap::BootstrapPlugin(container));
        }
    } // namespace Message
} // namespace UKControllerPluginTest
