#include "MockIntegrationActionProcessor.h"

namespace UKControllerPluginTest::Integration {
    MockIntegrationActionProcessor::MockIntegrationActionProcessor() = default;
    MockIntegrationActionProcessor::~MockIntegrationActionProcessor() = default;

    void MockIntegrationActionProcessor::ProcessAction(
        std::shared_ptr<MessageInterface> message,
        std::function<void(void)> success,
        std::function<void(std::vector<std::string>)> fail)
    {
        success();
        fail({"foo", "bar"});
    };
} // namespace UKControllerPluginTest::Integration
