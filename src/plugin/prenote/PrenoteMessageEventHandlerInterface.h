#pragma once

namespace UKControllerPlugin::Prenote {
    class PrenoteMessage;

    /**
     * An interface for classes that want to know about prenote messages.
     */
    class PrenoteMessageEventHandlerInterface
    {
        public:
        virtual ~PrenoteMessageEventHandlerInterface() = default;
        virtual void NewMessage(const PrenoteMessage& message)
        {
        }

        virtual void MessageCancelled(const PrenoteMessage& message)
        {
        }

        virtual void MessageAcknowledged(const PrenoteMessage& message)
        {
        }

        virtual void MessageTimeout(const PrenoteMessage& message)
        {
        }
    };
} // namespace UKControllerPlugin::Prenote
