#include "pch/pch.h"
#include "message/UserMessagerBootstrap.h"
#include "bootstrap/PersistenceContainer.h"

using UKControllerPlugin::Bootstrap::PersistenceContainer;

namespace UKControllerPlugin {
    namespace Message {

        void UserMessagerBootstrap::BootstrapPlugin(PersistenceContainer & container)
        {
            container.userMessager.reset(new UserMessager(*container.plugin));
        }
    }  // namespace Message
}  // namespace UKControllerPlugin
