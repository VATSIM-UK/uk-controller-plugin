#include "EventBusFactory.h"
#include "MutableEventBus.h"

namespace UKControllerPluginUtils::EventHandler {

    MutableEventBus::MutableEventBus(std::shared_ptr<EuroscopeThreadEventSink> euroscopeThreadEventProcessor)
        : EventBus(std::move(euroscopeThreadEventProcessor))
    {
    }

    void MutableEventBus::Reset()
    {
        EventBus::singleton = nullptr;
        EventBus::factory = nullptr;
        LogInfo("EventBus shutdown");
    }

    void MutableEventBus::SetObserver(std::shared_ptr<EventObserver> eventObserver)
    {
        this->observer = std::move(eventObserver);
    }

    void MutableEventBus::SetFactory(std::shared_ptr<EventBusFactory> eventFactory)
    {
        EventBus::factory = std::move(eventFactory);
    }

    auto MutableEventBus::IsInitialised() -> bool
    {
        return EventBus::factory != nullptr;
    }
} // namespace UKControllerPluginUtils::EventHandler
