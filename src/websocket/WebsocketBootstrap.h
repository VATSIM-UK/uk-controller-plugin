#pragma once
#include "bootstrap/PersistenceContainer.h"

namespace UKControllerPlugin {
    namespace Websocket {

        void BootstrapPlugin(
            Bootstrap::PersistenceContainer& container,
            bool duplicatePlugin
        );

    }  // namespace Websocket
}  // namespace UKControllerPlugin
