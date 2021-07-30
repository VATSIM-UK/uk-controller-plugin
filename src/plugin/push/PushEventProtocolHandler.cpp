#include "pch/pch.h"
#include "push/PushEventProtocolHandler.h"
#include "push/InterpretPushEvent.h"

using UKControllerPlugin::Push::PushEventConnectionInterface;
using UKControllerPlugin::Push::PushEventProcessorCollection;

namespace UKControllerPlugin {
    namespace Push {

        PushEventProtocolHandler::PushEventProtocolHandler(
            PushEventConnectionInterface& pushEvents,
            PushEventProcessorCollection& processors
        ) : pushEvents(pushEvents), processors(processors)
        {
        }

        /*
            Every time this event triggers, check for messages and hand
            them off to their processors.
        */
        void PushEventProtocolHandler::TimedEventTrigger(void)
        {
            std::string incomingMessage;

            while ((incomingMessage = this->pushEvents.GetNextMessage()) != this->pushEvents.noMessage) {
                PushEvent translatedMessage = InterpretPushedEvent(incomingMessage);

                if (translatedMessage == invalidMessage) {
                    continue;
                }

                this->processors.ProcessEvent(translatedMessage);
            }
        }
    } // namespace Push
}  // namespace UKControllerPlugin
