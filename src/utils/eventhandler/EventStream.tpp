#include "EventHandler.h"
#include "task/RunAsyncTask.h"

namespace UKControllerPluginUtils::EventHandler {
    template <typename T>
    void EventStream<T>::AddHandler(std::shared_ptr<EventHandler<T>> handler, EventHandlerFlags flags)
    {
        this->handlers.push_back({handler, flags});
    }

    template <typename T> void EventStream<T>::OnEvent(const T& event)
    {
        for (const auto& handler : this->handlers) {
            if ((handler.flags & EventHandlerFlags::Sync) == EventHandlerFlags::Sync) {
                handler.handler->OnEvent(event);
                continue;
            }

            if ((handler.flags & EventHandlerFlags::Async) == EventHandlerFlags::Async) {
                Async([event, handler]() { handler.handler->OnEvent(event); });
                continue;
            }
        }
    }
} // namespace UKControllerPluginUtils::EventHandler
