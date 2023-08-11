#pragma once
#include "bootstrap/BootstrapProviderInterface.h"

namespace UKControllerPlugin::ECFMP {

    class ECFMPBootstrapProvider : public Bootstrap::BootstrapProviderInterface
    {
        public:
        void BootstrapPlugin(Bootstrap::PersistenceContainer& container) override;
    };
} // namespace UKControllerPlugin::ECFMP
