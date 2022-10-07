#include "PrenoteMessageEventHandlerCollection.h"
#include "PrenoteMessageEventHandlerInterface.h"

namespace UKControllerPlugin::Prenote {
    void PrenoteMessageEventHandlerCollection::AddHandler(
        const std::shared_ptr<PrenoteMessageEventHandlerInterface>& handler)
    {
        assert(handlers.insert(handler).second && "Duplicate handler added to prenote message handlers");
    }

    void PrenoteMessageEventHandlerCollection::NewMessage(const PrenoteMessage& message) const
    {
        for (const auto& handler : handlers) {
            handler->NewMessage(message);
        }
    }

    void PrenoteMessageEventHandlerCollection::MessageCancelled(const PrenoteMessage& message) const
    {
        for (const auto& handler : handlers) {
            handler->MessageCancelled(message);
        }
    }

    void PrenoteMessageEventHandlerCollection::MessageAcknowledged(const PrenoteMessage& message) const
    {
        for (const auto& handler : handlers) {
            handler->MessageAcknowledged(message);
        }
    }

    void PrenoteMessageEventHandlerCollection::MessageTimeout(const PrenoteMessage& message) const
    {
        for (const auto& handler : handlers) {
            handler->MessageTimeout(message);
        }
    }

    auto PrenoteMessageEventHandlerCollection::CountHandlers() const -> size_t
    {
        return handlers.size();
    }
} // namespace UKControllerPlugin::Prenote
