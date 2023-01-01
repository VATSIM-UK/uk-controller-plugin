#pragma once
#include "bootstrap/BootstrapProviderInterface.h"

namespace UKControllerPlugin::IntentionCode {

    class IntentionCodeBootstrapProvider : public Bootstrap::BootstrapProviderInterface
    {
        public:
        void BootstrapPlugin(Bootstrap::PersistenceContainer& container) override;

        private:
        static const int intentionCodeTagItemId = 101;
    };

} // namespace UKControllerPlugin::IntentionCode
