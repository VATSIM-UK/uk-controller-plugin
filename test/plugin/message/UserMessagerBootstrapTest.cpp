#include "pch/pch.h"
#include "message/UserMessagerBootstrap.h"
#include "mock/MockEuroscopePluginLoopbackInterface.h"
#include "bootstrap/PersistenceContainer.h"

using UKControllerPlugin::Bootstrap::PersistenceContainer;
using UKControllerPluginTest::Euroscope::MockEuroscopePluginLoopbackInterface;
using UKControllerPlugin::Message::UserMessagerBootstrap;

namespace UKControllerPluginTest {
    namespace Message {

        TEST(UserMessagerBootstrap, BootstrapPluginSetsUpMessager)
        {
            PersistenceContainer container;
            EXPECT_NO_THROW(UserMessagerBootstrap::BootstrapPlugin(container));
        }
    }  // namespace Message
}  // namespace UKControllerPluginTest
