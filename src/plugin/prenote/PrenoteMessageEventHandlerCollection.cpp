#include "PrenoteMessageEventHandlerCollection.h"
#include "PrenoteMessageEventHandlerInterface.h"

namespace UKControllerPlugin::Prenote {
    void PrenoteMessageEventHandlerCollection::AddHandler(
        const std::shared_ptr<PrenoteMessageEventHandlerInterface>& handler)
    {
        assert(!handlers.contains(handler) && "Duplicate handler added to prenote message handlers");
        handlers.insert(handler);
    }

    void PrenoteMessageEventHandlerCollection::NewMessage(const PrenoteMessage& message) const
    {
        for (const auto& handler : handlers) {
            try {
                handler->NewMessage(message);
            } catch (const std::exception& e) {
                LogFatalExceptionAndRethrow(
                    "PrenoteMessageEventHandlerCollection::NewMessage", typeid(handler).name(), e);
            }
        }
    }

    void PrenoteMessageEventHandlerCollection::MessageCancelled(const PrenoteMessage& message) const
    {
        for (const auto& handler : handlers) {
            try {
                handler->MessageCancelled(message);
            } catch (const std::exception& e) {
                LogFatalExceptionAndRethrow(
                    "PrenoteMessageEventHandlerCollection::MessageCancelled", typeid(handler).name(), e);
            }
        }
    }

    void PrenoteMessageEventHandlerCollection::MessageAcknowledged(const PrenoteMessage& message) const
    {
        for (const auto& handler : handlers) {
            try {
                handler->MessageAcknowledged(message);
            } catch (const std::exception& e) {
                LogFatalExceptionAndRethrow(
                    "PrenoteMessageEventHandlerCollection::MessageAcknowledged", typeid(handler).name(), e);
            }
        }
    }

    void PrenoteMessageEventHandlerCollection::MessageTimeout(const PrenoteMessage& message) const
    {
        for (const auto& handler : handlers) {
            try {
                handler->MessageTimeout(message);
            } catch (const std::exception& e) {
                LogFatalExceptionAndRethrow(
                    "PrenoteMessageEventHandlerCollection::MessageTimeout", typeid(handler).name(), e);
            }
        }
    }

    auto PrenoteMessageEventHandlerCollection::CountHandlers() const -> size_t
    {
        return handlers.size();
    }
} // namespace UKControllerPlugin::Prenote
