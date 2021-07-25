#pragma once
#include "pch/pch.h"
#include "integration/OutboundIntegrationEventHandler.h"

using UKControllerPlugin::Integration::OutboundIntegrationEventHandler;

namespace UKControllerPluginTest::Integration {

    class MockOutboundIntegrationEventHandler : public OutboundIntegrationEventHandler
    {
        public:
            MOCK_METHOD(
                void,
                SendEvent, (std::shared_ptr<UKControllerPlugin::Integration::MessageInterface>),
                (override)
            );
    };

}  // namespace UKControllerPluginTest::Integration
