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
                try {
                    handler.handler->OnEvent(event);
                } catch (const std::exception& e) {
                    LogFatalExceptionAndRethrow(
                        "EventStream::OnEvent::Sync::" + std::string(typeid(T).name()), typeid(handler).name(), e);
                }
                continue;
            }

            if ((handler.flags & EventHandlerFlags::Async) == EventHandlerFlags::Async) {
                Async([event, handler]() {
                    try {
                        handler.handler->OnEvent(event);
                    } catch (const std::exception& e) {
                        LogFatalExceptionAndRethrow(
                            "EventStream::OnEvent::Async::" + std::string(typeid(T).name()), typeid(handler).name(), e);
                    }
                });
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
