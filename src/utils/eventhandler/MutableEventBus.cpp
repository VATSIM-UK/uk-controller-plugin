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

    void MutableEventBus::SetObserver(std::shared_ptr<EventObserver> observer)
    {
        this->observer = std::move(observer);
    }

    void MutableEventBus::SetFactory(std::shared_ptr<EventBusFactory> factory)
    {
        EventBus::factory = std::move(factory);
    }

    auto MutableEventBus::IsInitialised() -> bool
    {
        return EventBus::singleton != nullptr;
    }
} // namespace UKControllerPluginUtils::EventHandler
