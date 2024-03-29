#pragma once
#include "integration/IntegrationActionProcessor.h"
#include "integration/MessageType.h"
#include "integration/MessageInterface.h"

using UKControllerPlugin::Integration::IntegrationActionProcessor;
using UKControllerPlugin::Integration::MessageInterface;
using UKControllerPlugin::Integration::MessageType;

namespace UKControllerPluginTest::Integration {
    class MockIntegrationActionProcessor : public IntegrationActionProcessor
    {
        public:
        MockIntegrationActionProcessor();
        virtual ~MockIntegrationActionProcessor();
        MOCK_METHOD(std::vector<MessageType>, ActionsToProcess, (), (const, override));
        void ProcessAction(
            std::shared_ptr<MessageInterface> message,
            std::function<void(void)> success,
            std::function<void(std::vector<std::string>)> fail) override;
    };

} // namespace UKControllerPluginTest::Integration
