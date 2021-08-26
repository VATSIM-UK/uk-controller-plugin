#pragma once
#include "integration/MessageType.h"

namespace UKControllerPlugin::Integration {
    class MessageInterface;

    /*
     * An interface for classes that wish to process inbound actions
     * from integrations.
     */
    class IntegrationActionProcessor
    {
        public:
            virtual ~IntegrationActionProcessor() = default;
            [[nodiscard]] virtual std::vector<MessageType> ActionsToProcess() const = 0;
            virtual void ProcessAction(
                std::shared_ptr<MessageInterface> message,
                std::function<void(void)> success,
                std::function<void(std::vector<std::string>)> fail
            ) = 0;
    };
} // namespace UKControllerPlugin::Integration
