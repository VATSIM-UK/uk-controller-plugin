#pragma once
#include "integration/InboundIntegrationMessageProcessor.h"
#include "integration/MessageType.h"

using UKControllerPlugin::Integration::InboundIntegrationMessageProcessor;
using UKControllerPlugin::Integration::MessageType;

namespace UKControllerPluginTest::Integration {
    class MockInboundIntegrationMessageProcessor : public InboundIntegrationMessageProcessor
    {
        public:
            MOCK_METHOD(MessageType, MessageToProcess, (), (const, override));
            MOCK_METHOD(
                void,
                ProcessMessage,
                (int, std::shared_ptr<UKControllerPlugin::Integration::MessageInterface>), (override)
            );
    };

} // namespace UKControllerPluginTest::Integration
