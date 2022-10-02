#pragma once
#include "bootstrap/PersistenceContainer.h"

namespace UKControllerPlugin {
    namespace Integration {

        void BootstrapPlugin(Bootstrap::PersistenceContainer& container, bool duplicatePlugin, bool winsockInitialised);
    } // namespace Integration
} // namespace UKControllerPlugin
