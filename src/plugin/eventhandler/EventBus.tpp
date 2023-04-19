#include "EventObserver.h"
#include "EventStream.h"
#include "log/LoggerFunctions.h"

namespace UKControllerPlugin::EventHandler {

    /**
     * Get the given stream, or create if it doesn't exist
     */
    template <typename T> auto EventBus::GetStream() -> EventStream<T>&
    {
        const auto index = std::type_index(typeid(T));
        if (!streams.contains(index)) {
            streams.insert({index, std::any(std::make_shared<EventStream<T>>())});
        }

        return *std::any_cast<std::shared_ptr<EventStream<T>>>(streams.at(index));
    }

    template <typename T> void EventBus::AddHandler(std::shared_ptr<EventHandler<T>> handler, EventHandlerFlags flags)
    {
        this->GetStream<T>().AddHandler(handler, flags);
    }

    template <typename T> void EventBus::OnEvent(const T& event)
    {
        // Send the event to any observer
        if (observer != nullptr) {
            observer->OnEvent(event);
        }

        // Send the event to its stream
        this->GetStream<T>().OnEvent(event);
    }
} // namespace UKControllerPlugin::EventHandler
