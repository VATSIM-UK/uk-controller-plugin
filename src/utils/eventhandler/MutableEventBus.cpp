#include "EventBusFactory.h"
#include "MutableEventBus.h"

namespace UKControllerPluginUtils::EventHandler {

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
} // namespace UKControllerPluginUtils::EventHandler
