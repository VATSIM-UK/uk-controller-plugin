#pragma once
#include "bootstrap/PersistenceContainer.h"

namespace UKControllerPlugin {
    namespace Push {

        void BootstrapPlugin(
            Bootstrap::PersistenceContainer& container,
            bool duplicatePlugin
        );

    } // namespace Push
}  // namespace UKControllerPlugin
