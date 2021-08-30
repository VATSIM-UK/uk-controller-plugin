#include "UserMessagerBootstrap.h"
#include "UserMessager.h"
#include "bootstrap/PersistenceContainer.h"
#include "plugin/UKPlugin.h"

using UKControllerPlugin::Bootstrap::PersistenceContainer;

namespace UKControllerPlugin::Message {

    void UserMessagerBootstrap::BootstrapPlugin(PersistenceContainer& container)
    {
        container.userMessager = std::make_unique<UserMessager>(*container.plugin);
    }
} // namespace UKControllerPlugin::Message
