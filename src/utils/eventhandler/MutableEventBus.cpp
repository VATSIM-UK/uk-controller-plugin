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

    void MutableEventBus::SetObserver(std::shared_ptr<EventObserver> newObserver)
    {
        this->observer = std::move(newObserver);
    }

    void MutableEventBus::SetFactory(std::shared_ptr<EventBusFactory> newFactory)
    {
        EventBus::factory = std::move(newFactory);
    }

    auto MutableEventBus::IsInitialised() -> bool
    {
        return EventBus::factory != nullptr;
    }
} // namespace UKControllerPluginUtils::EventHandler
