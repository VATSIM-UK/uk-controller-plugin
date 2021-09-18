#include <utility>

#include "push/InterpretPushEvent.h"
#include "push/PushEventProtocolHandler.h"

using UKControllerPlugin::Push::PushEventConnectionInterface;
using UKControllerPlugin::Push::PushEventProcessorCollection;

namespace UKControllerPlugin::Push {

    PushEventProtocolHandler::PushEventProtocolHandler(
        std::shared_ptr<PushEventConnectionInterface> pushEvents, PushEventProcessorCollection& processors)
        : pushEvents(std::move(pushEvents)), processors(processors)
    {
    }

    /*
        Every time this event triggers, check for messages and hand
        them off to their processors.
    */
    void PushEventProtocolHandler::TimedEventTrigger()
    {
        std::string incomingMessage;

        while ((incomingMessage = this->pushEvents->GetNextMessage()) != this->pushEvents->noMessage) {
            PushEvent translatedMessage = InterpretPushedEvent(incomingMessage);

            if (translatedMessage == invalidMessage) {
                continue;
            }

            this->processors.ProcessEvent(translatedMessage);
        }
    }
} // namespace UKControllerPlugin::Push
