#pragma once
#include "integration/InboundIntegrationMessageProcessor.h"
#include "integration/MessageType.h"
#include "integration/MessageInterface.h"

using UKControllerPlugin::Integration::InboundIntegrationMessageProcessor;
using UKControllerPlugin::Integration::MessageType;
using UKControllerPlugin::Integration::MessageInterface;

namespace UKControllerPluginTest::Integration {
    class MockInboundIntegrationMessageProcessor : public InboundIntegrationMessageProcessor
    {
        public:
            MOCK_METHOD(MessageType, MessageToProcess, (), (const, override));
            ~MockInboundIntegrationMessageProcessor() override = default;

            void ProcessMessage(
                std::shared_ptr<MessageInterface> message,
                std::function<void(std::shared_ptr<MessageInterface>)> sendResponse
            ) override
            {
                sendResponse(message);
            };
    };

} // namespace UKControllerPluginTest::Integration
