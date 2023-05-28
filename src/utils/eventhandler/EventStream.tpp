#include "EventHandler.h"
#include "eventhandler/EuroscopeThreadEventSink.h"
#include "log/LoggerFunctions.h"
#include "task/RunAsyncTask.h"

namespace UKControllerPluginUtils::EventHandler {
    template <typename T>
    EventStream<T>::EventStream(std::shared_ptr<EuroscopeThreadEventSink> euroscopeThreadEventProcessor)
        : euroscopeThreadEventProcessor(euroscopeThreadEventProcessor)
    {
        assert(euroscopeThreadEventProcessor != nullptr && "Euroscope thread event processor cannot be null");
    }

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

            if ((handler.flags & EventHandlerFlags::EuroscopeThread) == EventHandlerFlags::EuroscopeThread) {
                this->euroscopeThreadEventProcessor->OnEvent([event, handler]() { handler.handler->OnEvent(event); });
                continue;
            }

            LogError("Unable to handle event of type " + std::string(typeid(T).name()) + " - handler flags invalid");
        }
    }
} // namespace UKControllerPluginUtils::EventHandler
