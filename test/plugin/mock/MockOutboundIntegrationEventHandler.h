#pragma once
#include "integration/OutboundIntegrationEventHandler.h"

using UKControllerPlugin::Integration::OutboundIntegrationEventHandler;

namespace UKControllerPluginTest::Integration {

    class MockOutboundIntegrationEventHandler : public OutboundIntegrationEventHandler
    {
        public:
        MockOutboundIntegrationEventHandler();
        virtual ~MockOutboundIntegrationEventHandler();
        MOCK_METHOD(
            void, SendEvent, (std::shared_ptr<UKControllerPlugin::Integration::MessageInterface>), (const, override));
    };
} // namespace UKControllerPluginTest::Integration
