#pragma once
#include "integration/IntegrationActionProcessor.h"
#include "integration/MessageType.h"
#include "integration/MessageInterface.h"

using UKControllerPlugin::Integration::IntegrationActionProcessor;
using UKControllerPlugin::Integration::MessageType;
using UKControllerPlugin::Integration::MessageInterface;

namespace UKControllerPluginTest::Integration {
    class MockIntegrationActionProcessor : public IntegrationActionProcessor
    {
        public:
            MOCK_METHOD(std::vector<MessageType>, ActionsToProcess, (), (const, override));
            ~MockIntegrationActionProcessor() override = default;

            void ProcessAction(
                std::shared_ptr<MessageInterface> message,
                std::function<void(void)> success,
                std::function<void(std::vector<std::string>)> fail
            ) override
            {
                success();
                fail({"foo", "bar"});
            };
    };

} // namespace UKControllerPluginTest::Integration
